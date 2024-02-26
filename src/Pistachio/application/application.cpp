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

    registry.create();
    resourceManager = ResourceManager();
    resourceManager.loadTexture("flame", "resources/assets/flame.png");
    resourceManager.loadShader("sprite", "resources/shaders/sprite_shader.glsl");
    Sprite &sprite = resourceManager.getTexture("flame");
    Animation animation = Animation(12, {8, 8}, 2);
    TransformComponent tc = TransformComponent(0, 0);
    GraphicsComponent gc = GraphicsComponent("flame", &sprite, &animation);
    registry.assign<GraphicsComponent>(0, gc);
    registry.assign<TransformComponent>(0, tc);


    gcPool = registry.getPool<GraphicsComponent>();
    tcPool = registry.getPool<TransformComponent>();

    renderSystem = RenderSystem(gcPool, tcPool);
    movementSystem = MovementSystem(tcPool);

    glfwSetKeyCallback(_window, InputSystem::keyCallback);
}

void Application::run() {
    while (!glfwWindowShouldClose(_window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        movementSystem.update();
        renderSystem.update(&resourceManager);
        glfwSwapBuffers(_window);
        glfwPollEvents();
    }
}

void Application::cleanUp() {
    glfwTerminate();
}
