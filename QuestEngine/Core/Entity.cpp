#include "Entity.h"
#include <algorithm>
#include <cassert>
#include <type_traits>
#include "World.h"
Entity::Entity()
{
	m_rootSceneComponent = nullptr;
}

Entity::Entity(const Entity& other)
{
	m_rootSceneComponent = nullptr;

	for (auto it = other.m_components.begin(); it != other.m_components.end(); ++it)
	{
		Component* component = *it;

		Component* newComponent = component->Clone();
		newComponent->SetOwnEntity(this);
		m_components.insert(newComponent);
		World::Instance()->RegisterComponent(newComponent);
	}
}

Entity::~Entity()
{
	for (auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		Component* component = *it;

		delete component;
	}

	m_components.clear();
}

void Entity::Destroy()
{

}

void Entity::Start()
{
	for (auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		Component* component = *it;

		component->Start();
	}
}

void Entity::Update()
{
	for (auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		Component* component = *it;
		component->Update();
	}
}

Entity* Entity::Clone()
{
	Entity* entity = new Entity(*this);
	clonnedObject = entity;
	clonnedObject->baseObject = this;
	return entity;
}

void Entity::AssignPointerAndReference()
{
	Entity* baseEntity = (Entity*)baseObject;
	if (baseEntity->m_rootSceneComponent != nullptr)
		m_rootSceneComponent = (SceneComponent*)baseEntity->m_rootSceneComponent->clonnedObject;

	for (auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		Component* component = *it;
		component->AssignPointerAndReference();
	}
}

void Entity::DestroyComponent(Component* component)
{
	if (component != nullptr)
	{
		World::Instance()->UnRegisterComponent(component);

		m_components.erase(component);
		delete component;
		component = nullptr;
	}
}


void Entity::SetRootComponent(SceneComponent* rootComponent)
{
	if (m_rootSceneComponent != rootComponent)
	{
		if (rootComponent->GetOwnEntity() == this)
		{
			m_rootSceneComponent = rootComponent;
		}
	}
}

SceneComponent* Entity::GetRootComponent() const
{
	return m_rootSceneComponent;
}
