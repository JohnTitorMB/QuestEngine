#include "EntityGroupAsset.h"

EntityGroupAsset::EntityGroupAsset()
{
	
}

EntityGroupAsset::~EntityGroupAsset()
{
	for (auto it = m_entities.begin(); it != m_entities.end(); ++it)
	{
		Entity* entity = *it;
		delete entity;
	}

	m_entities.clear();
}

std::set<Entity*> EntityGroupAsset::CloneEntities(bool isAddedOnScene)
{
	std::set<Entity*> clonnedEntities;

	for (auto it = m_entities.begin(); it != m_entities.end(); ++it)
	{
		Entity* entity = *it;
		Entity* newEntity = entity->Clone(isAddedOnScene);
		clonnedEntities.insert(newEntity);
	}

	for (auto it = clonnedEntities.begin(); it != clonnedEntities.end(); ++it)
	{
		Entity* entity = *it;
		entity->AssignPointerAndReference();
	}

	return clonnedEntities;
}

std::set<Entity*> EntityGroupAsset::CloneEntities(Entity*& firstEntity, bool isAddedOnScene)
{
	std::set<Entity*> clonnedEntities;

	int index = 0;
	for (auto it = m_entitiesVector.begin(); it != m_entitiesVector.end(); ++it)
	{
		Entity* entity = *it;
		Entity* newEntity = entity->Clone(isAddedOnScene);
		clonnedEntities.insert(newEntity);

		if (index == 0)
		{
			firstEntity = newEntity;
			index++;
		}
	}

	for (auto it = clonnedEntities.begin(); it != clonnedEntities.end(); ++it)
	{
		Entity* entity = *it;
		entity->AssignPointerAndReference();
	}

	return clonnedEntities;
}