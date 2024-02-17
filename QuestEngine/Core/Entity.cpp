#include "Entity.h"
#include <algorithm>
#include <cassert>
#include <type_traits>
#include "World.h"
Entity::Entity()
{
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



void Entity::DestroyComponent(Component* component)
{
	if (component != nullptr)
	{
		m_components.erase(component);
		delete component;
	}
}
