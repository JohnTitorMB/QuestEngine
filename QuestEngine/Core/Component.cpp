#include "Component.h"
#include "Entity.h"
Component::Component()
{
	m_ownEntity = nullptr;
}

Component::~Component() 
{

};

void Component::SetOwnEntity(Entity* ownEntity)
{
	m_ownEntity = ownEntity;
}

Entity* Component::GetOwnEntity()
{
	return m_ownEntity;
}