//
// Created by thomas on 23/01/24.
//
#include "application.h"

ResourceManager resourceManager = ResourceManager();

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

    registry.create();
    resourceManager.loadTexture("flame", "resources/assets/flame.png");
    Sprite &sprite = resourceManager.getTexture("flame");
    Animation animation = Animation(12, 2, {8, 8});
    GraphicsComponent gc = GraphicsComponent("flame", &sprite, &animation);
    TransformComponent tc = TransformComponent(0, 0);
    registry.assign<GraphicsComponent>(0, gc);
    registry.assign<TransformComponent>(0, tc);

    Pool<GraphicsComponent> gcPool = registry.getPool<GraphicsComponent>();
    Pool<TransformComponent> tcPool = registry.getPool<TransformComponent>();

    std::cout << tcPool;
    std::cout << gcPool;
    exit(0);
    renderSystem = RenderSystem(gcPool, tcPool);
}

void Application::run() {
    while (!glfwWindowShouldClose(_window)) {
        renderSystem.update();
        glfwSwapBuffers(_window);
        glfwPollEvents();
    }
}

void Application::cleanUp() {
    glfwTerminate();

}
