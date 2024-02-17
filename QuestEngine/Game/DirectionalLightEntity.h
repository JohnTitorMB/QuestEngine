#ifndef _DIRECTIONAL_LIGHT_ENTITY_H_
#define _DIRECTIONAL_LIGHT_ENTITY_H_
#include "../Core/Entity.h"
#include "../Core/Components/DirectionalLight.h"


class DirectionalLightEntity : public Entity
{

private :
	DirectionalLightComponent* m_directionalLightComponent;

protected:
	friend class World;
	void Start()override;
	void Update()override;

public:
	DirectionalLightEntity();
	void SetDirectionalLightComponent(DirectionalLightComponent* directionalLightComponent);
};
#endif
