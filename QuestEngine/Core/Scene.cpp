#include "Scene.h"



void Scene::RemoveEntity(Entity* entity)
{
	if (entity != nullptr)
	{
		m_entities.erase(entity);
		delete entity;
	}
}