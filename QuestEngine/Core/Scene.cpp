#include "Scene.h"



Scene::~Scene()
{
	for (auto it = m_entities.begin(); it != m_entities.end(); ++it)
	{
		Entity* entity = *it;
		if (entity != nullptr)
			delete entity;
	}
}

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

std::set<Entity*> Scene::CloneGroupEntityToScene(EntityGroupAsset* entityGroupAsset, Entity*& firtEntity)
{
	if (entityGroupAsset == nullptr)
		return std::set<Entity*>();

	std::set<Entity*> clonnedEntities = entityGroupAsset->CloneEntities(firtEntity, true);

	for (Entity* entity : clonnedEntities)
		m_entities.insert(entity);

	return clonnedEntities;
}
