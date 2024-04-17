#ifndef  _WORLD_H_
#define _WORLD_H_
#include "Components/Camera.h"
#include "Components/MeshRenderer.h"
#include "Components/Light.h"
#include "Window.h"
#include <unordered_map>

class Entity;
class World
{
	friend class Entity;
	friend class SceneManager;

private:
	std::set<Entity*> m_entities;
	std::set<LightComponent*> m_lights;
	std::set<MeshRendererComponent*> m_meshRenderers;
	std::set<Component*> m_components;
	std::set<CameraComponent*> m_cameras;

private:
	void DestroyWorldEntity();
	void RegisterComponent(Component* component);
	void UnRegisterComponent(Component* component);

protected:
	static World* m_world;
	World();
	~World();

public :
	float deltaTime = 0.0f;
	static World* Instance();

	World(World& other) = delete;

	template<class T>
	T* CreateEntity()
	{
		static_assert(std::is_base_of<Entity, T>::value, "T must be derived from Entity");
		Entity* entity = (Entity*)(new T());
		m_entities.insert(entity);
		return (T*)entity;
	}
	void InitAssets();
	void InitWorld();
	void Update();
	void Display(Window* window);
};

#include "Entity.h"

#endif

