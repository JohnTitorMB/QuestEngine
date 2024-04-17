#include "World.h"
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
World* World::m_world = nullptr;

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
	Texture* cubeSpecularTexture = AssetsManager::CreateTexture("CubeSpecularTexture","Assets/CubeTextureSpecular.png");

	//Initialise Mesh
	Mesh* cubeMesh = MeshUtilities::CreateCustomCubeUV("CubeMesh", 1.0f, MeshUtilities::CubeUVInfo());

	//Initialise Materials
	Material* material = AssetsManager::CreateMaterial("Material0", cubeTexture, cubeTexture, cubeSpecularTexture, Color(1, 0, 0, 1), Color(1, 0, 0, 1), Color(1, 1, 1, 1),32);
	Material* material2 = AssetsManager::CreateMaterial("Material1", cubeTexture, cubeTexture, cubeSpecularTexture, Color(0, 1, 0, 1), Color(0, 1, 0, 1), Color(1, 1, 0, 1), 32);
	Material* material3 = AssetsManager::CreateMaterial("Material2", cubeTexture, cubeTexture, cubeSpecularTexture, Color(0, 0, 1, 1), Color(0, 0, 1, 1), Color(1, 1, 1, 1), 32);
	Material* material4 = AssetsManager::CreateMaterial("Material3", cubeTexture, cubeTexture, cubeSpecularTexture, Color(0, 1, 1, 1), Color(0, 1, 1, 1), Color(1, 1, 1, 1), 32);
	Material* material5 = AssetsManager::CreateMaterial("Material4", cubeTexture, cubeTexture, cubeSpecularTexture, Color(1, 0, 1, 1), Color(1, 0, 1, 1), Color(1, 1, 1, 1), 32);
	
}

void World::InitWorld()
{
	InitAssets();

	Scene& cornellBoxScene = SceneManager::Instance()->CreateScene();

	//Intialise Camera Entity
	Vector3D cameraPosition = Vector3D(0.0f, 0.0f, -10.8f);
	Quaternion cameraRotation = Quaternion::Identity();
	Entity* cameraEntity = cornellBoxScene.CreateEntity<Entity>();
	{
		CameraComponent* cameraComponent = cameraEntity->AddComponent<CameraComponent>(true);
		cameraComponent->SetWorldPosition(cameraPosition);
		cameraComponent->SetWorldRotation(cameraRotation);
		cameraComponent->SetProjectionMode(CameraComponent::EProjectionMode::PERSPECTIVE);
		cameraComponent->SetFov(60);

		CameraController* cameraController = cameraEntity->AddComponent<CameraController>(true);
	}

	//Intialise Cornell Box Entities

	std::vector<Vector3D> entitiesPosition = std::vector<Vector3D>{ Vector3D(0.0f, -3.0f, 0.0f),
																	Vector3D(0.0f, 3.0f, 0.0f),
																	Vector3D(3.0f,  0.0f, 0.0f),
																	Vector3D(-3.0f,  0.0f, 0.0f),
																	Vector3D(0.0f, 0.0f, 3.0f)};

	std::vector<Vector3D> entitiesScale = std::vector<Vector3D>{ Vector3D(6.0f, 0.01f, 6.0f),
																Vector3D(6.0f, 0.01f, 6.0f),
																Vector3D(0.01f, 6.0f, 6.0f),
																Vector3D(0.01f, 6.0f, 6.0f),
																Vector3D(6.0f, 6.0f, 0.01f) };

	std::vector<Quaternion> entitiesRotation = std::vector<Quaternion>{ Quaternion::AxisAngle(Vector3D::Right,180),
																	 Quaternion::AxisAngle(Vector3D::Right,180),
																	 Quaternion::AxisAngle(Vector3D::Up, 180),
																	 Quaternion::AxisAngle(Vector3D::Up, 180),
																	 Quaternion::Identity() };
	
	LightingSettings::m_globalAmbiantColor = Color(0.5, 0.5, 0.5, 1.0f);

	for (int i = 0; i < 5; i++)
	{
		Entity* cubeEntity = cornellBoxScene.CreateEntity<Entity>();
		{			
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(entitiesPosition[i]);
			meshRendererComponent->SetWorldRotation(entitiesRotation[i]);
			meshRendererComponent->SetWorldScale(entitiesScale[i]);
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(i)));
		}
	}


	//Initialise Directional Light entity
	DirectionalLightEntity* lightEntity = cornellBoxScene.CreateEntity<DirectionalLightEntity>();
	{
		DirectionalLightComponent* dLightComponent = lightEntity->AddComponent<DirectionalLightComponent>(true);
		dLightComponent->m_ambiantColor = Color(0, 0, 0, 1.0f);
		dLightComponent->m_specularColor = Color(0.3f, 0.3f, 0.3f, 1.0f);
		dLightComponent->m_intensity = 3.0f;
		
		DirectionalLightControllerComponent* dLightControllerComponent = lightEntity->AddComponent<DirectionalLightControllerComponent>(true);
		dLightControllerComponent->SetDirectionalLightComponent(dLightComponent);
	}	

	Entity* sceneSwitchController = cornellBoxScene.CreateEntity<Entity>();
	{
		sceneSwitchController->AddComponent<SceneSwitchController>();
	}

	Scene& graphSceneText = SceneManager::Instance()->CreateScene();
	Entity* camera2Entity = graphSceneText.CreateEntity<Entity>();
	{
		CameraComponent* cameraComponent = camera2Entity->AddComponent<CameraComponent>(true);
		cameraComponent->SetWorldPosition(cameraPosition);
		cameraComponent->SetWorldRotation(cameraRotation);
		cameraComponent->SetProjectionMode(CameraComponent::EProjectionMode::PERSPECTIVE);
		cameraComponent->SetFov(60);

		CameraController* cameraController = camera2Entity->AddComponent<CameraController>(true);
	}

	Entity* cube2Entity = graphSceneText.CreateEntity<Entity>();
	MeshRendererComponent* meshRenderer2Component = cube2Entity->AddComponent<MeshRendererComponent>(true);
	meshRenderer2Component->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
	meshRenderer2Component->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
	meshRenderer2Component->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(0)));

	Entity* cube3Entity = graphSceneText.CreateEntity<Entity>();
	MeshRendererComponent* meshRenderer3Component = cube3Entity->AddComponent<MeshRendererComponent>(true);
	meshRenderer3Component->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
	meshRenderer3Component->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
	meshRenderer3Component->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(1)));

	Entity* cube4Entity = graphSceneText.CreateEntity<Entity>();
	MeshRendererComponent* childMeshComponent = cube4Entity->AddComponent<MeshRendererComponent>(true);
	childMeshComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
	childMeshComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
	childMeshComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(2)));

	Entity* cube5Entity = graphSceneText.CreateEntity<Entity>();
	MeshRendererComponent* child2MeshComponent = cube4Entity->AddComponent<MeshRendererComponent>(true);
	child2MeshComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
	child2MeshComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
	child2MeshComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(3)));

	SceneGraphTestComponent* sceneGraphTestComponent = cube2Entity->AddComponent<SceneGraphTestComponent>(true);
	sceneGraphTestComponent->SetSceneComponent(meshRenderer2Component);
	sceneGraphTestComponent->SetSceneComponent2(meshRenderer3Component);
	sceneGraphTestComponent->SetSceneComponentChild(childMeshComponent);
	sceneGraphTestComponent->SetSceneComponentChild2(child2MeshComponent);

	meshRenderer2Component->SetWorldPosition(Vector3D(0, 1, 0));
	childMeshComponent->SetParent(meshRenderer2Component);
	childMeshComponent->SetRelativePosition(Vector3D(4.0f, 0, 0));

	child2MeshComponent->SetParent(childMeshComponent);
	child2MeshComponent->SetRelativePosition(Vector3D(2.0f, 0, 0));

	meshRenderer3Component->SetWorldPosition(Vector3D(-2, 1, 0));

	Entity* sceneSwitchController2 = graphSceneText.CreateEntity<Entity>();
	{
		sceneSwitchController2->AddComponent<SceneSwitchController>();
	}

	SceneManager::Instance()->LoadScene(1);
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
	for (auto it = m_cameras.begin(); it != m_cameras.end(); ++it)
	{
		CameraComponent* camera = *it;

		for (auto rendererIt = m_meshRenderers.begin(); rendererIt != m_meshRenderers.end(); ++rendererIt)
		{
			MeshRendererComponent* meshRenderer = *rendererIt;

			meshRenderer->Draw(camera, m_lights, window);
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