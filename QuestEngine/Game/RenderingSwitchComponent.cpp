#include "RenderingSwitchComponent.h"
#include "../Core/Inputs/InputSystem.h"
#include "../Core/World.h"
RenderingSwitchComponent::RenderingSwitchComponent() : Component()
{

}

RenderingSwitchComponent::RenderingSwitchComponent(const RenderingSwitchComponent& renderingSwitchComponent) : Component(renderingSwitchComponent)
{
	m_cameraComponent = renderingSwitchComponent.m_cameraComponent;
}

void RenderingSwitchComponent::Start()
{
	Component::Start();

	InputSystem* inputSystem = InputSystem::Instance();
	m_inputMap = inputSystem->CreateInputMap();

	InputAction& inputAction = m_inputMap->CreateInputAction("DefaultMode");
	inputAction.Started.AddListener(new EventCallback<RenderingSwitchComponent, InputCallbackData>(this, &RenderingSwitchComponent::DefaultMode));
	m_inputMap->BindInputCode("DefaultMode", InputCode::E_KEY_1);

	InputAction& inputAction2 = m_inputMap->CreateInputAction("UVMode");
	inputAction2.Started.AddListener(new EventCallback<RenderingSwitchComponent, InputCallbackData>(this, &RenderingSwitchComponent::UVMode));
	m_inputMap->BindInputCode("UVMode", InputCode::E_KEY_2);

	InputAction& inputAction3 = m_inputMap->CreateInputAction("NormalMode");
	inputAction3.Started.AddListener(new EventCallback<RenderingSwitchComponent, InputCallbackData>(this, &RenderingSwitchComponent::NormalMode));
	m_inputMap->BindInputCode("NormalMode", InputCode::E_KEY_3);

	InputAction& inputAction4 = m_inputMap->CreateInputAction("FragCoordZMode");
	inputAction4.Started.AddListener(new EventCallback<RenderingSwitchComponent, InputCallbackData>(this, &RenderingSwitchComponent::FragCoordZMode));
	m_inputMap->BindInputCode("FragCoordZMode", InputCode::E_KEY_4);

	InputAction& inputAction5 = m_inputMap->CreateInputAction("LinearDepthMode");
	inputAction5.Started.AddListener(new EventCallback<RenderingSwitchComponent, InputCallbackData>(this, &RenderingSwitchComponent::LinearDepthMode));
	m_inputMap->BindInputCode("LinearDepthMode", InputCode::E_KEY_5);

	InputAction& inputAction6 = m_inputMap->CreateInputAction("FogMode");
	inputAction6.Started.AddListener(new EventCallback<RenderingSwitchComponent, InputCallbackData>(this, &RenderingSwitchComponent::FogMode));
	m_inputMap->BindInputCode("FogMode", InputCode::E_KEY_6);

	InputAction& inputAction8 = m_inputMap->CreateInputAction("PrevCameraMode");
	inputAction8.Started.AddListener(new EventCallback<RenderingSwitchComponent, InputCallbackData>(this, &RenderingSwitchComponent::PrevCameraMode));
	m_inputMap->BindInputCode("PrevCameraMode", InputCode::E_KEY_Q);

	InputAction& inputAction9 = m_inputMap->CreateInputAction("NextCameraMode");
	inputAction9.Started.AddListener(new EventCallback<RenderingSwitchComponent, InputCallbackData>(this, &RenderingSwitchComponent::NextCameraMode));
	m_inputMap->BindInputCode("NextCameraMode", InputCode::E_KEY_E);

}

void RenderingSwitchComponent::DefaultMode(InputCallbackData data)
{
	World::m_renderingType = RenderingType::Default;
	std::cout << "RenderingMode : Default" << std::endl;
}

void RenderingSwitchComponent::UVMode(InputCallbackData data)
{
	World::m_renderingType = RenderingType::UV;
	std::cout << "RenderingMode : UV" << std::endl;
}

void RenderingSwitchComponent::NormalMode(InputCallbackData data)
{
	World::m_renderingType = RenderingType::Normal;
	std::cout << "RenderingMode : Normal" << std::endl;
}

void RenderingSwitchComponent::FragCoordZMode(InputCallbackData data)
{
	World::m_renderingType = RenderingType::FragCoordZ;
	std::cout << "RenderingMode : FragCoordZ" << std::endl;
}

void RenderingSwitchComponent::LinearDepthMode(InputCallbackData data)
{
	World::m_renderingType = RenderingType::LinearDepth;
	std::cout << "RenderingMode : LinearDepth" << std::endl;
}

void RenderingSwitchComponent::FogMode(InputCallbackData data)
{
	World::m_renderingType = RenderingType::Fog;
	std::cout << "RenderingMode : FogMode" << std::endl;
}

void RenderingSwitchComponent::NextCameraMode(InputCallbackData data)
{
	if (m_cameraMode != CameraMode::InfiniteReversedZ01)
	{
		m_cameraMode = (CameraMode)((int)m_cameraMode + 1);
		RefreshCameraMode();
	}
}

void RenderingSwitchComponent::PrevCameraMode(InputCallbackData data)
{
	if (m_cameraMode != CameraMode::Default)
	{
		m_cameraMode = (CameraMode)((int)m_cameraMode - 1);
		RefreshCameraMode();
	}
}

void RenderingSwitchComponent::RefreshCameraMode()
{

	if (m_cameraComponent != nullptr)
	{
		m_cameraComponent->SetUseInfiniteProjection(false);
		m_cameraComponent->SetUseReversedZProjection(false);
		m_cameraComponent->SetUseDepthZeroToOneProjection(false);

		switch (m_cameraMode)
		{
		case CameraMode::Default:
			break;
		case CameraMode::Infinite:
			m_cameraComponent->SetUseInfiniteProjection(true);
			break;
		case CameraMode::DefaultReversedZ:
			m_cameraComponent->SetUseReversedZProjection(true);
			break;
		case CameraMode::InfiniteReversedZ:
			m_cameraComponent->SetUseInfiniteProjection(true);
			m_cameraComponent->SetUseReversedZProjection(true);
			break;
		case CameraMode::Default01:
			m_cameraComponent->SetUseDepthZeroToOneProjection(true);
			break;
		case CameraMode::Infinite01:
			m_cameraComponent->SetUseDepthZeroToOneProjection(true);
			m_cameraComponent->SetUseInfiniteProjection(true);
			break;
		case CameraMode::DefaultReversedZ01:
			m_cameraComponent->SetUseDepthZeroToOneProjection(true);
			m_cameraComponent->SetUseReversedZProjection(true);
			break;
		case CameraMode::InfiniteReversedZ01:
			m_cameraComponent->SetUseDepthZeroToOneProjection(true);
			m_cameraComponent->SetUseInfiniteProjection(true);
			m_cameraComponent->SetUseReversedZProjection(true);
			break;
		default:
			break;
		}

		DisplayCurrentCameraMode();
	}
}

void RenderingSwitchComponent::DisplayCurrentCameraMode()
{
	switch (m_cameraMode)
	{
	case CameraMode::Default:
		std::cout << "No infinite / No reversedZ / Range [-1; 1]" << std::endl;
		break;
	case CameraMode::Infinite:
		std::cout << "infinite / No reversedZ / Range [-1; 1]" << std::endl;
		break;
	case CameraMode::DefaultReversedZ:
		std::cout << "No infinite / reversedZ / Range [-1; 1]" << std::endl;
		break;
	case CameraMode::InfiniteReversedZ:
		std::cout << "infinite / reversedZ / Range [-1; 1]" << std::endl;
		break;
	case CameraMode::Default01:
		std::cout << "No infinite / No reversedZ / Range [0; 1]" << std::endl;
		break;
	case CameraMode::Infinite01:
		std::cout << "infinite / No reversedZ / Range [0; 1]" << std::endl;
		break;
	case CameraMode::DefaultReversedZ01:
		std::cout << "No infinite / reversedZ / Range [0; 1]" << std::endl;
		break;
	case CameraMode::InfiniteReversedZ01:
		std::cout << "infinite / reversedZ / Range [0; 1]" << std::endl;
		break;
	default:
		break;
	}
}

void RenderingSwitchComponent::Update()
{
	Component::Update();
}

RenderingSwitchComponent::~RenderingSwitchComponent()
{
}

Component* RenderingSwitchComponent::Clone()
{
	RenderingSwitchComponent* renderingSwitchComponent = new RenderingSwitchComponent(*this);
	clonnedObject = renderingSwitchComponent;
	clonnedObject->baseObject = this;
	return renderingSwitchComponent;
}

void RenderingSwitchComponent::AssignPointerAndReference()
{
	Component::AssignPointerAndReference();

	RenderingSwitchComponent* baseRenderingSwitchComponent = (RenderingSwitchComponent*)baseObject;

	if (baseRenderingSwitchComponent->m_cameraComponent != nullptr)
		m_cameraComponent = (CameraComponent*)baseRenderingSwitchComponent->m_cameraComponent->clonnedObject;
}
