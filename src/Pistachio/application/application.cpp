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

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(_mWindow, true);
    ImGui_ImplOpenGL3_Init("#version 460");

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
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        static bool firstLoop = true;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

        if (ImGui::Begin("Pistachio Editor", nullptr, window_flags)) {
            if (firstLoop) {

                ImGuiID id = ImGui::GetID("Pistachio Editor DockSpace");


                ImGui::DockBuilderRemoveNode(id);
                ImGui::DockBuilderAddNode(id);

                ImGuiID terminal = ImGui::DockBuilderSplitNode(id, ImGuiDir_Down, 0.3f, nullptr, &id);
                ImGui::DockBuilderDockWindow("Terminal", terminal);
                ImGuiID scene = ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 0.7f, nullptr, &id);
                ImGuiID editor = ImGui::DockBuilderSplitNode(id, ImGuiDir_Right, 0.3f, nullptr, &id);
                ImGui::DockBuilderDockWindow("Scene", scene);
                ImGui::DockBuilderDockWindow("Editor", editor);
                ImGui::DockBuilderFinish(id);
                firstLoop = false;
            }
            ImGui::DockSpace(ImGui::GetID("Pistachio Editor DockSpace"), ImVec2(0, 0));
            ImGui::Begin("Scene"); ImGui::End();
            ImGui::Begin("Editor"); ImGui::End();
            ImGui::Begin("Terminal"); ImGui::End();
        }
        ImGui::SetWindowPos(ImVec2(0, 0));
        ImGui::End();

        // Rendering
        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(_mWindow);
    }
}

void Application::cleanUp() {
    glfwTerminate();
}
