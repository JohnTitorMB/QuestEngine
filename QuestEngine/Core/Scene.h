#ifndef _SCENE_H_
#define _SCENE_H_
#include "Entity.h"
#include <vector>
#include <unordered_map>
#include "Assets/EntityGroupAsset.h"
class SceneManager;
class Scene
{
	friend class SceneManager;
private:
	std::set<Entity*> m_entities;

public:
	template<class T>
	T* CreateEntity()
	{
		static_assert(std::is_base_of<Entity, T>::value, "T must be derived from Entity");
		Entity* entity = (Entity*)(new T());
		m_entities.insert(entity);
		return (T*)entity;
	}

	void RemoveEntity(Entity* entity);

	std::set<Entity*>CloneGroupEntityToScene(EntityGroupAsset* entityGroupAsset);
};

#endif // !_SCENE_H_
