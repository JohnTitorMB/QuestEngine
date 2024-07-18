#include "CameraController.h"
#include "../Core/Entity.h"
#include "../Core/Inputs/InputSystem.h"
#include "../Math/Mathf.h"
#include "../Core/World.h"
#include "../Core/TimeManager.h"
CameraController::CameraController()
{
	m_pitch = 0;
	m_yaw = 0;
	m_controlCamera = false;
	m_cameraComponent = nullptr;
	m_sensibility = 0.2f;
	m_pitchMinValue = -89;
	m_pitchMaxValue = 89;
	movementSpeed = 10;
	m_inputMap = nullptr;
}

CameraController::CameraController(const CameraController& cameraController)
{
	m_pitch = cameraController.m_pitch;
	m_yaw = cameraController.m_yaw;
	m_controlCamera = cameraController.m_controlCamera;
	m_sensibility = cameraController.m_sensibility;
	m_pitchMinValue = cameraController.m_pitchMinValue;
	m_pitchMaxValue = cameraController.m_pitchMaxValue;
	m_averageDeltaX = cameraController.m_averageDeltaX;
	m_deltaX = cameraController.m_deltaX;
	m_deltaY = cameraController.m_deltaY;
	movementSpeed = cameraController.movementSpeed;
	m_inputMap = nullptr;
}

CameraController::~CameraController()
{
	InputSystem* inputSystem = InputSystem::Instance();
	inputSystem->DestroyInputMap(*m_inputMap);
}


void CameraController::Start()
{
	InputSystem* inputSystem = InputSystem::Instance();
	m_cameraComponent = GetOwnEntity()->GetComponent<CameraComponent>();

	m_inputMap = inputSystem->CreateInputMap();
	
	/* --------------------------------------------------Controller Action ------------------------------------*/
	InputAction& inputAction = m_inputMap->CreateInputAction("MoveRight");
	inputAction.Ongoing.AddListener(this, &CameraController::MoveRight);
	m_inputMap->BindInputCode("MoveRight", InputCode::E_KEY_RIGHT);

	InputAction&  inputAction2 = m_inputMap->CreateInputAction("MoveLeft");
	inputAction2.Ongoing.AddListener(this, &CameraController::MoveLeft);
	m_inputMap->BindInputCode("MoveLeft", InputCode::E_KEY_LEFT);

	InputAction&  inputAction3 = m_inputMap->CreateInputAction("MoveForward");
	inputAction3.Ongoing.AddListener(this, &CameraController::MoveForward);
	m_inputMap->BindInputCode("MoveForward", InputCode::E_KEY_UP);

	InputAction& inputAction4 = m_inputMap->CreateInputAction("MoveBackward");
	inputAction4.Ongoing.AddListener(this, &CameraController::MoveBackward);
	m_inputMap->BindInputCode("MoveBackward", InputCode::E_KEY_DOWN);

	InputAction& inputAction5 = m_inputMap->CreateInputAction("RotateX");
	inputAction5.OnModified.AddListener(this, &CameraController::RotateX);
	m_inputMap->BindInputCode("RotateX", InputCode::E_MOUSE_POS_Y);

	InputAction& inputAction6 = m_inputMap->CreateInputAction("RotateY");
	inputAction6.OnModified.AddListener(this, &CameraController::RotateY);
	m_inputMap->BindInputCode("RotateY", InputCode::E_MOUSE_POS_X);

	InputAction& inputAction7 = m_inputMap->CreateInputAction("ControlCamera");
	inputAction7.Started.AddListener(this, &CameraController::ControlCamera);
	m_inputMap->BindInputCode("ControlCamera", InputCode::E_MOUSE_BUTTON_RIGHT);

	InputAction& inputAction8 = m_inputMap->CreateInputAction("UnControlCamera");
	inputAction8.Started.AddListener(this, &CameraController::UnControlCamera);
	m_inputMap->BindInputCode("UnControlCamera", InputCode::E_KEY_ESCAPE);

	InputAction& inputAction11 = m_inputMap->CreateInputAction("ScrollMove");
	inputAction11.Started.AddListener(this, &CameraController::ScrollMove);
	m_inputMap->BindInputCode("ScrollMove", InputCode::E_MOUSE_SCROLL_Y);
	
	Event<> autoMoveEvent = Event<>();
	autoMoveEvent.AddListener(this, &CameraController::AutoMove, 0.1f);
	Timer& timer = TimeManager::Instance()->SetTimer(0.5f, true, autoMoveEvent);

	m_averageDeltaX.Resize(6);
	m_averageDeltaY.Resize(6);
}

void CameraController::Update()
{
	if (m_controlCamera)
	{	
		m_averageDeltaY.AddSample(m_deltaY);
		m_averageDeltaX.AddSample(m_deltaX);
	
		m_yaw += m_averageDeltaY.GetAverage();
		m_pitch += m_averageDeltaX.GetAverage();
		
		m_pitch = Mathf::Clamp(m_pitch, m_pitchMinValue, m_pitchMaxValue);

		Quaternion rot = m_cameraComponent->GetWorldRotation();
		Quaternion rotation = Quaternion::FromEulerAngle(Vector3D(m_pitch, m_yaw, 0.0f));
		m_cameraComponent->SetWorldRotation(rotation);
		
		m_deltaX = 0;
		m_deltaY = 0;
	}
}

void CameraController::ControlCamera(InputCallbackData data)
{
	m_controlCamera = !m_controlCamera;

	InputSystem* inputSystem = InputSystem::Instance();
	if(m_controlCamera)
		inputSystem->LockMouseCursor();
	else
		inputSystem->UnLockMouseCursor();
}

void CameraController::UnControlCamera(InputCallbackData data)
{
	InputSystem* inputSystem = InputSystem::Instance();
	m_controlCamera = false;
	inputSystem->UnLockMouseCursor();
}

void CameraController::AutoMove(float speed)
{
	Vector3D cameraPos = m_cameraComponent->GetWorldPosition();
	Vector3D cameraRight = m_cameraComponent->GetWorldRotation().GetRightDirection();
	m_cameraComponent->SetWorldPosition(cameraPos + cameraRight * movementSpeed * speed);

	std::cout << "AutoMove" << ", speed : " << speed << std::endl;
}

void CameraController::DigitalInputTestStarted(InputCallbackData data)
{
	std::cout << "DigitalInputTestStarted" << std::endl;
}

void CameraController::DigitalInputTestOnGoing(InputCallbackData data)
{
	std::cout << "DigitalInputTestOnGoing" << std::endl;
}

void CameraController::DigitalInputTestOnModified(InputCallbackData data)
{
	std::cout << "DigitalInputTestOnModified" << std::endl;
}

void CameraController::DigitalInputTestCompleted(InputCallbackData data)
{
	std::cout << "DigitalInputTestCompleted" << std::endl;
}

void CameraController::AnalogInputTestStarted(InputCallbackData data)
{
	std::cout << "AnalogInputTestStarted : " << data.m_value << std::endl;
}

void CameraController::AnalogInputTestOnGoing(InputCallbackData data)
{
	std::cout << "AnalogInputTestOnGoing : " << data.m_value << std::endl;
}

void CameraController::AnalogInputTestOnModified(InputCallbackData data)
{
	std::cout << "AnalogInputTestOnModifed : " << data.m_value << std::endl;
}

void CameraController::AnalogInputTestCompleted(InputCallbackData data)
{
	std::cout << "AnalogInputTestCompleted : " << data.m_value << std::endl;
}

Component* CameraController::Clone()
{
	CameraController* cameraController = new CameraController(*this);
	clonnedObject = cameraController;
	clonnedObject->baseObject = this;
	return cameraController;
}

void CameraController::AssignPointerAndReference()
{
	Component::AssignPointerAndReference();
	CameraController* baseCameraController = (CameraController*)baseObject;

	if (baseCameraController->m_cameraComponent != nullptr)
		m_cameraComponent = (CameraComponent*)baseCameraController->m_cameraComponent->clonnedObject;
}

void CameraController::MoveRight(InputCallbackData data)
{
	Vector3D cameraPos = m_cameraComponent->GetWorldPosition();
	Vector3D cameraRight = m_cameraComponent->GetWorldRotation().GetRightDirection();

	
	m_cameraComponent->SetWorldPosition(cameraPos + cameraRight * movementSpeed * TimeManager::Instance()->GetDeltaTime());
}

void CameraController::MoveLeft(InputCallbackData data)
{
	Vector3D cameraPos = m_cameraComponent->GetWorldPosition();
	Vector3D cameraRight = m_cameraComponent->GetWorldRotation().GetRightDirection();

	m_cameraComponent->SetWorldPosition(cameraPos + cameraRight  * -movementSpeed * TimeManager::Instance()->GetDeltaTime());
}

void CameraController::MoveForward(InputCallbackData data)
{
	Vector3D cameraPos = m_cameraComponent->GetWorldPosition();
	Vector3D cameraForward = m_cameraComponent->GetWorldRotation().GetForwardDirection();
	m_cameraComponent->SetWorldPosition(cameraPos + cameraForward * movementSpeed * TimeManager::Instance()->GetDeltaTime());
}

void CameraController::MoveBackward(InputCallbackData data)
{
	Vector3D cameraPos = m_cameraComponent->GetWorldPosition();
	Vector3D cameraForward = m_cameraComponent->GetWorldRotation().GetForwardDirection();
	m_cameraComponent->SetWorldPosition(cameraPos + cameraForward * -movementSpeed * TimeManager::Instance()->GetDeltaTime());
}

void CameraController::ScrollMove(InputCallbackData data)
{
	Vector3D cameraPos = m_cameraComponent->GetWorldPosition();
	Vector3D cameraForward = m_cameraComponent->GetWorldRotation().GetForwardDirection();
	m_cameraComponent->SetWorldPosition(cameraPos + cameraForward * data.m_value*10);
}

void CameraController::RotateX(InputCallbackData data)
{
	if (m_controlCamera)
		m_deltaX += data.m_deltaValue* m_sensibility;
}

void CameraController::RotateY(InputCallbackData data)
{
	if (m_controlCamera)
		m_deltaY += data.m_deltaValue * m_sensibility;
}