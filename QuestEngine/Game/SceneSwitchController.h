#ifndef _SceneSwitchController_H_
#define _SceneSwitchController_H_
#include "../Core/Component.h"
#include "../Core/Inputs/InputAction.h"
#include "../Core/Inputs/InputMap.h"

class SceneSwitchController : public Component
{
	InputMap* m_inputMap = nullptr;
	void SetScene0(InputCallbackData data);
	void SetScene1(InputCallbackData data);
	void SetScene2(InputCallbackData data);

protected:
	void Start() override;
	void Update() override;
public:
	SceneSwitchController();
	~SceneSwitchController();
	SceneSwitchController(const SceneSwitchController& other);
	Component* Clone()override;
	void AssignPointerAndReference()override;

	

};

#endif // !_SceneSwitchController_H_

