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
#include "../Game/SkydomeCycleComponent.h"
#include "../Game/RotatorComponent.h"

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


	Shader* cubeMapShader = AssetsManager::CreateShader("CubeMapShader", "Assets/CubeMapShader.vert", "Assets/CubeMapShader.frag");
	Shader* skyboxShader = AssetsManager::CreateShader("SkyboxShader", "Assets/SkyboxShader.vert", "Assets/SkyboxShader.frag");
	Shader* skydomeShader = AssetsManager::CreateShader("SkydomeShader", "Assets/SkydomeShader.vert", "Assets/SkydomeShader.frag");

	//Initialise Textures
	Texture* whiteTexture = AssetsManager::CreateTexture2D("White","Assets/WhiteTexture.png");	
	Texture* cubeMap = AssetsManager::CreateCubeMap("CubeMapTexture","Assets/CubeMap.jpg");	
	Texture* simpleTexture = AssetsManager::CreateTexture2D("SimpleTexture","Assets/Texture.png");
	
	//Initialise Mesh
	Mesh* cubeMesh = MeshUtilities::CreateCube("CubeMesh", 1.0f);
	Mesh* sphereMesh = MeshUtilities::CreateUVSphere("SphereMesh", 0.5f,32,32);

	//Initialise Materials
	Material* cubeMapMaterial = AssetsManager::CreateMaterial("CubeMapMaterial");
	cubeMapMaterial->SetTexture("cubemap", cubeMap);


	Material* skyDomeMaterial = AssetsManager::CreateMaterial("SkydomeMaterial");
	skyDomeMaterial->SetVector3D("lightDirection", Vector3D(0,-1,0));
	skyDomeMaterial->SetColor("colorA", Color(60 / 255.0f, 127 / 255.0f, 170 / 255.0f, 1.0f));
	skyDomeMaterial->SetColor("colorB", Color(112 / 255.0f, 188 / 255.0f, 220 / 255.0f, 1.0f));
	skyDomeMaterial->SetColor("sunColor", Color(255 / 255.0f, 223 / 255.0f, 79 / 255.0f, 1.0f));
	skyDomeMaterial->SetFloat("horizonOffset", 0.53f);
	skyDomeMaterial->SetFloat("exposure", 8);
	skyDomeMaterial->SetFloat("sunSize", 2.0f * Mathf::DegToRad);
	skyDomeMaterial->SetFloat("sunSmoothThreshold", 0.2f * Mathf::DegToRad);
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
		cameraComponent->SetWorldPosition(Vector3D(0, 1, -10));
	}

	//Initialise Directional Light entity
	DirectionalLightComponent* dLightComponent = nullptr;
	Entity* lightEntity = objLoaderScene.CreateEntity<Entity>();
	{
		dLightComponent = lightEntity->AddComponent<DirectionalLightComponent>(true);
		dLightComponent->m_ambiantColor = Color(0.1f, 0.1f, 0.1f, 1.0f);
		dLightComponent->m_diffuseColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
		dLightComponent->m_specularColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
		dLightComponent->m_intensity = 1.0f;

		dLightComponent->SetWorldRotation(Quaternion::FromEulerAngle(Vector3D(50, -30, 0)));

		DirectionalLightControllerComponent* dLightControllerComponent = lightEntity->AddComponent<DirectionalLightControllerComponent>(true);
		dLightControllerComponent->SetDirectionalLightComponent(dLightComponent);
	}
	
	
	
	
	
	Entity* cubeEntity = objLoaderScene.CreateEntity<Entity>(); //CubeMap
	{
		MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
		meshRendererComponent->SetWorldPosition(Vector3D(0, 0, 0));
		meshRendererComponent->SetWorldScale(Vector3D(1, 1, 1));
		meshRendererComponent->SetWorldRotation(Quaternion::FromEulerAngle(Vector3D(0,0,0)));
		meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
		meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("CubeMapShader"));
		meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("CubeMapMaterial"));
		meshRendererComponent->EnableCullFace(true);

		meshRendererComponent->SetGeometryRenderingPriority(0);
	}
	
	Entity* skyBoxEntity = objLoaderScene.CreateEntity<Entity>(); //SkyBox
	{
		MeshRendererComponent* meshRendererComponent = skyBoxEntity->AddComponent<MeshRendererComponent>(true);
		meshRendererComponent->SetWorldPosition(Vector3D(0, 0, 0));
		meshRendererComponent->SetWorldScale(Vector3D(1, 1, 1));
		meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
		meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("SkyboxShader"));
		meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("CubeMapMaterial"));
		meshRendererComponent->EnableDepthMask(false);
		meshRendererComponent->m_useViewMatrixWithoutTranslation = true;
		meshRendererComponent->SetDepthTestFunc(DepthTestFunc::Lequal);
		meshRendererComponent->EnableCullFace(false);
		meshRendererComponent->SetGeometryRenderingPriority(100);
		/*skyBoxEntity->SetRootComponent(meshRendererComponent);

		RotatorComponent* rotatorComponent = skyBoxEntity->AddComponent<RotatorComponent>(true);
		rotatorComponent->m_speed = 1.0f;*/
	}
	

	/*
	Entity* skyDome = objLoaderScene.CreateEntity<Entity>(); //SkyDome
	{
		MeshRendererComponent* meshRendererComponent = skyDome->AddComponent<MeshRendererComponent>(true);
		meshRendererComponent->SetWorldPosition(Vector3D(0, 0, 0));
		meshRendererComponent->SetWorldScale(Vector3D(1, 1, 1));
//		meshRendererComponent->SetWorldRotation(Quaternion::FromEulerAngle(Vector3D(90,0,0)));
		meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("SphereMesh"));
		meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("SkydomeShader"));
		meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("SkydomeMaterial"));

		meshRendererComponent->m_useViewMatrixWithoutTranslation = true;
		meshRendererComponent->SetDepthTestFunc(DepthTestFunc::Lequal);
		meshRendererComponent->EnableDepthMask(false);
		meshRendererComponent->EnableCullFace(false);
		meshRendererComponent->SetGeometryRenderingPriority(1000);

		SkydomeCycleComponent* skydomeCycleComponent = skyDome->AddComponent<SkydomeCycleComponent>();
		skydomeCycleComponent->skyDomeMaterial = AssetsManager::GetAsset<Material>("SkydomeMaterial");
		skydomeCycleComponent->directionalLightComponent = dLightComponent;
	}
	*/
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

	if (!m_isMeshRendererOrdored)
		OrdoredMeshRenderer();

	for (auto it = m_cameras.begin(); it != m_cameras.end(); ++it)
	{
		CameraComponent* camera = *it;
		
		int count = 0; 
		for (auto rendererIt = m_meshRenderersStoredByPriority.begin(); rendererIt != m_meshRenderersStoredByPriority.end(); ++rendererIt)
		{
			MeshRendererComponent* meshRenderer = *rendererIt;
			meshRenderer->Draw(camera, m_lights, window);
			count++;
		}
	}
}

void World::RefreshPriorityRenderingComponent(MeshRendererComponent* component)
{
	if (m_meshRenderers.find(component) != m_meshRenderers.end())
	{
		m_isMeshRendererOrdored = false;
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
	{
		m_meshRenderers.insert(m);
		m_meshRenderersStoredByPriority.push_back(m);
		m_isMeshRendererOrdored = false;
	}

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
	{
		m_meshRenderers.erase(m);

		auto it = std::find(m_meshRenderersStoredByPriority.begin(), m_meshRenderersStoredByPriority.end(), m);
		if (it != m_meshRenderersStoredByPriority.end()) {
			m_meshRenderersStoredByPriority.erase(it);
		}	
	}

	LightComponent* l = dynamic_cast<LightComponent*>(component);
	if (l)
		m_lights.erase(l);

	CameraComponent* c = dynamic_cast<CameraComponent*>(component);
	if (c)
		m_cameras.erase(c);
}

void World::OrdoredMeshRenderer()
{
	std::sort(m_meshRenderersStoredByPriority.begin(), m_meshRenderersStoredByPriority.end(),
		[](MeshRendererComponent* a, MeshRendererComponent* b) {
			return a->GetGeometryRenderingPriority() < b->GetGeometryRenderingPriority();
		});

	m_isMeshRendererOrdored = true;
}
