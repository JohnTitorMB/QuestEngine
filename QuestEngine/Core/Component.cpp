#include "Component.h"
#include "Entity.h"
#include <iostream>
Component::Component()
{
	m_ownEntity = nullptr;
}

Component::Component(const Component& other)
{
	m_ownEntity = other.m_ownEntity;
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

Component* Component::Clone()
{
	Component* component = new Component(*this);
	clonnedObject = component;
	clonnedObject->baseObject = this;
	return component;
}

void Component::AssignPointerAndReference()
{
	Component* baseComponent = (Component*)baseObject;
	if (baseComponent->m_ownEntity != nullptr)
		m_ownEntity = (Entity*)baseComponent->m_ownEntity->clonnedObject;
}