#include "AntiAliasingTestComponent.h"
#include "../Core/Inputs/InputSystem.h"
#include "../Core/Graphics.h"

AntiAliasingTestComponent::AntiAliasingTestComponent() : Component()
{
	m_inputMap = nullptr;
}

AntiAliasingTestComponent::AntiAliasingTestComponent(const AntiAliasingTestComponent& antiAliasingTestComponent) : Component(antiAliasingTestComponent)
{
	m_inputMap = nullptr;
}

void AntiAliasingTestComponent::Start()
{
	Component::Start();

	InputSystem* inputSystem = InputSystem::Instance();

	Graphics* graphics = Graphics::GetInstance();
	m_inputMap = inputSystem->CreateInputMap();

	InputAction& inputAction1 = m_inputMap->CreateInputAction("ScrollMove");
	inputAction1.Started.AddListener(this, &AntiAliasingTestComponent::ChangeSSAAMultiplier);
	inputAction1.Started.AddListener(this, &AntiAliasingTestComponent::ChangeMSAAMultiplier);
	m_inputMap->BindInputCode("ScrollMove", InputCode::E_MOUSE_SCROLL_Y);


	InputAction& inputAction2 = m_inputMap->CreateInputAction("Next");
	inputAction2.Started.AddListener(this, &AntiAliasingTestComponent::NextAntialiasingType);
	m_inputMap->BindInputCode("Next", InputCode::E_KEY_R);

	InputAction& inputAction3 = m_inputMap->CreateInputAction("Prev");
	inputAction3.Started.AddListener(this, &AntiAliasingTestComponent::PrevAntialiasingType);
	m_inputMap->BindInputCode("Prev", InputCode::E_KEY_E);

	Graphics::AntiAliasingType antiAliasingType = graphics->GetAntiAliasingType();


	if (antiAliasingType == Graphics::AntiAliasingType::SSAA)
		std::cout << "AntialiasingType : SSAA " << ", Multiplier : " << graphics->GetSSAAMultiplier() << std::endl;
	else if (antiAliasingType == Graphics::AntiAliasingType::MSAA)
		std::cout << "AntialiasingType : MSAA " << ", Multiplier : " << graphics->GetMSAASample() << std::endl;
	else if (antiAliasingType == Graphics::AntiAliasingType::MSAA_RT)
		std::cout << "AntialiasingType : MSAA RT" << ", Multiplier : " << graphics->GetMSAASample() << std::endl;
	else if (antiAliasingType == Graphics::AntiAliasingType::None)
		std::cout << "AntialiasingType : None" << std::endl;


	Graphics::GetInstance()->SetAntiAliasingType(antiAliasingType);
	m_cameraComponent->m_enableMultiSampling = false;
}


void AntiAliasingTestComponent::Update()
{
	Component::Update();
}




AntiAliasingTestComponent::~AntiAliasingTestComponent()
{
}

Component* AntiAliasingTestComponent::Clone()
{
	AntiAliasingTestComponent* antiAliasingTestComponent = new AntiAliasingTestComponent(*this);
	clonnedObject = antiAliasingTestComponent;
	clonnedObject->baseObject = this;
	return antiAliasingTestComponent;
}

void AntiAliasingTestComponent::AssignPointerAndReference()
{
	Component::AssignPointerAndReference();

	AntiAliasingTestComponent* base = (AntiAliasingTestComponent*)baseObject;

	if (base->m_cameraComponent != nullptr)
		m_cameraComponent = (CameraComponent*)base->m_cameraComponent->clonnedObject;
}

void AntiAliasingTestComponent::ChangeSSAAMultiplier(InputCallbackData data)
{
	if (Graphics::GetInstance()->GetAntiAliasingType() == Graphics::AntiAliasingType::SSAA)
	{
		int multiplier = Graphics::GetInstance()->GetSSAAMultiplier();

		if (data.m_value > 0)
		{
			multiplier++;
			if (multiplier == 0)
				multiplier = 2;
		}
		else if (data.m_value < 0)
		{
			multiplier--;

			if (multiplier == 0)
				multiplier = -2;
		}
		std::cout << "SSAA Multiplier : " << Graphics::GetInstance()->GetSSAAMultiplier() << std::endl;

		Graphics::GetInstance()->SetSSAAMultiplier(multiplier);
	}
}

void AntiAliasingTestComponent::ChangeMSAAMultiplier(InputCallbackData data)
{
	if (Graphics::GetInstance()->GetAntiAliasingType() == Graphics::AntiAliasingType::MSAA
		|| Graphics::GetInstance()->GetAntiAliasingType() == Graphics::AntiAliasingType::MSAA_RT)
	{
		int msaaSample = Graphics::GetInstance()->GetMSAASample();

		if (data.m_value > 0)
		{
			msaaSample++;
		}
		else if (data.m_value < 0 && msaaSample != 1)
		{
			msaaSample--;
		}

		std::cout << "MSAA Multiplier : " << Graphics::GetInstance()->GetMSAASample() << std::endl;

		Graphics::GetInstance()->SetMSAASample(msaaSample);
	}
}

void AntiAliasingTestComponent::NextAntialiasingType(InputCallbackData data)
{
	Graphics::AntiAliasingType antiAliasingType = Graphics::GetInstance()->GetAntiAliasingType();
	if (antiAliasingType != Graphics::AntiAliasingType::MSAA)
	{
		antiAliasingType = (Graphics::AntiAliasingType)((int)antiAliasingType + 1);

		Graphics::GetInstance()->SetAntiAliasingType(antiAliasingType);

		if (antiAliasingType == Graphics::AntiAliasingType::SSAA)
		{
			m_cameraComponent->m_enableMultiSampling = false;
			std::cout << "AntialiasingType : SSAA " << ", Multiplier : " << Graphics::GetInstance()->GetSSAAMultiplier() << std::endl;
		}
		else if (antiAliasingType == Graphics::AntiAliasingType::MSAA)
		{
			std::cout << "AntialiasingType : MSAA " << ", Multiplier : " << Graphics::GetInstance()->GetMSAASample() << std::endl;
			m_cameraComponent->m_enableMultiSampling = true;
		}
		else if (antiAliasingType == Graphics::AntiAliasingType::MSAA_RT)
		{
			m_cameraComponent->m_enableMultiSampling = true;
			std::cout << "AntialiasingType : MSAA RT" << ", Multiplier : " << Graphics::GetInstance()->GetMSAASample() << std::endl;
		}
		else if (antiAliasingType == Graphics::AntiAliasingType::None)
		{
			m_cameraComponent->m_enableMultiSampling = false;
			std::cout << "AntialiasingType : None" << std::endl;

		}
	}
}

void AntiAliasingTestComponent::PrevAntialiasingType(InputCallbackData data)
{
	Graphics::AntiAliasingType antiAliasingType = Graphics::GetInstance()->GetAntiAliasingType();
	if (antiAliasingType != Graphics::AntiAliasingType::None)
	{
		antiAliasingType = (Graphics::AntiAliasingType)((int)antiAliasingType - 1);
		Graphics::GetInstance()->SetAntiAliasingType(antiAliasingType);

		if (antiAliasingType == Graphics::AntiAliasingType::SSAA)
			std::cout << "AntialiasingType : SSAA " << ", Multiplier : " << Graphics::GetInstance()->GetSSAAMultiplier() << std::endl;
		else if (antiAliasingType == Graphics::AntiAliasingType::MSAA)
			std::cout << "AntialiasingType : MSAA " << ", Multiplier : " << Graphics::GetInstance()->GetMSAASample() << std::endl;
		else if (antiAliasingType == Graphics::AntiAliasingType::MSAA_RT)
			std::cout << "AntialiasingType : MSAA RT" << ", Multiplier : " << Graphics::GetInstance()->GetMSAASample() << std::endl;
		else if (antiAliasingType == Graphics::AntiAliasingType::None)
			std::cout << "AntialiasingType : None" << std::endl;
	}
}
