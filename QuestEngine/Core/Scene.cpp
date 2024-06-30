#include "Scene.h"



void Scene::RemoveEntity(Entity* entity)
{
	if (entity != nullptr)
	{
		m_entities.erase(entity);
		delete entity;
	}
}

std::set<Entity*> Scene::CloneGroupEntityToScene(EntityGroupAsset* entityGroupAsset)
{
	if (entityGroupAsset == nullptr)
		return std::set<Entity*>();

	std::set<Entity*> clonnedEntities = entityGroupAsset->CloneEntities(true);

	for (Entity* entity : clonnedEntities)
		m_entities.insert(entity);

	return clonnedEntities;
}
