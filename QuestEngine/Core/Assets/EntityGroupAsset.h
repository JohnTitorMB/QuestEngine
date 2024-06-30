#ifndef _ENTITY_ASSETS_H_
#define _ENTITY_ASSETS_H_
#include "Assets.h"
#include "../Entity.h"
class EntityGroupAsset : public Assets
{
	std::set<Entity*> m_entities;
	std::vector<Entity*> m_entitiesVector;
public:

	template<class T>
	T* CreateEntity()
	{
		static_assert(std::is_base_of<Entity, T>::value, "T must be derived from Entity");
		Entity* entity = (Entity*)(new T());
		m_entities.insert(entity);
		m_entitiesVector.push_back(entity);
		return (T*)entity;
	}

	EntityGroupAsset();
	~EntityGroupAsset();

	std::set<Entity*> CloneEntities(bool isAddedOnScene = false);
	Entity* GetEntityAt(int index)
	{
		if (index < 0 || index >= m_entitiesVector.size())
		{
			throw std::out_of_range("Index is out of range");
		}
		return m_entitiesVector[index];
	}
};

#endif

