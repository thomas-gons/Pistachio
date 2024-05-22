//
// Created by thomas on 23/01/24.
//
#include "application.h"


Application::Application() {
    if (pipe(_mPipe) == -1) {
        std::cerr << "Failed to create pipe" << std::endl;
        exit(-1);
    }

    int flags = fcntl(_mPipe[0], F_GETFL, 0);
    if (flags == -1) {
        std::cerr << "Failed to get flags" << std::endl;
        exit(-1);
    }

    if (fcntl(_mPipe[0], F_SETFL, flags | O_NONBLOCK) == -1) {
        std::cerr << "Failed to set flags" << std::endl;
        exit(-1);
    }

    dup2(_mPipe[1], STDOUT_FILENO);
    close(_mPipe[1]);
    fflush(stdout);

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
    Animation animation = Animation(12, {8, 8});
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

    _mModelManager.loadAllModels();
}

void Application::run() {
    int i = 0;
    while (!glfwWindowShouldClose(_mWindow)) {

        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        static bool firstLoop = true;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove;

        if (ImGui::Begin("Pistachio Editor", nullptr, window_flags)) {
            if (firstLoop) {
                ImGuiID id = ImGui::GetID("Pistachio Editor DockSpace");

                ImGui::DockBuilderAddNode(id);
                ImGuiID editor = ImGui::DockBuilderSplitNode(id, ImGuiDir_Right, 0.3f, nullptr, &id);
                ImGuiID terminal = ImGui::DockBuilderSplitNode(id, ImGuiDir_Down, 0.3f, nullptr, &id);
                ImGuiID scene = ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 0.7f, nullptr, &id);
                ImGui::DockBuilderDockWindow("Scene", scene);
                ImGui::DockBuilderDockWindow("Terminal", terminal);
                ImGui::DockBuilderDockWindow("Editor", editor);
                ImGui::DockBuilderFinish(id);

                ImGui::DockBuilderAddNode(id);
                ImGuiID components = ImGui::DockBuilderSplitNode(editor, ImGuiDir_Down, 0.5f, nullptr, &id);
                ImGui::DockBuilderDockWindow("Component Editor", components);
                ImGui::DockBuilderFinish(id);
                firstLoop = false;
            }
            ImGui::DockSpace(ImGui::GetID("Pistachio Editor DockSpace"), ImVec2(0, 0));

            setScene();
            setEditor();
            setTerminal();
            setComponentEditor();
        }
        ImGui::SetWindowPos(ImVec2(0, 0));
        ImGui::End();

        // Rendering
        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(_mWindow);
    }
    cleanUp();
}

void Application::setScene() {
    ImGui::Begin("Scene"); ImGui::End();
}

void Application::setEditor() {
    ImGui::Begin("Editor");
    ImGui::SetNextItemOpen(true);
    if (ImGui::TreeNode("General")) {
        if (ImGui::TreeNode("Textures")) {
            for (auto &textureTag : mResourceManager.getTextureTags()) {
                if (ImGui::Button(textureTag.c_str())) {
                    mResourceManager.getTexture(textureTag.c_str());
                }
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Shaders")) {
            for (auto &shaderTag : mResourceManager.getShaderTags()) {
                if (ImGui::Button(shaderTag.c_str())) {
                    mResourceManager.getShader(shaderTag.c_str());
                }
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Models")) {
            for (auto &modelTag : _mModelManager.getModelTags()) {
                if (ImGui::Button(modelTag.c_str())) {
                    _mCurrentModel = modelTag;
                }
            }
            ImGui::TreePop();
        }
        ImGui::TreePop();
//        ImGui::Image((void*)(intptr_t)mResourceManager.getTexture("flame").getTextureID(), ImVec2(100, 100));
    }
    ImGui::End();
}

void Application::setComponentEditor() {
    ImGui::Begin("Component Editor");
    if (!_mCurrentModel.empty()) {
        auto model = _mModelManager.getModel(_mCurrentModel);
        for (auto &componentTag: model.getComponentTags())
                // create a sub window for each component
                if (ImGui::CollapsingHeader(componentTag.c_str())) {
                    ImGui::Text("Component: %s", componentTag.c_str());
                }
    } else {
        ImGui::Text("No model selected");
    }
    ImGui::End();
}

void Application::setTerminal() {
    static char *buffer = new char[BUFFER_LOG_SIZE];
    if (read(_mPipe[0], buffer, BUFFER_LOG_SIZE) != -1)
        _mBuffer.append(buffer);
    ImGui::Begin("Terminal");
    ImGui::TextUnformatted(_mBuffer.begin(), _mBuffer.end());
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

    ImGui::End();
}

void Application::cleanUp() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}


void Application::typeMappingToImGuiNode(const std::string &type) {
    if (type == "int") {
        ImGui::InputInt("##int", nullptr);
    } else if (type == "float") {
        ImGui::InputFloat("##float", nullptr);
    } else if (type == "bool") {
        ImGui::Checkbox("##bool", nullptr);
    } else if (type == "string") {
        ImGui::InputText("##string", nullptr, 0);
    }
}
