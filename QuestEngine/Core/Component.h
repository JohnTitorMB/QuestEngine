#ifndef  _COMPONENT_H_
#define _COMPONENT_H_
#include "Object.h"
class Entity;
class Component : public Object
{
	friend class Entity;

public:
	virtual ~Component();
	Component();
	Component(const Component& other);
	virtual void Update() {};
	virtual void Start() {};
	Entity* GetOwnEntity();
	virtual Component* Clone();
	virtual void AssignPointerAndReference();
protected:
	
private : 
	Entity* m_ownEntity = nullptr;
	void SetOwnEntity(Entity* ownEntity);
};
#endif

