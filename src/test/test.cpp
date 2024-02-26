//
// Created by thomas on 01/02/24.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "resourceManager/resourceManager.h"
#include "quadArray.h"

#define WIDTH 1920
#define HEIGHT 1080

#define N_SPRITES (uint) 1e6

struct QuadInfoNDC {
    glm::vec2 quad_xy;
    glm::vec2 quad_wh;
    glm::vec2 tex_uv;
};


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);


    // load glew
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        exit(-1);
    }

    auto resourceManager = ResourceManager();
    resourceManager.loadShader("test", "resources/shaders/test_shader.glsl");
    Shader sh = resourceManager.getShader("test");
    sh.use();

    resourceManager.loadTexture("slime", "resources/assets/slime.png");
    Texture tex = resourceManager.getTexture("slime");

    float vertices[] = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 1.0f,
            1.0f, 0.0f
    };

    auto *packed_vertices = new float[N_SPRITES * 6 ];
    // print memory needed for vertices
    std::cout << "Memory allocated: " << N_SPRITES * 6 * sizeof(float) / (1024 * 1024) << "Mb" << std::endl;

    for (int i = 0; i < N_SPRITES; i++) {
        for (int j = 0; j < 6; j++) {
            packed_vertices[i * 6 + j] = vertices[j];
        }
    }

    GLuint vao, vbo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(packed_vertices), packed_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    GLuint ssbo;
    glGenBuffers(1, &ssbo);

    GLsizeiptr dataSize = sizeof(QuadInfoNDC) * N_SPRITES + sizeof(glm::vec2);
    std::cout << "GPU memory allocated: " << dataSize / (1024 * 1024) << "Mb" << std::endl;
    exit(0);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);

    glBufferData(GL_SHADER_STORAGE_BUFFER, dataSize, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

    QuadInfoNDC *quadInfo = (QuadInfoNDC *) malloc(sizeof(QuadInfoNDC) * N_SPRITES);



    for (int i = 0; i < N_SPRITES; i++) {
        quadInfo[i].quad_xy = glm::vec2(rand() % 1920, rand() % 1080);
        quadInfo[i].quad_wh = glm::vec2(tex.getWidth() / 18, tex.getHeight() / 3);
        quadInfo[i].tex_uv = glm::vec2(0, 0);
    }

    uint row = 1, col = 0;

    uint n_frames = 18;
    uint n_rows = 3;
    float frame_duration = 0.08;
    glm::vec2 uvBase = glm::vec2(1/ (float) n_frames, 1 /(float) n_rows);

    glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(QuadInfoNDC) * N_SPRITES, sizeof(glm::vec2), &uvBase);
    double lastFrame;

    auto *frame = new int[N_SPRITES];
    auto *lastTime = new double[N_SPRITES];
    for (int i = 0; i < N_SPRITES; i++) {
        frame[i] = rand() % n_frames;
        lastTime[i] = glfwGetTime();
    }

    glFrontFace(GL_CW);
    glEnable(GL_CULL_FACE);

    while (!glfwWindowShouldClose(window)) {
        lastFrame = glfwGetTime();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, tex.getTextureID());

        for (int i = 0; i < N_SPRITES; i++) {
            quadInfo[i].quad_xy.x = 2 * (quadInfo[i].quad_xy.x / WIDTH) - 1;
            quadInfo[i].quad_xy.y = 2 * (quadInfo[i].quad_xy.y / HEIGHT) - 1;

            quadInfo[i].quad_wh.x = 2 * (quadInfo[i].quad_wh.x / WIDTH);
            quadInfo[i].quad_wh.y = 2 * (quadInfo[i].quad_wh.y / HEIGHT);

            quadInfo[i].tex_uv.x = uvBase.x * (float) frame[i];
            quadInfo[i].tex_uv.y = uvBase.y * (float) row;
        }

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, dataSize, quadInfo);

        glDrawArrays(GL_TRIANGLES, 0, N_SPRITES * 6);

        for (int i = 0; i < N_SPRITES; i++) {
            if (glfwGetTime() - lastTime[i] > frame_duration) {
                frame[i] = (frame[i] + 1) % n_frames;
                lastTime[i] = glfwGetTime();
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        std::cout << 1 / (glfwGetTime() - lastFrame) << std::endl;
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}