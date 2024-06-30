#ifndef  _ENTITY_H_
#define _ENTITY_H_
#include <set>
#include <unordered_map>
#include <list>
#include "Object.h"
#include "Components/SceneComponent.h"
class World;
class Component;
class Entity : public Object
{
	std::set<Component*> m_components;
	std::unordered_map<int, Component*> m_componentsByID;
	SceneComponent* m_rootSceneComponent;
	friend class World;
	friend class Scene;
	std::string m_name;
	bool isWorldEntity = false;
public:
	
	Entity();
	Entity(const Entity& other, bool isAddedOnScene = false);
	virtual ~Entity();

	void Destroy();
	virtual void Start();
	virtual void Update();
	virtual Entity* Clone(bool isAddedOnScene = false);
	virtual void AssignPointerAndReference();

	template<class T>
	T* AddComponent(bool isAddedOnScene = false)
	{
		static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");

		T* t = new T();
		Component* c = dynamic_cast<Component*>(t);
		c->SetOwnEntity(this);
		m_components.insert(c);

		if(!isAddedOnScene)
			World::Instance()->RegisterComponent(c);
		return t;
	};

	template<class T>
	T* GetComponent() {
		static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");

		for (auto it = m_components.begin(); it != m_components.end(); ++it)
		{
			T* t = dynamic_cast<T*>(*it);
			if (t != nullptr)
				return t;
		}

		return nullptr;
	};

	template<class T>
	std::set<T*> GetComponents()
	{
		static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");
		std::set<T*> componentsOfTypeT;

		for (auto it = m_components.begin(); it != m_components.end(); ++it)
		{
			T* t = dynamic_cast<T*>(*it);
			if (t != nullptr)
				componentsOfTypeT.insert(t);
		}

		return componentsOfTypeT;
	};

	void DestroyComponent(Component* component);
	void SetRootComponent(SceneComponent* rootComponent);
	SceneComponent* GetRootComponent()const;
	const std::string GetName()const;
	void SetName(const std::string& name);
};

#include "Component.h"






#endif

