//
// Created by thomas on 23/01/24.
//
#pragma once

#include "core/registry.h"
#include "core/resourceManager/resourceManager.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Application {

private:
    GLFWwindow *_window;
    GLint _width = 1920;
    GLint _height = 1080;

    static void GLAPIENTRY debugCallback (
            GLenum source, GLenum type, GLuint id, GLenum severity,
            GLsizei length, const GLchar* message, const void* userParam)
    {
        std::cout << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "") << " type = "
                  << type << ", severity = " << severity << ", message = " << message << std::endl;
    }

public:

    Registry registry;
    ResourceManager resourceManager;

    Application();
    void run();
    void cleanUp();
};