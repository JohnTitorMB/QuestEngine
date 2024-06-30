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
	DirectionalLightEntity(const DirectionalLightEntity& other);
	void SetDirectionalLightComponent(DirectionalLightComponent* directionalLightComponent);

	Entity* Clone(bool isAddedOnScene = false)override;
	void AssignPointerAndReference()override;
};
#endif
