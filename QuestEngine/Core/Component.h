#ifndef  _COMPONENT_H_
#define _COMPONENT_H_

class Entity;
class Component
{
	friend class Entity;
public:
	~Component();
	Component();
	virtual void Update() {};
	virtual void Start() {};

protected:
	
	Entity* GetOwnEntity();
private : 
	Entity* m_ownEntity = nullptr;
	void SetOwnEntity(Entity* ownEntity);
};
#endif

