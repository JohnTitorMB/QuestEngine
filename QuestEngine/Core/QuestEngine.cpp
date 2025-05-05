#include "QuestEngine.h"
#include "AssetsManager.h"
#include "SceneManager.h"
#include "Graphics.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "../Editor/SimpleEditor.h"

QuestEngine::QuestEngine()
{
}

QuestEngine::~QuestEngine()
{
	delete m_window;
	m_window = nullptr;
}


void QuestEngine::Init()
{
	m_window = new Window(1280, 720, new char[] {"Opengl Window"});

	Graphics::GetInstance()->RefreshMSAASampleEvent.AddListener(this, &QuestEngine::RefreshMSAASample);

	//Init World
	World* world = World::Instance();
	world->InitWorld();

	InputSystem* inputSystem = InputSystem::Instance();

	// Game Engine Loop
	float cameraAngle = 0.0f;
	float stepLight = 1.0f;

	bool vsync = false;
	m_window->SetVsync(vsync);

	glDisable(GL_MULTISAMPLE);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // optional

	ImGui::StyleColorsDark(); // ou Light / Classic

	// Backend binding
	ImGui_ImplGlfw_InitForOpenGL(m_window->GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");


}

void QuestEngine::Update()
{
	InputSystem* inputSystem = InputSystem::Instance();
	World* world = World::Instance();




	while (!glfwWindowShouldClose(m_window->GetWindow()))
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		inputSystem->ProcessInput(m_window);

		glStencilMask(0xFF);
		glDepthMask(true);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClearDepth(1.0f);
		glClearStencil(0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
	    world->Update();

		SimpleEditor::Instance()->Display();

		world->Display(m_window);



		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(m_window->GetWindow());
	}

	World::Destroy();
	TimeManager::Destroy();
	AssetsManager::Destroy();
	InputSystem::Destroy();
	SceneManager::Destroy();
	SimpleEditor::Destroy();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void QuestEngine::RefreshMSAASample(int msaaSample)
{
	m_window->SetMSAASample(msaaSample);
	std::cout << "Refresh " << msaaSample << std::endl;
}
