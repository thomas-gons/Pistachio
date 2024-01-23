//
// Created by thomas on 23/01/24.
//
#include "application.h"


Application::Application() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    // --------------------
    _window = glfwCreateWindow(this->_width, this->_height, "LearnOpenGL", nullptr, nullptr);
    if (_window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(_window);

    // load glew
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        exit(-1);
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(debugCallback, nullptr);

    this->registry = Registry();
    this->resourceManager = ResourceManager();
}

void Application::run() {
    while (!glfwWindowShouldClose(_window)) {
        glfwSwapBuffers(_window);
        glfwPollEvents();
    }
}

void Application::cleanUp() {
    glfwTerminate();

}
