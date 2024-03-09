#ifndef _INPUTACTION_H_
#define _INPUTACTION_H_
#include "../Events/Event.h"
#include <string>
class InputCallbackData
{
public:
	InputCallbackData(float value, float delta) : m_value{ value }, m_deltaValue{ delta } {};
	float m_value;
	float m_deltaValue;
};

class InputAction
{
	std::string m_inputActionName;
	bool m_enable;
public:
	InputAction() = default;

	InputAction(std::string inputActionName);
	std::string GetName()const;
	Event<InputCallbackData> Started;
	Event<InputCallbackData> Ongoing;
	Event<InputCallbackData> OnModified;
	Event<InputCallbackData> Completed;
	void Enable();
	void Disable();
	bool IsEnable()const;
};



#endif

