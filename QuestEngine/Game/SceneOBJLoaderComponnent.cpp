#include "SceneOBJLoaderComponnent.h"
#include "../Core/Inputs/InputSystem.h"
#include "../Core/World.h"
#include "../Core/AssetsManager.h"

SceneOBJLoaderComponnent::SceneOBJLoaderComponnent()
{
	m_inputMap = nullptr;

}

SceneOBJLoaderComponnent::SceneOBJLoaderComponnent(const SceneOBJLoaderComponnent& sceneOBJLoaderComponnent)
{
	m_inputMap = nullptr;
}

void SceneOBJLoaderComponnent::Start()
{
	InputSystem* inputSystem = InputSystem::Instance();
	m_inputMap = inputSystem->CreateInputMap();

	InputAction& inputAction = m_inputMap->CreateInputAction("PrevObj");
	inputAction.Started.AddListener(new EventCallback<SceneOBJLoaderComponnent, InputCallbackData>(this, &SceneOBJLoaderComponnent::PrevObj));
	m_inputMap->BindInputCode("PrevObj", InputCode::E_KEY_Q);

	InputAction& inputAction2 = m_inputMap->CreateInputAction("NextObj");
	inputAction2.Started.AddListener(new EventCallback<SceneOBJLoaderComponnent, InputCallbackData>(this, &SceneOBJLoaderComponnent::NextObj));
	m_inputMap->BindInputCode("NextObj", InputCode::E_KEY_E);

	World* world = World::Instance();
	objs = world->InstantiateEntityGroup(AssetsManager::GetAsset<EntityGroupAsset>("OBJObject" + std::to_string(currentOBJ)));	
}

void SceneOBJLoaderComponnent::Update()
{
}


void SceneOBJLoaderComponnent::NextObj(InputCallbackData data)
{
	if (currentOBJ < 18)
	{
		currentOBJ++;
		UnloadObjs();

		std::cout << "Load OBJObject " << currentOBJ << std::endl;
		World* world = World::Instance();
		objs = world->InstantiateEntityGroup(AssetsManager::GetAsset<EntityGroupAsset>("OBJObject" + std::to_string(currentOBJ)));
	}
}

void SceneOBJLoaderComponnent::PrevObj(InputCallbackData data)
{
	if (currentOBJ > 0)
	{
		currentOBJ--;
		UnloadObjs();

		std::cout << "Load OBJObject " << currentOBJ << std::endl;
		World* world = World::Instance();
		objs = world->InstantiateEntityGroup(AssetsManager::GetAsset<EntityGroupAsset>("OBJObject" + std::to_string(currentOBJ)));
	}
}

void SceneOBJLoaderComponnent::UnloadObjs()
{
	World* world = World::Instance();

	for (Entity* entity : objs)
	{
		world->DestroyEntity(entity);
	}
}

SceneOBJLoaderComponnent::~SceneOBJLoaderComponnent()
{
}

Component* SceneOBJLoaderComponnent::Clone()
{
	SceneOBJLoaderComponnent* sceneOBJLoaderComponnent = new SceneOBJLoaderComponnent(*this);
	clonnedObject = sceneOBJLoaderComponnent;
	clonnedObject->baseObject = this;
	return sceneOBJLoaderComponnent;
}

void SceneOBJLoaderComponnent::AssignPointerAndReference()
{
	Component::AssignPointerAndReference();
}
