#include "SceneSwitchController.h"
#include "../Core/Inputs/InputSystem.h"
#include "../Core/SceneManager.h"
SceneSwitchController::SceneSwitchController()
{
	m_inputMap = nullptr;
}

SceneSwitchController::SceneSwitchController(const SceneSwitchController& other)
{
	m_inputMap = nullptr;
}

SceneSwitchController::~SceneSwitchController()
{
	InputSystem* inputSystem = InputSystem::Instance();
	inputSystem->DestroyInputMap(*m_inputMap);
}

void SceneSwitchController::Start()
{
	InputSystem* inputSystem = InputSystem::Instance();
	m_inputMap = inputSystem->CreateInputMap();

	InputAction& inputAction = m_inputMap->CreateInputAction("SetScene0");
	inputAction.Started.AddListener(new EventCallback<SceneSwitchController, InputCallbackData>(this, &SceneSwitchController::SetScene0));
	m_inputMap->BindInputCode("SetScene0", InputCode::E_KEY_F1);

	InputAction& inputAction2 = m_inputMap->CreateInputAction("SetScene1");
	inputAction2.Started.AddListener(new EventCallback<SceneSwitchController, InputCallbackData>(this, &SceneSwitchController::SetScene1));
	m_inputMap->BindInputCode("SetScene1", InputCode::E_KEY_F2);
}

void SceneSwitchController::Update()
{
}

Component* SceneSwitchController::Clone()
{
	SceneSwitchController* sceneSwitchController = new SceneSwitchController(*this);
	clonnedObject = sceneSwitchController;
	clonnedObject->baseObject = this;
	return sceneSwitchController;
}

void SceneSwitchController::AssignPointerAndReference()
{
	Component::AssignPointerAndReference();
}

void SceneSwitchController::SetScene0(InputCallbackData data)
{
	SceneManager::Instance()->LoadScene(0);
}

void SceneSwitchController::SetScene1(InputCallbackData data)
{
	SceneManager::Instance()->LoadScene(1);
}

void SceneSwitchController::SetScene2(InputCallbackData data)
{
	SceneManager::Instance()->LoadScene(2);
}
