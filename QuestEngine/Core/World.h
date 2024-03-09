#ifndef  _WORLD_H_
#define _WORLD_H_
#include "Components/Camera.h"
#include "Components/MeshRenderer.h"
#include "Components/Light.h"
#include "Window.h"


class Entity;
class World
{
	std::set<Entity*> m_entities;
	std::set<LightComponent*> m_lights;
	std::set<MeshRendererComponent*> m_meshRenderers;
	std::set<Component*> m_components;

	void RegisterComponent(Component* component);
	friend class Entity;

protected:
	static World* m_world;
	World();
	~World();

public :
	std::set<CameraComponent*> m_cameras;
	float deltaTime = 0.0f;
	static World* Instance();

	World(World& other) = delete;

	template<class T>
	T* CreateEntity()
	{
		static_assert(std::is_base_of<Entity, T>::value, "T must be derived from Entity");
		T* entity = new T();
		m_entities.insert(entity);

		return entity;
	}
	void InitAssets();
	void InitWorld();
	void Update();
	void Display(Window* window);
};

#include "Entity.h"

#endif

