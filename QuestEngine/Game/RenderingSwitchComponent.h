#ifndef _RENDERINGSWITCHCOMPONENT_H_
#define _RENDERINGSWITCHCOMPONENT_H_

#include "../Core/Component.h"
#include "../Core/Events/EventCallback.h"
#include "../Core/Inputs/InputAction.h"
#include "../Core/Entity.h"
#include "../Core/Inputs/InputMap.h"
#include "../Core/Components/Camera.h"

class RenderingSwitchComponent : public Component
{
	InputMap* m_inputMap;

	enum class CameraMode
	{
		Default,
		Infinite,
		DefaultReversedZ,
		InfiniteReversedZ,

		Default01,
		Infinite01,
		DefaultReversedZ01,
		InfiniteReversedZ01,
	};

	CameraMode m_cameraMode;
protected:
	void Update() override;
	void Start() override;
	void DefaultMode(InputCallbackData data);
	void UVMode(InputCallbackData data);
	void NormalMode(InputCallbackData data);
	void FragCoordZMode(InputCallbackData data);
	void LinearDepthMode(InputCallbackData data);
	void FogMode(InputCallbackData data);
	void NextCameraMode(InputCallbackData data);
	void PrevCameraMode(InputCallbackData data);
	void RefreshCameraMode();
	void DisplayCurrentCameraMode();
public:
	RenderingSwitchComponent();
	RenderingSwitchComponent(const RenderingSwitchComponent& renderingSwitchComponent);
	~RenderingSwitchComponent();
	Component* Clone()override;
	void AssignPointerAndReference()override;
	CameraComponent* m_cameraComponent = nullptr;
};

#endif

