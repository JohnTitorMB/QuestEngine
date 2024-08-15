#ifndef  _ROTATORCOMPONENT_H_
#define _ROTATORCOMPONENT_H_

#include "../Core/Component.h"
class RotatorComponent : public Component
{
protected:
	void Update() override;
	void Start() override;
public:
	RotatorComponent();
	~RotatorComponent();
	Component* Clone()override;
	void AssignPointerAndReference()override;
	float m_speed = 1.0f;
};
#endif // ! _ROTATORCOMPONENT_H_