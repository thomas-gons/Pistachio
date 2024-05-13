//
// Created by thomas on 23/01/24.
//
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "core/ecs/registry.h"
#include "core/resources/resourceManager.h"
#include "core/ecs/systemManager.h"
#include "core/ecs/component.h"


class Application {

private:
    GLFWwindow *_mWindow;
    GLint _mWidth = WIDTH;
    GLint _mHeight = HEIGHT;

    static void GLAPIENTRY debugCallback (
            GLenum source, GLenum type, GLuint id, GLenum severity,
            GLsizei length, const GLchar* message, const void* userParam)
    {
        std::cout << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "") << " type = "
                  << type << ", severity = " << severity << ", message = " << message << std::endl;
    }

public:

    Registry mRegistry;
    SystemManager mSystemManager;
    ResourceManager mResourceManager;
    Pool<GraphicsComponent> mGcPool;
    Pool<TransformComponent> mTcPool;

    Application();
    void run();
    void cleanUp();
};

#endif //__APPLICATION_H__