//
// Created by thomas on 01/02/24.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "resources/resourceManager.h"

#define WIDTH 800
#define HEIGHT 600

#define N_SPRITES (uint) 1

struct QuadInfoNDC {
    glm::vec2 mQuadXY;
    glm::vec2 mQuadWH;
    glm::vec2 mTexUV;
};


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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

    resourceManager.loadTexture("slime", "resources/assets/flame.png");
    Texture tex = resourceManager.getTexture("slime");

    GLuint vao;
    glGenVertexArrays(1, &vao);

    GLuint ssbo;
    glGenBuffers(1, &ssbo);

    GLsizeiptr dataSize = sizeof(QuadInfoNDC) * N_SPRITES + sizeof(glm::vec2);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);

    glBufferData(GL_SHADER_STORAGE_BUFFER, dataSize, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

    auto quadInfoBuffer = std::vector<QuadInfoNDC>(N_SPRITES);
    auto quadInfoBufferTmp = std::vector<QuadInfoNDC>(N_SPRITES);

    for (auto &quadInfo: quadInfoBuffer) {
        quadInfo.mQuadXY = glm::vec2(rand() % WIDTH, rand() % HEIGHT);
        quadInfo.mQuadWH = glm::vec2(tex.getWidth() / 8, tex.getHeight() / 2);
        quadInfo.mTexUV = glm::vec2(0, 0);
    }

    uint row = 1, col = 0;

    uint n_frames = 8;
    uint n_rows = 2;
    float frame_duration = 0.08;
    glm::vec2 uvBase = glm::vec2(1/ (float) n_frames, 1 /(float) n_rows);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec2), &uvBase);
    double lastFrame;

    auto *frame = new int[N_SPRITES];
    auto *lastTime = new double[N_SPRITES];
    for (int i = 0; i < N_SPRITES; i++) {
        frame[i] = rand() % n_frames;
        lastTime[i] = glfwGetTime();
    }

    int n = 0;

    double t;

    int i = 0;
    while (!glfwWindowShouldClose(window)) {
        t = glfwGetTime();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, tex.getTextureID());

        for (int i = 0; i < N_SPRITES; i++) {
            quadInfoBufferTmp[i].mQuadXY.x = 2 * (quadInfoBuffer[i].mQuadXY.x / WIDTH) - 1;
            quadInfoBufferTmp[i].mQuadXY.y = 2 * (quadInfoBuffer[i].mQuadXY.y / HEIGHT) - 1;

            quadInfoBufferTmp[i].mQuadWH.x = 2 * (quadInfoBuffer[i].mQuadWH.x / WIDTH);
            quadInfoBufferTmp[i].mQuadWH.y = 2 * (quadInfoBuffer[i].mQuadWH.y / HEIGHT);

            quadInfoBufferTmp[i].mTexUV.x = uvBase.x * (float) frame[i];
            quadInfoBufferTmp[i].mTexUV.y = uvBase.y * (float) row;
            std::cout << "Quad info: " << quadInfoBufferTmp[i].mQuadXY.x << ", " << quadInfoBufferTmp[i].mQuadXY.y
                      << std::endl;
            std::cout << "           " << quadInfoBufferTmp[i].mQuadWH.x << ", " << quadInfoBufferTmp[i].mQuadWH.y
                        << std::endl;
            std::cout << "           " << quadInfoBufferTmp[i].mTexUV.x << ", " << quadInfoBufferTmp[i].mTexUV.y
                        << std::endl;
            std::cout << "           " << uvBase.x << ", " << uvBase.y << std::endl;
            std::cout << std::endl;
        }

        lastFrame = glfwGetTime();
        for (int i = 0; i < N_SPRITES; i++) {
            if (lastFrame - lastTime[i] > frame_duration) {
                frame[i] = (frame[i] + 1) % n_frames;
                lastTime[i] = lastFrame;
            }
        }

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec2), sizeof(QuadInfoNDC) * N_SPRITES,
                        quadInfoBufferTmp.data());

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6 * N_SPRITES);

        glfwSwapBuffers(window);
        glfwPollEvents();
        if (++i == 10) {
        }
    }
    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}