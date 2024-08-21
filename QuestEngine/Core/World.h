#ifndef  _WORLD_H_
#define _WORLD_H_
#include "Components/Camera.h"
#include "Components/MeshRenderer.h"
#include "Components/Light.h"
#include "Window.h"
#include <unordered_map>
#include "Assets/EntityGroupAsset.h"

enum class RenderingType
{
	Default,
	UV,
	Normal,
	FragCoordZ,
	LinearDepth,
	Fog
};

class Entity;
class QuestEngine;
class World
{
	friend class Entity;
	friend class SceneManager;
	friend class QuestEngine;

private:
	std::set<Entity*> m_entities;
	std::set<LightComponent*> m_lights;
	std::set<MeshRendererComponent*> m_meshRenderers;
	std::vector<MeshRendererComponent*> m_opaqueMeshRenderers;
	std::vector<MeshRendererComponent*> m_transparentMeshRenderers;
	std::set<Component*> m_components;
	std::set<CameraComponent*> m_cameras;

	bool m_isOpaqueMeshRendererOrdered = false;

private:
	void DestroyWorldEntity();
	void RegisterComponent(Component* component);
	void UnRegisterComponent(Component* component);
	void OrdoredOpaqueMeshRenderer();
	void OrdoredTransparenceMeshRenderer(CameraComponent* cameraComponent);
	 
protected:
	static World* m_world;
	World();
	~World();

public :

	static RenderingType m_renderingType;
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

	std::set<Entity*> InstantiateEntityGroup(EntityGroupAsset* entityGroupAsset);
	void DestroyEntity(Entity* entity);
	void InitAssets();
	void InitWorld();
	void Update();
	void Display(Window* window);
	void RefreshPriorityRenderingComponent(MeshRendererComponent* component);
	void RefreshBlendRenderingComponent(MeshRendererComponent* component, bool isBlendEnable);
};

#include "Entity.h"

#endif

