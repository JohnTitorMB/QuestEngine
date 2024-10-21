#include "QuestEngine.h"
#include "AssetsManager.h"
#include "SceneManager.h"
#include "Graphics.h"
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
	m_window = new Window(1920, 1080, new char[] {"Opengl Window"});

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


}

void QuestEngine::Update()
{
	InputSystem* inputSystem = InputSystem::Instance();
	World* world = World::Instance();




	while (!glfwWindowShouldClose(m_window->GetWindow()))
	{
		glfwPollEvents();
		inputSystem->ProcessInput(m_window);

		glStencilMask(0xFF);
		glDepthMask(true);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClearDepth(1.0f);
		glClearStencil(0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
	    world->Update();

		world->Display(m_window);

		glfwSwapBuffers(m_window->GetWindow());
	}

	delete World::Instance();
	delete TimeManager::Instance();
	delete AssetsManager::Instance();
	delete InputSystem::Instance();
	delete SceneManager::Instance();
}

void QuestEngine::RefreshMSAASample(int msaaSample)
{
	m_window->SetMSAASample(msaaSample);
	std::cout << "Refresh " << msaaSample << std::endl;
}
