#ifndef _INPUTSYSTEM_H_
#define _INPUTSYSTEM_H_
#include <unordered_map>
#include <set>
#include "InputAction.h"
#include "InputCode.h"

class InputMap;
class QuestEngine;
class Window;

class InputSystem
{
	friend class QuestEngine;
private:
	std::unordered_map<InputCode, float> m_inputsCurrentState;
	std::vector<std::pair<InputCode, float>> m_unhandledInput;
	std::set<InputCode> m_activeInput;
	
	std::unordered_map<InputCode, std::vector<InputAction*>> m_inputsActions;

	std::vector<InputMap*> m_inputMaps;
	static InputSystem* m_inputSystem;
	bool m_mustUpdateMouseLockInput;
	bool m_lockMouseInput = false;
private : 
	InputSystem();
	~InputSystem();
	void InitInputCodeState();
	static void Destroy();
public:
	static InputSystem* Instance();

	InputMap* CreateInputMap();
	void DestroyInputMap(InputMap& inputMap);

	void RegisterInput(Window* window, InputCode inputCode, float value);
	void BindInputActionKey(InputCode inputCode, InputAction* inputAction);
	void UnBindInputActionKey(InputCode inputCode, InputAction* inputAction);
	float GetInputState(InputCode inputCode);
	void ProcessInput(Window* window);
	void LockMouseCursor();
	void UnLockMouseCursor();
};

#endif