#ifndef _SCENE_H_
#define _SCENE_H_
#include "Entity.h"
#include <vector>
#include <unordered_map>

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
};

#endif // !_SCENE_H_
