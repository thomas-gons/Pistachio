

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.h"

#include <iostream>



int main()
{
    const unsigned int width = 800;
    const unsigned int height = 600;

    const glm::vec2 screen = glm::vec2(width, height);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // load glew
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }


    // build and compile our shader program
    auto shaderBuffer = Shader();
    shaderBuffer.addShader("vertex.glsl", GL_VERTEX_SHADER);
    shaderBuffer.addShader("fragment.glsl", GL_FRAGMENT_SHADER);
    shaderBuffer.compile();
    GLuint shaderProgram = shaderBuffer.getShaderProgram();

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    int samples = 4;
    float quadVerts[] = {
        -1.0, -1.0,     0.0, 0.0,
        -1.0, 1.0,      0.0, 1.0,
        1.0, -1.0,      1.0, 0.0,

        1.0, -1.0,      1.0, 0.0,
        -1.0, 1.0,      0.0, 1.0,
        1.0, 1.0,       1.0, 1.0
    };

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    shaderBuffer.setUniform("iResolution", screen);
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = glfwGetTime();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glm::vec2 iResolution;
        shaderBuffer.getUniform("iResolution", iResolution);

        std::cout << "iResolution: " << iResolution[0] << ", " << iResolution[1] << std::endl;
        float iTime = 0.0f;
        shaderBuffer.setUniform("iTime", iTime);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);


        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

