#ifndef _COMPONENTEXAMPLE_H_
#define _COMPONENTEXAMPLE_H_

#include "../Core/Component.h"
class ComponentExample : public Component
{
protected:
	void Update() override;
	void Start() override;
public:
	ComponentExample();
	ComponentExample(const ComponentExample& cameraController);
	~ComponentExample();
	Component* Clone()override;
	void AssignPointerAndReference()override;
};

#endif

