#include "World.h"
World* World::m_world = nullptr;


#include "AssetsManager.h"
#include "../Utilities/MeshUtilities.h"
#include "Components/DirectionalLight.h"
#include "../Game/DirectionalLightEntity.h"
#include "../Game/DirectionalLightController.h"
#include "../Game/CameraController.h"
#include "Components/Transform.h"
#include "Components/MeshRenderer.h"
#include "Components/Camera.h"
#include "LightingSettings.h"
#include "../Game/SceneGraphTestComponent.h"
#include "SceneManager.h"
#include "../Game/SceneSwitchController.h"
#include "OBJLoader.h"
#include "../Game/SceneOBJLoaderComponnent.h"
#include "../Game/RotatorComponnent.h"

World::World()
{

}

World* World::Instance()
{
	if (m_world == nullptr)
	{
		m_world = new World();
	}

	return m_world;
}

World::~World()
{
	DestroyWorldEntity();

	if (m_world)
	{
		delete m_world;
		m_world = nullptr;
	}
}

void World::InitAssets()
{
	//Initialise Shader
	Shader* shader = AssetsManager::CreateShader("BlinnPhongShader", "Assets/BlinnPhongShader.vert", "Assets/BlinnPhongShader.frag");

	//Initialise Textures
	Texture* cubeTexture = AssetsManager::CreateTexture("CubeFaceTexture","Assets/CubeTextureFace.png");
	Texture* whiteTexture = AssetsManager::CreateTexture("White","Assets/WhiteTexture.png");
	Texture* cubeSpecularTexture = AssetsManager::CreateTexture("CubeSpecularTexture","Assets/CubeTextureSpecular.png");

	//Initialise Mesh
	Mesh* cubeMesh = MeshUtilities::CreateCustomCubeUV("CubeMesh", 1.0f, MeshUtilities::CubeUVInfo());
	Mesh* sphereMesh = MeshUtilities::CreateUVSphere("SphereMesh", 1.0f,32.0f,32.0f);

	//Initialise Materials
	Material* material = AssetsManager::CreateMaterial("Material0", cubeTexture, cubeTexture, cubeSpecularTexture, Color(1, 0, 0, 1), Color(1, 0, 0, 1), Color(1, 1, 1, 1),32);
	Material* material2 = AssetsManager::CreateMaterial("Material1", cubeTexture, cubeTexture, cubeSpecularTexture, Color(0, 1, 0, 1), Color(0, 1, 0, 1), Color(1, 1, 0, 1), 32);
	Material* material3 = AssetsManager::CreateMaterial("Material2", cubeTexture, cubeTexture, cubeSpecularTexture, Color(0, 0, 1, 1), Color(0, 0, 1, 1), Color(1, 1, 1, 1), 32);
	Material* material4 = AssetsManager::CreateMaterial("Material3", cubeTexture, cubeTexture, cubeSpecularTexture, Color(0, 1, 1, 1), Color(0, 1, 1, 1), Color(1, 1, 1, 1), 32);
	Material* material5 = AssetsManager::CreateMaterial("Material4", cubeTexture, cubeTexture, cubeSpecularTexture, Color(1, 0, 1, 1), Color(1, 0, 1, 1), Color(1, 1, 1, 1), 32);
	
	using std::chrono::high_resolution_clock;
	using std::chrono::duration_cast;
	using std::chrono::duration;
	using std::chrono::milliseconds;

	auto t1 = high_resolution_clock::now();

	EntityGroupAsset* obj = OBJLibrary::OBJLoader::LoadOBJ("OBJObject0", "Assets/OBJ/1.Cube/cube.obj");
	Entity* entity = obj->GetEntityAt(0);
	entity->AddComponent<RotatorComponnent>();
	
	EntityGroupAsset* obj1 = OBJLibrary::OBJLoader::LoadOBJ("OBJObject1", "Assets/OBJ/3.Cup/Cup.obj");
	entity = obj1->GetEntityAt(0);
	entity->AddComponent<RotatorComponnent>();

	EntityGroupAsset* obj2 = OBJLibrary::OBJLoader::LoadOBJ("OBJObject2", "Assets/OBJ/13.Conference/conference.obj", 0.001f);
	EntityGroupAsset* obj3 = OBJLibrary::OBJLoader::LoadOBJ("OBJObject3", "Assets/OBJ/16.Lost-empire/lost_empire.obj");
	EntityGroupAsset* obj4 = OBJLibrary::OBJLoader::LoadOBJ("OBJObject4", "Assets/OBJ/18.SportsCar/sportsCar.obj");

	entity = obj4->GetEntityAt(0);
	entity->AddComponent<RotatorComponnent>();

	EntityGroupAsset* obj5 = OBJLibrary::OBJLoader::LoadOBJ("OBJObject5", "Assets/OBJ/20.Breakfast_room/breakfast_room.obj"); //one material have a diffuse color equal to zero but have diffuse path.
	EntityGroupAsset* obj6 = OBJLibrary::OBJLoader::LoadOBJ("OBJObject6", "Assets/OBJ/21.Bmw/bmw.obj", 0.005f);
	entity = obj6->GetEntityAt(0);
	entity->AddComponent<RotatorComponnent>();


	EntityGroupAsset* obj7 = OBJLibrary::OBJLoader::LoadOBJ("OBJObject7", "Assets/OBJ/23.Fireplace_room/fireplace_room.obj");
	EntityGroupAsset* obj8 = OBJLibrary::OBJLoader::LoadOBJ("OBJObject8", "Assets/OBJ/24.White_oak/white_oak.obj",0.01f); // alpha of this obj is not supported
	EntityGroupAsset* obj9 = OBJLibrary::OBJLoader::LoadOBJ("OBJObject9", "Assets/OBJ/25.Vokselia_spawn/vokselia_spawn.obj");
	EntityGroupAsset* obj10 = OBJLibrary::OBJLoader::LoadOBJ("OBJObject10", "Assets/OBJ/26.Living_room/living_room.obj");
	EntityGroupAsset* obj11 = OBJLibrary::OBJLoader::LoadOBJ("OBJObject11", "Assets/OBJ/30.RoadBike/roadBike.obj");
	entity = obj11->GetEntityAt(0);
	entity->AddComponent<RotatorComponnent>();

	EntityGroupAsset* obj12 = OBJLibrary::OBJLoader::LoadOBJ("OBJObject12", "Assets/OBJ/31.salle_de_bain/salle_de_bain.obj");
	EntityGroupAsset* obj13 = OBJLibrary::OBJLoader::LoadOBJ("OBJObject13", "Assets/OBJ/35.Rungholt/rungholt.obj");	
	EntityGroupAsset* obj14 = OBJLibrary::OBJLoader::LoadOBJ("OBJObject14", "Assets/OBJ/39.Sponza/sponza.obj", 0.05f);
	EntityGroupAsset* obj15 = OBJLibrary::OBJLoader::LoadOBJ("OBJObject15", "Assets/OBJ/41.San_Miguel/san-miguel.obj");
	EntityGroupAsset* obj16 = OBJLibrary::OBJLoader::LoadOBJ("OBJObject16", "Assets/OBJ/42.Pure3D_Medieval/p3d_medieval_enterable_bld-13.obj",0.01f);
	EntityGroupAsset* obj17 = OBJLibrary::OBJLoader::LoadOBJ("OBJObject17", "Assets/OBJ/43.uploads_files_2558144_well/uploads_files_2558144_well.obj");
	EntityGroupAsset* obj18 = OBJLibrary::OBJLoader::LoadOBJ("OBJObject18", "Assets/OBJ/45.Cottage/Cottage.obj");

	auto t2 = high_resolution_clock::now();

	/* Getting number of milliseconds as an integer. */
	auto ms_int = duration_cast<milliseconds>(t2 - t1);

	/* Getting number of milliseconds as a double. */
	duration<double, std::milli> ms_double = t2 - t1;

//	std::cout << "ObjLoaderDuration : " << ms_int.count() << "ms\n";
//	std::cout << "ObjLoaderDuration : " << ms_double.count() << "ms\n";

	//Initialise Mesh
	Mesh* convexPolygonFan = MeshUtilities::CreateConvexPolygonFanMethod("ConvexPolygonFan", { Vector2D(0.5f, 0.866f),Vector2D(0.866f, 0.5f),Vector2D(1.0f, 0.0f),Vector2D(0.866f, -0.5f),
		Vector2D(0.5f, -0.866f), Vector2D(0.0f, -1.0f), Vector2D(-0.5f, -0.866f), Vector2D(-0.866f, -0.5f), Vector2D(-1.0f, 0.0f), Vector2D(-0.866f, 0.5f), Vector2D(-0.5f, 0.866f), Vector2D(0.0f, 1.0f) });

	Mesh* convexPolygonEar = MeshUtilities::CreatePolygonWithEarMethod("ConvexPolygonEar", { Vector2D(0.5f, 0.866f),Vector2D(0.866f, 0.5f),Vector2D(1.0f, 0.0f),Vector2D(0.866f, -0.5f),
		Vector2D(0.5f, -0.866f), Vector2D(0.0f, - 1.0f), Vector2D(-0.5f, - 0.866f), Vector2D(-0.866f, -0.5f), Vector2D(-1.0f, 0.0f), Vector2D(-0.866f, 0.5f), Vector2D(-0.5f, 0.866f), Vector2D(0.0f, 1.0f) });



}

void World::InitWorld()
{
	InitAssets();

	LightingSettings::m_globalAmbiantColor = Color(0.2f,0.2f,0.2f,1);
	Scene& objLoaderScene = SceneManager::Instance()->CreateScene();

	Entity* cameraEntity = objLoaderScene.CreateEntity<Entity>();
	{
		CameraComponent* cameraComponent = cameraEntity->AddComponent<CameraComponent>(true);
		cameraComponent->SetWorldPosition(Vector3D(0, 0, -5));
		cameraComponent->SetNear(0.01f);
		cameraComponent->SetProjectionMode(CameraComponent::EProjectionMode::PERSPECTIVE);
		cameraComponent->SetFov(60);
		cameraEntity->SetRootComponent(cameraComponent);
		CameraController* cameraController = cameraEntity->AddComponent<CameraController>(true);
	}

	//Initialise Directional Light entity
	DirectionalLightEntity* lightEntity = objLoaderScene.CreateEntity<DirectionalLightEntity>();
	{
		DirectionalLightComponent* dLightComponent = lightEntity->AddComponent<DirectionalLightComponent>(true);
		dLightComponent->m_ambiantColor = Color(0.1f, 0.1f, 0.1f, 1.0f);
		dLightComponent->m_diffuseColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
		dLightComponent->m_specularColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
		dLightComponent->m_intensity = 1.0f;

		DirectionalLightControllerComponent* dLightControllerComponent = lightEntity->AddComponent<DirectionalLightControllerComponent>(true);
		dLightControllerComponent->SetDirectionalLightComponent(dLightComponent);

		dLightComponent->SetParent(cameraEntity->GetRootComponent());
	}

	//Initialise Directional Light entity
	Entity* sceneObjEntity = objLoaderScene.CreateEntity<Entity>();
	{
		SceneOBJLoaderComponnent* sceneObjComponent = sceneObjEntity->AddComponent<SceneOBJLoaderComponnent>(true);
	}

	SceneManager::Instance()->LoadScene(0);
}

std::set<Entity*> World::InstantiateEntityGroup(EntityGroupAsset* entityGroupAsset)
{
	if (entityGroupAsset == nullptr)
		return std::set<Entity*>();

	std::set<Entity*> clonnedEntities = entityGroupAsset->CloneEntities();

	for (Entity* entity : clonnedEntities)
		m_entities.insert(entity);

	return clonnedEntities;
}

void World::DestroyEntity(Entity* entity)
{
	if (entity->isWorldEntity)
	{
		m_entities.erase(entity);
		if (entity)
			delete entity;
	}
	else
	{
		std::cout << "this entity cannot be destroyed from the world, it is not an entity born on the world!" << std::endl;
	}
	
}

void World::Update()
{
	for (auto it = m_entities.begin(); it != m_entities.end(); ++it)
	{
		Entity* entity = *it;

		entity->Update();
	}
}

void World::Display(Window* window)
{
	if (window == nullptr)
		std::cout << "Error" << std::endl;

	for (auto it = m_cameras.begin(); it != m_cameras.end(); ++it)
	{
		CameraComponent* camera = *it;
		
		int count = 0; 
		for (auto rendererIt = m_meshRenderers.begin(); rendererIt != m_meshRenderers.end(); ++rendererIt)
		{
			MeshRendererComponent* meshRenderer = *rendererIt;
			meshRenderer->Draw(camera, m_lights, window);
			count++;
		}
	}
}

void World::DestroyWorldEntity()
{
	for (auto it = m_entities.begin(); it != m_entities.end(); ++it)
	{
		Entity* entity = *it;
		delete entity;
	}

	m_entities.clear();
	m_components.clear();
	m_meshRenderers.clear();
	m_cameras.clear();
	m_lights.clear();
}

void World::RegisterComponent(Component* component)
{
	m_components.insert(component);

	MeshRendererComponent* m = dynamic_cast<MeshRendererComponent*>(component);
	if (m)
		m_meshRenderers.insert(m);

	LightComponent* l = dynamic_cast<LightComponent*>(component);
	if (l)
		m_lights.insert(l);

	CameraComponent* c = dynamic_cast<CameraComponent*>(component);
	if (c)
		m_cameras.insert(c);
}

void World::UnRegisterComponent(Component* component)
{
	m_components.erase(component);
	MeshRendererComponent* m = dynamic_cast<MeshRendererComponent*>(component);
	if (m)
		m_meshRenderers.erase(m);

	LightComponent* l = dynamic_cast<LightComponent*>(component);
	if (l)
		m_lights.erase(l);

	CameraComponent* c = dynamic_cast<CameraComponent*>(component);
	if (c)
		m_cameras.erase(c);
}