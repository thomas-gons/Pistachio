//
// Created by thomas on 23/01/24.
//
#include "application.h"


Application::Application() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    _mWindow = glfwCreateWindow(_mWidth, _mHeight, "LearnOpenGL", nullptr, nullptr);
    if (_mWindow == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(_mWindow);

    // load glew
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        exit(-1);
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(debugCallback, nullptr);

    mRegistry.create();
    mResourceManager = ResourceManager();
    mResourceManager.loadTexture("flame", "resources/assets/flame.png");
    mResourceManager.loadShader("sprite", "resources/shaders/test_shader.glsl");
    mResourceManager.getShader("sprite").use();
    Sprite &sprite = mResourceManager.getTexture("flame");
    Animation animation = Animation(12, {8, 8}, 2);
    TransformComponent tc = TransformComponent(300, 300);
    GraphicsComponent gc = GraphicsComponent("flame", &sprite, &animation);
    mRegistry.assign<GraphicsComponent>(0, gc);
    mRegistry.assign<TransformComponent>(0, tc);


    mGcPool = mRegistry.getPool<GraphicsComponent>();
    mTcPool = mRegistry.getPool<TransformComponent>();

    mSystemManager = SystemManager();
    mSystemManager.addSystem<RenderSystem>(mGcPool, mTcPool);
    auto rs = mSystemManager.getSystem<RenderSystem>();
    rs->addSpriteBatch("flame");
    mSystemManager.addSystem<MovementSystem>(mTcPool);

    glfwSetKeyCallback(_mWindow, InputSystem::keyCallback);
}

void Application::run() {
    int i = 0;
    while (!glfwWindowShouldClose(_mWindow)) {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//        mSystemManager.update<MovementSystem>();
        mSystemManager.update<RenderSystem>();
        glfwSwapBuffers(_mWindow);
        glfwPollEvents();
        if (++i == 10) {
        }
    }
}

void Application::cleanUp() {
    glfwTerminate();
}
