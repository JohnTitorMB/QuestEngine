#ifndef _INPUTMAP_H_
#define _INPUTMAP_H_
#include <unordered_map>
#include "InputSystem.h"

class InputMap
{
private:
	std::unordered_map<std::string, InputAction*> m_inputActions;
	std::unordered_map<std::string, std::vector<InputCode>> m_inputscode;
	InputMap();
	~InputMap();

	friend class InputSystem;
public:
	
	InputAction& CreateInputAction(std::string inputActionName);
	
	void BindInputCode(std::string inputActionName, InputCode inputCode);
	void UnBindInputCode(std::string inputActionName, InputCode inputCode);
	void Enable();
	void Disable();
	void ClearMap();
};

#endif
