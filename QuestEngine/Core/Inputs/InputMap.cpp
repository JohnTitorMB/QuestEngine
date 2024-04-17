#include "InputMap.h"

InputMap::InputMap()
{
	
}

InputMap::~InputMap()
{
	ClearMap();
}
InputAction& InputMap::CreateInputAction(std::string inputActionName)
{
	if (m_inputActions.find(inputActionName) == m_inputActions.end())
	{
		m_inputActions[inputActionName] = new InputAction(inputActionName);
	}
	
	return *m_inputActions[inputActionName];
}

void InputMap::BindInputCode(std::string inputActionName, InputCode inputCode)
{
	InputSystem* inputSystem = InputSystem::Instance();

	m_inputscode[inputActionName].push_back(inputCode);

	if (m_inputActions.find(inputActionName) != m_inputActions.end())
		inputSystem->BindInputActionKey(inputCode, m_inputActions[inputActionName]);
}

void InputMap::UnBindInputCode(std::string inputActionName, InputCode inputCode)
{
	InputSystem* inputSystem = InputSystem::Instance();

	if (m_inputActions.find(inputActionName) != m_inputActions.end())
	{
		std::vector<InputCode>::iterator iterator = std::remove(m_inputscode[inputActionName].begin(), m_inputscode[inputActionName].end(), inputCode);
		m_inputscode[inputActionName].erase(iterator, m_inputscode[inputActionName].end());

		inputSystem->UnBindInputActionKey(inputCode, m_inputActions[inputActionName]);
	}
}

void InputMap::Enable()
{
	for (std::pair<std::string, InputAction*> inputAction : m_inputActions)
	{
		m_inputActions[inputAction.first]->Enable();
	}
}

void InputMap::Disable()
{
	for (std::pair<std::string, InputAction*> inputAction : m_inputActions)
	{
		m_inputActions[inputAction.first]->Disable();
	}
}

void InputMap::ClearMap()
{
	InputSystem* inputSystem = InputSystem::Instance();

	for (std::pair<std::string, InputAction*> inputAction : m_inputActions)
	{
		if (inputAction.second != nullptr)
		{
			for (InputCode inputCode : m_inputscode[inputAction.first])
				inputSystem->UnBindInputActionKey(inputCode, inputAction.second);

			delete inputAction.second;
		}
	}

	m_inputscode.clear();
	m_inputActions.clear();
}
