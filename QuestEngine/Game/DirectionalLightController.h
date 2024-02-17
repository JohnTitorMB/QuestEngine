#ifndef _DIRECTIONAL_LIGHT_CONTROLLER_H_
#define _DIRECTIONALLIGHTCONTROLLER_H_
#include "../Core/Component.h"
#include "../Core/Components/DirectionalLight.h"

class DirectionalLightControllerComponent : public Component
{
private :
	DirectionalLightComponent* m_directionalLightComponent;

protected:
	void Update() override;
	void Start() override;

public:

	DirectionalLightControllerComponent();
	void SetDirectionalLightComponent(DirectionalLightComponent* directionalLightComponent);
};
#endif
