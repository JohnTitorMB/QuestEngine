#include "SceneManager.h"
#include "World.h"
SceneManager* SceneManager::m_instance = nullptr;

SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{
	for (int i = 0; i < m_scenes.size(); i++)
	{
		delete m_scenes[i];
	}

	m_scenes.clear();
}

void SceneManager::Destroy()
{
	delete m_instance;
	m_instance = nullptr;
}

Scene& SceneManager::CreateScene()
{
	Scene* scene = new Scene();
	m_scenes.push_back(scene);
	return *scene;
}

void SceneManager::LoadScene(int index)
{
	if (index < 0 || index >= m_scenes.size())
		return;

	World* world = World::Instance();
	
	world->DestroyWorldEntity();

	for (auto it = m_scenes[index]->m_entities.begin(); it != m_scenes[index]->m_entities.end(); ++it)
	{
		Entity* entity = *it;
		Entity* newEntity = entity->Clone();
		world->m_entities.insert(newEntity);
	}

	for (auto it = world->m_entities.begin(); it != world->m_entities.end(); ++it)
	{
		Entity* entity = *it;
		entity->AssignPointerAndReference();
	}

	for (auto it = world->m_entities.begin(); it != world->m_entities.end(); ++it)
	{
		Entity* entity = *it;
		entity->Start();
	}
}

SceneManager* SceneManager::Instance()
{
	if (m_instance == nullptr)
	{
		m_instance = new SceneManager();
	}
	
	return m_instance;
}
