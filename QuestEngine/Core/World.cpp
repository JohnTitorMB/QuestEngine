#include "World.h"
#include "AssetsManager.h"
#include "../Utilities/MeshUtilities.h"
#include "Components/DirectionalLight.h"
#include "../Game/DirectionalLightEntity.h"
#include "../Game/DirectionalLightController.h"
#include "Components/Transform.h"
#include "Components/MeshRenderer.h"
#include "Components/Camera.h"
#include "LightingSettings.h"

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

	//Intialise Camera Entity
	Vector3D cameraPosition = Vector3D(0.0f, 0.0f, -10.8f);
	Vector3D cameraAngles = Vector3D(0.0f, 0.0f, 0.0f);

	Entity* cameraEntity = CreateEntity<Entity>();
	{
		CameraComponent* cameraComponent = cameraEntity->AddComponent<CameraComponent>();
		cameraComponent->SetPosition(cameraPosition);
		cameraComponent->SetAngle(cameraAngles);
		cameraComponent->SetProjectionMode(CameraComponent::EProjectionMode::PERSPECTIVE);
		cameraComponent->SetFov(60);
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
		Entity* cubeEntity = CreateEntity<Entity>();
		{
			TransformComponent* transformComponent = cubeEntity->AddComponent<TransformComponent>();
			transformComponent->SetPosition(entitiesPosition[i]);
			transformComponent->SetRotation(entitiesRotation[i]);
			transformComponent->SetScale(entitiesScale[i]);

			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>();
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetTransformComponent(transformComponent);
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(i)));
		}
	}


	//Initialise Directional Light entity
	DirectionalLightEntity* lightEntity = CreateEntity<DirectionalLightEntity>();
	{
		DirectionalLightComponent* dLightComponent = lightEntity->AddComponent<DirectionalLightComponent>();
		dLightComponent->m_direction = Vector3D(0, 0, 1).Normalized();
		dLightComponent->m_ambiantColor = Color(0, 0, 0, 1.0f);
		dLightComponent->m_specularColor = Color(0.3f, 0.3f, 0.3f, 1.0f);
		dLightComponent->m_intensity = 3.0f;
		
//		lightEntity->SetDirectionalLightComponent(dLightComponent);

		DirectionalLightControllerComponent* dLightControllerComponent = lightEntity->AddComponent<DirectionalLightControllerComponent>();
		dLightControllerComponent->SetDirectionalLightComponent(dLightComponent);
	}	

	for (auto it = m_entities.begin(); it != m_entities.end(); ++it)
	{
		Entity* entity = *it;
		entity->Start();
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