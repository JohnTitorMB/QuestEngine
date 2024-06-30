#ifndef _DIRECTIONAL_LIGHT_CONTROLLER_H_
#define _DIRECTIONALLIGHTCONTROLLER_H_
#include "../Core/Component.h"
#include "../Core/Components/DirectionalLight.h"
#include "../Core/Components/Camera.h"
class DirectionalLightControllerComponent : public Component
{
private :
	DirectionalLightComponent* m_directionalLightComponent;

protected:
	void Update() override;
	void Start() override;

public:

	DirectionalLightControllerComponent();
	DirectionalLightControllerComponent(const DirectionalLightControllerComponent& other);
	void SetDirectionalLightComponent(DirectionalLightComponent* directionalLightComponent);

	Component* Clone()override;
	void AssignPointerAndReference()override;

	CameraComponent* cameraComponent;
};
#endif
