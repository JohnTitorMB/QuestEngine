#include "InputAction.h"

InputAction::InputAction(std::string inputActionName)
{
	m_inputActionName = inputActionName;
	m_enable = true;
}

std::string InputAction::GetName() const
{
	return m_inputActionName;
}

void InputAction::Enable()
{
	m_enable = true;
}

void InputAction::Disable()
{
	m_enable = false;
}

bool InputAction::IsEnable()const
{
	return m_enable;
}
