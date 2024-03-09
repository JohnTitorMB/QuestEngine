#include "CameraController.h"
#include "../Core/Inputs/InputMap.h"
#include "../Core/Entity.h"
#include "../Core/Inputs/InputSystem.h"
#include "../Math/Mathf.h"
#include "../Core/World.h"
CameraController::CameraController()
{
	m_pitch = 0;
	m_yaw = 0;
	m_controlCamera = false;
	m_cameraComponent = nullptr;
	m_sensibility = 0.2f;
	m_pitchMinValue = -89;
	m_pitchMaxValue = 89;
	movementSpeed = 0.1f;
}




void CameraController::Start()
{
	InputSystem* inputSystem = InputSystem::Instance();
	m_cameraComponent = GetOwnEntity()->GetComponent<CameraComponent>();

	InputMap& inputMap = inputSystem->CreateInputMap();

	
	/* --------------------------------------------------Controller Action ------------------------------------*/
	InputAction& inputAction = inputMap.CreateInputAction("MoveRight");
	inputAction.Ongoing.AddListener(new EventCallback<CameraController, InputCallbackData>(this, &CameraController::MoveRight));
	inputMap.BindInputCode("MoveRight", InputCode::E_KEY_RIGHT);

	InputAction&  inputAction2 = inputMap.CreateInputAction("MoveLeft");
	inputAction2.Ongoing.AddListener(new EventCallback<CameraController, InputCallbackData>(this, &CameraController::MoveLeft));
	inputMap.BindInputCode("MoveLeft", InputCode::E_KEY_LEFT);

	InputAction&  inputAction3 = inputMap.CreateInputAction("MoveForward");
	inputAction3.Ongoing.AddListener(new EventCallback<CameraController, InputCallbackData>(this, &CameraController::MoveForward));
	inputMap.BindInputCode("MoveForward", InputCode::E_KEY_UP);

	InputAction& inputAction4 = inputMap.CreateInputAction("MoveBackward");
	inputAction4.Ongoing.AddListener(new EventCallback<CameraController, InputCallbackData>(this, &CameraController::MoveBackward));
	inputMap.BindInputCode("MoveBackward", InputCode::E_KEY_DOWN);

	InputAction& inputAction5 = inputMap.CreateInputAction("RotateX");
	inputAction5.OnModified.AddListener(new EventCallback<CameraController, InputCallbackData>(this, &CameraController::RotateX));
	inputMap.BindInputCode("RotateX", InputCode::E_MOUSE_POS_Y);

	InputAction& inputAction6 = inputMap.CreateInputAction("RotateY");
	inputAction6.OnModified.AddListener(new EventCallback<CameraController, InputCallbackData>(this, &CameraController::RotateY));
	inputMap.BindInputCode("RotateY", InputCode::E_MOUSE_POS_X);

	InputAction& inputAction7 = inputMap.CreateInputAction("ControlCamera");
	inputAction7.Started.AddListener(new EventCallback<CameraController, InputCallbackData>(this, &CameraController::ControlCamera));
	inputMap.BindInputCode("ControlCamera", InputCode::E_MOUSE_BUTTON_RIGHT);

	auto m_eventUnControlCamera = new EventCallback<CameraController, InputCallbackData>(this, &CameraController::UnControlCamera);
	InputAction& inputAction8 = inputMap.CreateInputAction("UnControlCamera");
	inputAction8.Started.AddListener(m_eventUnControlCamera);
	inputMap.BindInputCode("UnControlCamera", InputCode::E_KEY_ESCAPE);

	auto m_eventScrollMoveCallback = new EventCallback<CameraController, InputCallbackData>(this, &CameraController::ScrollMove);
	InputAction& inputAction11 = inputMap.CreateInputAction("ScrollMove");
	inputAction11.Started.AddListener(m_eventScrollMoveCallback);
	inputMap.BindInputCode("ScrollMove", InputCode::E_MOUSE_SCROLL_Y);
	
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

		Quaternion rot = m_cameraComponent->GetRotation();
		Quaternion rotation = Quaternion::FromEulerAngle(Vector3D(m_pitch, m_yaw, 0.0f));
		m_cameraComponent->SetRotation(rotation);
		
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

void CameraController::MoveRight(InputCallbackData data)
{
	Vector3D cameraPos = m_cameraComponent->GetPosition();
	Vector3D cameraRight = m_cameraComponent->GetRotation().GetRightDirection();

	m_cameraComponent->SetPosition(cameraPos + cameraRight * movementSpeed);
}

void CameraController::MoveLeft(InputCallbackData data)
{
	Vector3D cameraPos = m_cameraComponent->GetPosition();
	Vector3D cameraRight = m_cameraComponent->GetRotation().GetRightDirection();

	m_cameraComponent->SetPosition(cameraPos + cameraRight  * -movementSpeed);
}

void CameraController::MoveForward(InputCallbackData data)
{
	Vector3D cameraPos = m_cameraComponent->GetPosition();
	Vector3D cameraForward = m_cameraComponent->GetRotation().GetForwardDirection();
	m_cameraComponent->SetPosition(cameraPos + cameraForward * movementSpeed);
}

void CameraController::MoveBackward(InputCallbackData data)
{
	Vector3D cameraPos = m_cameraComponent->GetPosition();
	Vector3D cameraForward = m_cameraComponent->GetRotation().GetForwardDirection();
	m_cameraComponent->SetPosition(cameraPos + cameraForward * -movementSpeed);
}

void CameraController::ScrollMove(InputCallbackData data)
{
	Vector3D cameraPos = m_cameraComponent->GetPosition();
	Vector3D cameraForward = m_cameraComponent->GetRotation().GetForwardDirection();
	m_cameraComponent->SetPosition(cameraPos + cameraForward * data.m_value);
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