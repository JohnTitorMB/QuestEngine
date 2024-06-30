#ifndef  _ROTATORCOMPONENT_H_
#define _ROTATORCOMPONENT_H_

#include "../Core/Component.h"
class RotatorComponnent : public Component
{
protected:
	void Update() override;
	void Start() override;
public:
	RotatorComponnent();
	RotatorComponnent(const RotatorComponnent& cameraController);
	~RotatorComponnent();
	Component* Clone()override;
	void AssignPointerAndReference()override;
};
#endif // ! _ROTATORCOMPONENT_H_
