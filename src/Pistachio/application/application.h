//
// Created by thomas on 23/01/24.
//
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

#include "core/ecs/registry.h"
#include "core/resources/resourceManager.h"
#include "core/ecs/systemManager.h"
#include "core/ecs/component.h"

#define BUFFER_LOG_SIZE 4096

class Application {

private:
    GLFWwindow *_mWindow;
    GLint _mWidth = WIDTH;
    GLint _mHeight = HEIGHT;

    // use pipe to redirect stdout to terminal
    int _mPipe[2];
    ImGuiTextBuffer _mBuffer;

    static void GLAPIENTRY debugCallback (
            GLenum source, GLenum type, GLuint id, GLenum severity,
            GLsizei length, const GLchar* message, const void* userParam)
    {
        std::cout << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "") << " type = "
                  << type << ", severity = " << severity << ", message = " << message << std::endl;
    }

    void setEditor();
    void setComponentEditor();
    void setTerminal();
    void setScene();


public:

    Registry mRegistry;
    SystemManager mSystemManager;
    ResourceManager mResourceManager;
    Pool<GraphicsComponent> mGcPool;
    Pool<TransformComponent> mTcPool;

    Application();
    void run();
    static void cleanUp();
};

#endif //__APPLICATION_H__