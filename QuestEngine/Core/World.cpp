#include "World.h"
World* World::m_world = nullptr;
RenderingType World::m_renderingType = RenderingType::Default;

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
#include "SceneManager.h"
#include "OBJLoader.h"
#include "Components/Transform.h"
#include "../Game/RenderingSwitchComponent.h"
#include "TimeManager.h"

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
	TimeManager::Instance()->Destroy();

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
	Shader* shader2 = AssetsManager::CreateShader("UVShader", "Assets/BlinnPhongShader.vert", "Assets/UVShader.frag");
	Shader* shader3 = AssetsManager::CreateShader("NormalShader", "Assets/BlinnPhongShader.vert", "Assets/NormalShader.frag");
	Shader* shader4 = AssetsManager::CreateShader("FragCoordZShader", "Assets/BlinnPhongShader.vert", "Assets/FragCoordZShader.frag");
	Shader* shader5 = AssetsManager::CreateShader("LinearDepthShader", "Assets/BlinnPhongShader.vert", "Assets/LinearDepthShader.frag");
	Shader* shader6 = AssetsManager::CreateShader("BlinnPhongShaderFog", "Assets/BlinnPhongShader.vert", "Assets/BlinnPhongShaderFog.frag");

	//Initialise Textures
	Texture* cubeTexture = AssetsManager::CreateTexture("CubeFaceTexture","Assets/CubeTextureFace.png");
	Texture* whiteTexture = AssetsManager::CreateTexture("White","Assets/WhiteTexture.png");
	Texture* cubeSpecularTexture = AssetsManager::CreateTexture("CubeSpecularTexture","Assets/CubeTextureSpecular.png");

	//Initialise Mesh
	Mesh* cubeMesh = MeshUtilities::CreateCustomCubeUV("CubeMesh", 1.0f, MeshUtilities::CubeUVInfo());
	Mesh* sphereMesh = MeshUtilities::CreateUVSphere("SphereMesh", 1.0f,32.0f,32.0f);

	//Initialise Materials
	Material* material = AssetsManager::CreateBlinnPhongMaterial("Material0", whiteTexture, whiteTexture, whiteTexture, Color(1, 0, 0, 1), Color(1, 0, 0, 1), Color(1, 1, 1, 1),32);
	Material* material2 = AssetsManager::CreateBlinnPhongMaterial("Material1", whiteTexture, whiteTexture, whiteTexture, Color(0, 1, 0, 1), Color(0, 1, 0, 1), Color(1, 1, 0, 1), 32);
	Material* material3 = AssetsManager::CreateBlinnPhongMaterial("Material2", whiteTexture, whiteTexture, whiteTexture, Color(0, 0, 1, 1), Color(0, 0, 1, 1), Color(1, 1, 1, 1), 32);
	Material* material4 = AssetsManager::CreateBlinnPhongMaterial("Material3", whiteTexture, whiteTexture, whiteTexture, Color(0, 1, 1, 1), Color(0, 1, 1, 1), Color(1, 1, 1, 1), 32);
	Material* material5 = AssetsManager::CreateBlinnPhongMaterial("Material4", whiteTexture, whiteTexture, whiteTexture, Color(1, 0, 1, 1), Color(1, 0, 1, 1), Color(1, 1, 1, 1), 32);
	

	//EntityGroupAsset* obj = OBJLibrary::OBJLoader::LoadOBJ("OBJObject0", "Assets/OBJ/35.Rungholt/rungholt.obj");
}

void World::InitWorld()
{
	InitAssets();

	LightingSettings::m_globalAmbiantColor = Color(0.2f,0.2f,0.2f,1);
	Scene& objLoaderScene = SceneManager::Instance()->CreateScene();
	CameraComponent* cameraComponent= nullptr;
	Entity* cameraEntity = objLoaderScene.CreateEntity<Entity>();
	{
		cameraComponent = cameraEntity->AddComponent<CameraComponent>(true);
		cameraComponent->SetNear(0.01f);
		cameraComponent->SetFar(1000.0);
		cameraComponent->SetProjectionMode(CameraComponent::EProjectionMode::PERSPECTIVE);
		cameraComponent->SetFov(60);
		cameraEntity->SetRootComponent(cameraComponent);

		CameraController* cameraController = cameraEntity->AddComponent<CameraController>(true);
		cameraComponent->SetWorldPosition(Vector3D(0, 4, -20));
	}

	//Initialise Directional Light entity
	Entity* lightEntity = objLoaderScene.CreateEntity<Entity>();
	{
		DirectionalLightComponent* dLightComponent = lightEntity->AddComponent<DirectionalLightComponent>(true);
		dLightComponent->m_ambiantColor = Color(0.1f, 0.1f, 0.1f, 1.0f);
		dLightComponent->m_diffuseColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
		dLightComponent->m_specularColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
		dLightComponent->m_intensity = 1.0f;

		dLightComponent->SetWorldRotation(Quaternion::FromEulerAngle(Vector3D(50, -30, 0)));

		DirectionalLightControllerComponent* dLightControllerComponent = lightEntity->AddComponent<DirectionalLightControllerComponent>(true);
		dLightControllerComponent->SetDirectionalLightComponent(dLightComponent);
	}

	
	Entity* cubeEntity = objLoaderScene.CreateEntity<Entity>();
	{
		MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
		meshRendererComponent->SetWorldPosition(Vector3D(0,0,0));
		meshRendererComponent->SetWorldScale(Vector3D(10,1, 10));
		meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
		meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
		meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(0)));
	}

	//Initialise Directional Light entity
	Entity* entity = objLoaderScene.CreateEntity<Entity>();
	{
		RenderingSwitchComponent* renderingSwitchComponent = entity->AddComponent<RenderingSwitchComponent>(true);
		renderingSwitchComponent->m_cameraComponent = cameraComponent;
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
	//Update Time
	TimeManager::Instance()->m_time = glfwGetTime();
	TimeManager::Instance()->m_deltaTime = TimeManager::Instance()->m_time - TimeManager::Instance()->m_previousTime;


	//Update Timer
	TimeManager::Instance()->UpdateTimers();

	//Update World Entities
	for (auto it = m_entities.begin(); it != m_entities.end(); ++it)
	{
		Entity* entity = *it;

		entity->Update();
	}

	TimeManager::Instance()->m_previousTime = TimeManager::Instance()->m_time;
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