#include "InputSystem.h"
#include <iostream>
#include "../Window.h"
#include "InputAction.h"
#include "InputMap.h"
InputSystem* InputSystem::m_inputSystem = nullptr;

InputSystem::InputSystem()
{
	InitInputCodeState();
}

InputSystem::~InputSystem()
{
	for (InputMap* inputMap : m_inputMaps)
		delete inputMap;
}

InputSystem* InputSystem::Instance()
{
	if (!m_inputSystem)
		m_inputSystem = new InputSystem();

	return m_inputSystem;
}

InputMap* InputSystem::CreateInputMap()
{
	InputMap* inputMap = new InputMap();
	m_inputMaps.push_back(inputMap);
	return inputMap;
}

void InputSystem::DestroyInputMap(InputMap& inputMap)
{
	std::vector<InputMap*>::iterator iterator = std::remove(m_inputMaps.begin(), m_inputMaps.end(), &inputMap);
	inputMap.ClearMap();
	m_inputMaps.erase(iterator, m_inputMaps.end());

	delete (&inputMap);

}

void InputSystem::RegisterInput(Window* window, InputCode inputCode, float value)
{
	m_unhandledInput.push_back(std::pair<InputCode, float>(inputCode, value));
}

void InputSystem::BindInputActionKey(InputCode InputCode, InputAction* inputAction)
{
	m_inputsActions[InputCode].push_back(inputAction);
}

void InputSystem::UnBindInputActionKey(InputCode InputCode, InputAction* inputAction)
{
	std::vector<InputAction*>::iterator iterator = std::remove(m_inputsActions[InputCode].begin(), m_inputsActions[InputCode].end(), inputAction);
	m_inputsActions[InputCode].erase(iterator, m_inputsActions[InputCode].end());
}

void InputSystem::ProcessInput(Window* window)
{
	
	std::set<InputCode> currentPressedKeys = std::set<InputCode>(m_activeInput);
	float mouseXInputState = GetInputState(InputCode::E_MOUSE_POS_X);
	float mouseYInputState = GetInputState(InputCode::E_MOUSE_POS_Y);

	float mouseScrollXInputState = GetInputState(InputCode::E_MOUSE_SCROLL_X);
	float mouseScrollYInputState = GetInputState(InputCode::E_MOUSE_SCROLL_Y);

	bool mouseXIsModified = false;
	bool mouseYIsModified = false;

	bool mouseScrollXIsModified = false;
	bool mouseScrollYIsModified = false;
	//Process Input
	for (auto it = m_unhandledInput.begin(); it != m_unhandledInput.end(); it++)
	{
		std::pair<InputCode, float> inputState = (*it);

		InputCode inputCode = inputState.first;
		float State = inputState.second;

		bool isAnalogInput = inputCode == InputCode::E_MOUSE_POS_X || inputCode == InputCode::E_MOUSE_POS_Y || inputCode == InputCode::E_MOUSE_SCROLL_X || inputCode == InputCode::E_MOUSE_SCROLL_Y;
		bool isActiveInput = m_activeInput.find(inputCode) != m_activeInput.end();
		if (isAnalogInput)
			m_activeInput.insert(inputCode);
		else
		{
			if (State != 0.0f)
				m_activeInput.insert(inputCode);
			else
				m_activeInput.erase(inputCode);
		}

		if (inputCode == InputCode::E_MOUSE_POS_X)
			mouseXIsModified = true;
		else if (inputCode == InputCode::E_MOUSE_POS_Y)
			mouseYIsModified = true;
		else if (inputCode == InputCode::E_MOUSE_SCROLL_X)
			mouseScrollXIsModified = true;
		else if (inputCode == InputCode::E_MOUSE_SCROLL_Y)
			mouseScrollYIsModified = true;

		float currentState = m_inputsCurrentState[inputCode];
		m_inputsCurrentState[inputCode] = State;

		//Input Started
		if (currentState == 0.0f && State != 0.0f || (isAnalogInput && !isActiveInput))
		{
			if (m_inputsActions.find(inputCode) != m_inputsActions.end())
			{
				for (InputAction* inputAction : m_inputsActions[inputCode])
				{
					if (inputAction->IsEnable())
					{
						InputCallbackData inputCallbackData = InputCallbackData(State, State - currentState);
						inputAction->Started.Trigger(inputCallbackData);
					}
				}
			}
		}
		//Input Completed
		else if (!isAnalogInput && currentState != 0.0f && State == 0.0f)
		{
			for (InputAction* inputAction : m_inputsActions[inputCode])
			{
				if (inputAction->IsEnable())
				{
					InputCallbackData inputCallbackData = InputCallbackData(State, State - currentState);
					inputAction->Completed.Trigger(inputCallbackData);
				}
			}
		}
		 
		//Input Modified
		for (InputAction* inputAction : m_inputsActions[inputCode])
		{
			if (inputAction->IsEnable())
			{
				InputCallbackData inputCallbackData = InputCallbackData(State, State - currentState);
				inputAction->OnModified.Trigger(inputCallbackData);
			}
		}
	}

	m_unhandledInput.clear();	

	//Check Completed Mouse Scroll X
	if (!mouseScrollXIsModified && m_activeInput.find(InputCode::E_MOUSE_SCROLL_X) != m_activeInput.end())
	{
		m_inputsCurrentState[InputCode::E_MOUSE_SCROLL_X] = 0.0f;
		m_activeInput.erase(InputCode::E_MOUSE_SCROLL_X);

		for (InputAction* inputAction : m_inputsActions[InputCode::E_MOUSE_SCROLL_X])
		{
			if (inputAction->IsEnable())
			{
				InputCallbackData inputCallbackData = InputCallbackData(0.0f, -mouseScrollXInputState);
				inputAction->Completed.Trigger(inputCallbackData);
			}
		}
	}

	//Check Completed Mouse Scroll Y
	if (!mouseScrollYIsModified && m_activeInput.find(InputCode::E_MOUSE_SCROLL_Y) != m_activeInput.end())
	{
		m_inputsCurrentState[InputCode::E_MOUSE_SCROLL_Y] = 0.0f;
		m_activeInput.erase(InputCode::E_MOUSE_SCROLL_Y);

		for (InputAction* inputAction : m_inputsActions[InputCode::E_MOUSE_SCROLL_Y])
		{
			if (inputAction->IsEnable())
			{
				InputCallbackData inputCallbackData = InputCallbackData(0.0f, -mouseScrollYInputState);
				inputAction->Completed.Trigger(inputCallbackData);
			}
		}
	}


	//Check Mouse Position X
	float newMouseXInputState = GetInputState(InputCode::E_MOUSE_POS_X);

	for (InputAction* inputAction : m_inputsActions[InputCode::E_MOUSE_POS_X])
	{
		if (inputAction->IsEnable())
		{
			InputCallbackData inputCallbackData = InputCallbackData(newMouseXInputState, newMouseXInputState - mouseXInputState);
			inputAction->Ongoing.Trigger(inputCallbackData);
		}
	}

	if (!mouseXIsModified && m_activeInput.find(InputCode::E_MOUSE_POS_X) != m_activeInput.end())
	{
		m_activeInput.erase(InputCode::E_MOUSE_POS_X);

		for (InputAction* inputAction : m_inputsActions[InputCode::E_MOUSE_POS_X])
		{
			if (inputAction->IsEnable())
			{
				InputCallbackData inputCallbackData = InputCallbackData(newMouseXInputState, -mouseXInputState);
				inputAction->Completed.Trigger(inputCallbackData);
			}
		}
	}

	//Check Mouse Position Y
	float newMouseYInputState = GetInputState(InputCode::E_MOUSE_POS_Y);

	for (InputAction* inputAction : m_inputsActions[InputCode::E_MOUSE_POS_Y])
	{
		if (inputAction->IsEnable())
		{
			InputCallbackData inputCallbackData = InputCallbackData(newMouseYInputState, newMouseYInputState - mouseYInputState);
			inputAction->Ongoing.Trigger(inputCallbackData);
		}
	}

	if (!mouseYIsModified && m_activeInput.find(InputCode::E_MOUSE_POS_Y) != m_activeInput.end())
	{
		m_activeInput.erase(InputCode::E_MOUSE_POS_Y);

		for (InputAction* inputAction : m_inputsActions[InputCode::E_MOUSE_POS_Y])
		{
			if (inputAction->IsEnable())
			{
				InputCallbackData inputCallbackData = InputCallbackData(newMouseYInputState, -mouseYInputState);
				inputAction->Completed.Trigger(inputCallbackData);
			}
		}
	}	


	//Process Active Input Ongoing Callback
	for (InputCode key : m_activeInput)
	{
		bool isAnalogInput = key == InputCode::E_MOUSE_POS_X || key == InputCode::E_MOUSE_POS_Y;

		//KeyRepeated
		if (!isAnalogInput)
		{
			for (InputAction* inputAction : m_inputsActions[key])
			{
				if (inputAction->IsEnable())
				{
					InputCallbackData inputCallbackData = InputCallbackData(m_inputsCurrentState[key], 0.0f);
					inputAction->Ongoing.Trigger(inputCallbackData);
				}
			}
		}
	}


	if (m_mustUpdateMouseLockInput)
	{
		window->LockMouseInput(m_lockMouseInput);
		m_mustUpdateMouseLockInput = false;
	}
}

float InputSystem::GetInputState(InputCode inputCode)
{
	return m_inputsCurrentState[inputCode];
}

void InputSystem::LockMouseCursor()
{
	m_lockMouseInput = true;
	m_mustUpdateMouseLockInput = true;
}

void InputSystem::UnLockMouseCursor()
{
	m_lockMouseInput = false;
	m_mustUpdateMouseLockInput = true;
}

void InputSystem::InitInputCodeState()
{
	m_inputsCurrentState[InputCode::E_UNKNOWN] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_SPACE] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_APOSTROPHE] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_COMMA] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_MINUS] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_PERIOD] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_SLASH] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_0] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_1] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_2] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_3] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_4] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_5] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_6] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_7] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_8] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_9] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_SEMICOLON] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_EQUAL] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_A] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_B] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_C] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_D] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_E] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_F] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_G] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_H] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_I] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_J] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_K] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_L] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_M] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_N] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_O] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_P] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_Q] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_R] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_S] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_T] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_U] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_V] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_W] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_X] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_Y] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_Z] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_LEFT_BRACKET] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_BACKSLASH] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_RIGHT_BRACKET] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_GRAVE_ACCENT] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_WORLD_1] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_WORLD_2] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_ESCAPE] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_ENTER] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_TAB] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_BACKSPACE] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_INSERT] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_DELETE] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_RIGHT] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_LEFT] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_DOWN] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_UP] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_PAGE_UP] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_PAGE_DOWN] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_HOME] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_END] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_CAPS_LOCK] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_SCROLL_LOCK] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_NUM_LOCK] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_PRINT_SCREEN] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_PAUSE] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_F1] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_F2] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_F3] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_F4] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_F5] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_F6] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_F7] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_F8] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_F9] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_F10] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_F11] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_F12] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_F13] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_F14] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_F15] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_F16] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_F17] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_F18] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_F19] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_F20] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_F21] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_F22] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_F23] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_F24] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_F25] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_KP_0] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_KP_1] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_KP_2] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_KP_3] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_KP_4] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_KP_5] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_KP_6] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_KP_7] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_KP_8] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_KP_9] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_KP_DECIMAL] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_KP_DIVIDE] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_KP_MULTIPLY] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_KP_SUBTRACT] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_KP_ADD] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_KP_ENTER] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_KP_EQUAL] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_LEFT_SHIFT] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_LEFT_CONTROL] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_LEFT_ALT] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_LEFT_SUPER] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_RIGHT_SHIFT] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_RIGHT_CONTROL] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_RIGHT_ALT] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_RIGHT_SUPER] = 0.0f;
	m_inputsCurrentState[InputCode::E_KEY_MENU] = 0.0f;

	m_inputsCurrentState[InputCode::E_MOUSE_BUTTON_1] = 0.0f;
	m_inputsCurrentState[InputCode::E_MOUSE_BUTTON_2] = 0.0f;
	m_inputsCurrentState[InputCode::E_MOUSE_BUTTON_3] = 0.0f;
	m_inputsCurrentState[InputCode::E_MOUSE_BUTTON_4] = 0.0f;
	m_inputsCurrentState[InputCode::E_MOUSE_BUTTON_5] = 0.0f;
	m_inputsCurrentState[InputCode::E_MOUSE_BUTTON_6] = 0.0f;
	m_inputsCurrentState[InputCode::E_MOUSE_BUTTON_7] = 0.0f;
	m_inputsCurrentState[InputCode::E_MOUSE_BUTTON_8] = 0.0f;
	m_inputsCurrentState[InputCode::E_MOUSE_POS_X] = 0.0f;
	m_inputsCurrentState[InputCode::E_MOUSE_POS_Y] = 0.0f;
	m_inputsCurrentState[InputCode::E_MOUSE_SCROLL_X] = 0.0f;
	m_inputsCurrentState[InputCode::E_MOUSE_SCROLL_Y] = 0.0f;
}

void InputSystem::Destroy()
{
	delete m_inputSystem;
	m_inputSystem = nullptr;
}
