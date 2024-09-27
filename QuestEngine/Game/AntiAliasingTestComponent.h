#ifndef _SSAAMULTIPLIER_H_
#define _SSAAMULTIPLIER_H_
#include "../Core/Events/EventCallback.h"
#include "../Core/Inputs/InputAction.h"
#include "../Core/Inputs/InputMap.h"

#include "../Core/Component.h"
#include "../Core/Components/Camera.h"
class AntiAliasingTestComponent : public Component
{

	InputMap* m_inputMap = nullptr;
protected:
	void Update() override;
	void Start() override;
public:
	AntiAliasingTestComponent();
	AntiAliasingTestComponent(const AntiAliasingTestComponent& ssaaMultiplier);
	~AntiAliasingTestComponent();
	Component* Clone()override;
	void AssignPointerAndReference()override;
	void ChangeSSAAMultiplier(InputCallbackData data);
	void ChangeMSAAMultiplier(InputCallbackData data);

	void NextAntialiasingType(InputCallbackData data);
	void PrevAntialiasingType(InputCallbackData data);

	CameraComponent* m_cameraComponent = nullptr;

};

#endif

