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
	Mesh* quadMesh = MeshUtilities::CreatePlane("QuadMesh", 4.8f);
	Mesh* sphereMesh = MeshUtilities::CreateUVSphere("SphereMesh", 1.0f,32.0f,32.0f);

	//Initialise Materials
	Material* material = AssetsManager::CreateBlinnPhongMaterial("Material0", whiteTexture, whiteTexture, whiteTexture, Color(1, 0, 0, 1), Color(1, 0, 0, 1), Color(1, 1, 1, 1),32);
	Material* material2 = AssetsManager::CreateBlinnPhongMaterial("Material1", whiteTexture, whiteTexture, whiteTexture, Color(0, 1, 0, 1), Color(0, 1, 0, 1), Color(1, 1, 0, 1), 32);
	Material* material3 = AssetsManager::CreateBlinnPhongMaterial("Material2", whiteTexture, whiteTexture, whiteTexture, Color(0, 0, 1, 1), Color(0, 0, 1, 1), Color(1, 1, 1, 1), 32);
	Material* material4 = AssetsManager::CreateBlinnPhongMaterial("Material3", whiteTexture, whiteTexture, whiteTexture, Color(0, 1, 1, 1), Color(0, 1, 1, 1), Color(1, 1, 1, 1), 32);
	Material* material5 = AssetsManager::CreateBlinnPhongMaterial("Material4", whiteTexture, whiteTexture, whiteTexture, Color(1, 0, 1, 1), Color(1, 0, 1, 1), Color(1, 1, 1, 1), 32);
	

	Material* material6 = AssetsManager::CreateBlinnPhongMaterial("Red", whiteTexture, whiteTexture, whiteTexture, Color(1, 0, 0, 1), Color(1, 0, 0, 1), Color(1, 1, 1, 1), 32);
	Material* material7 = AssetsManager::CreateBlinnPhongMaterial("Blue", whiteTexture, whiteTexture, whiteTexture, Color(0, 0, 1, 1), Color(0, 0, 1, 1), Color(1, 1, 1, 1), 32);
	Material* material8 = AssetsManager::CreateBlinnPhongMaterial("Cyan", whiteTexture, whiteTexture, whiteTexture, Color(0, 1, 1, 1), Color(0, 1, 1, 1), Color(1, 1, 1, 1), 32);
	Material* material9 = AssetsManager::CreateBlinnPhongMaterial("WhiteM", whiteTexture, whiteTexture, whiteTexture, Color(1, 1, 1, 1), Color(1, 1, 1, 1), Color(1, 1, 1, 1), 32);



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
		cameraComponent->SetWorldPosition(Vector3D(0, 1, -10));
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

	int exampleIndex = 3;

	if (exampleIndex == 0)
	{
		Entity* cubeEntity = objLoaderScene.CreateEntity<Entity>(); // Red Object
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(0, 0, 0));
			meshRendererComponent->SetWorldScale(Vector3D(10, 1, 1));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(0)));


			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(false);
			meshRendererComponent->SetGeometryRenderingPriority(0);
		}


		Entity* cubeEntity2 = objLoaderScene.CreateEntity<Entity>(); //Green Object
		{
			MeshRendererComponent* meshRendererComponent2 = cubeEntity2->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent2->SetWorldPosition(Vector3D(0, 0, -2));
			meshRendererComponent2->SetWorldScale(Vector3D(1.0f, 10.0f, 1.0f));
			meshRendererComponent2->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent2->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent2->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(1))); //Green Matarial

			meshRendererComponent2->EnableDepthTest(true);
			meshRendererComponent2->EnableStencilTest(false);
			meshRendererComponent2->SetGeometryRenderingPriority(0);
		}
	}

	//Simple Stencil test
	if (exampleIndex == 1)
	{
		Entity* cubeEntity = objLoaderScene.CreateEntity<Entity>(); // Red Object
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(0, 0, 0));
			meshRendererComponent->SetWorldScale(Vector3D(10, 1, 1));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(0)));


			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(true);

			meshRendererComponent->SetDepthPassFrontAction(StencilTestAction::Replace);
			meshRendererComponent->SetStencilTestFrontFunc(StencilTestFunc::Always);
			meshRendererComponent->SetStencilTestFrontRef(0xFF);
			meshRendererComponent->SetGeometryRenderingPriority(0);
		}


		Entity* cubeEntity2 = objLoaderScene.CreateEntity<Entity>(); //Green Object
		{
			MeshRendererComponent* meshRendererComponent2 = cubeEntity2->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent2->SetWorldPosition(Vector3D(0, 0, -2));
			meshRendererComponent2->SetWorldScale(Vector3D(1.0f, 10.0f, 1.0f));
			meshRendererComponent2->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent2->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent2->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(1))); //Green Matarial

			meshRendererComponent2->EnableDepthTest(true);
			meshRendererComponent2->EnableStencilTest(true);

			meshRendererComponent2->SetStencilTestFrontFunc(StencilTestFunc::Equal);
			meshRendererComponent2->SetStencilTestFrontRef(0xFF);
			meshRendererComponent2->SetGeometryRenderingPriority(1);
		}
	}

	//Outline Stencil Test	
	if (exampleIndex == 2)
	{
		Entity* cubeEntity = objLoaderScene.CreateEntity<Entity>(); //Red Object
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(0, 0, 0));
			meshRendererComponent->SetWorldScale(Vector3D(1, 1, 1));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(0)));


			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(true);

			meshRendererComponent->SetDepthPassFrontAction(StencilTestAction::Replace);
			meshRendererComponent->SetDepthFailFrontAction(StencilTestAction::Replace);

			meshRendererComponent->SetStencilTestFrontFunc(StencilTestFunc::Always);
			meshRendererComponent->SetStencilTestFrontRef(0xFA);
			meshRendererComponent->SetGeometryRenderingPriority(0);
		}

		Entity* hideObject = objLoaderScene.CreateEntity<Entity>(); //HideObject
		{
			MeshRendererComponent* meshRendererComponent = hideObject->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(0, 0, -2));
			meshRendererComponent->SetWorldScale(Vector3D(5, 5, 1));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(0)));


			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(false);
			meshRendererComponent->SetGeometryRenderingPriority(0);
		}


		Entity* cubeEntity2 = objLoaderScene.CreateEntity<Entity>(); //Green Object
		{
			MeshRendererComponent* meshRendererComponent2 = cubeEntity2->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent2->SetWorldPosition(Vector3D(0, 0, 0));
			meshRendererComponent2->SetWorldScale(Vector3D(1.1f, 1.1f, 1.1f));
			meshRendererComponent2->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent2->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent2->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(1)));

			meshRendererComponent2->EnableDepthTest(false);
			meshRendererComponent2->EnableStencilTest(true);

			meshRendererComponent2->SetStencilFailFrontAction(StencilTestAction::Keep);
			meshRendererComponent2->SetDepthFailFrontAction(StencilTestAction::Keep);
			meshRendererComponent2->SetDepthPassFrontAction(StencilTestAction::Keep);


			meshRendererComponent2->SetStencilTestFrontFunc(StencilTestFunc::Notequal);
			meshRendererComponent2->SetStencilTestFrontRef(0xFA);
			meshRendererComponent2->SetGeometryRenderingPriority(1);
		}
	}
	
	//Cube stencil mask	
	if (exampleIndex == 3)
	{
		//Green Up Face
		Entity* cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(0, -2.5 + 0.05f, 0));

			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("QuadMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(1)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(false);
			meshRendererComponent->SetGeometryRenderingPriority(-1);
			meshRendererComponent->EnableCullFace(false);
		}

		//Down Up Face
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(0, 2.5 - 0.05f, 0));
			meshRendererComponent->SetWorldRotation(Quaternion::FromEulerAngle(Vector3D(180, 0, 0)));

			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("QuadMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(1)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(false);
			meshRendererComponent->EnableCullFace(false);

			meshRendererComponent->SetGeometryRenderingPriority(-1);
		}

		//Left Mask
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(-2.5 + 0.05f, 0, 0));
			meshRendererComponent->SetWorldRotation(Quaternion::FromEulerAngle(Vector3D(0, 0, -90)));

			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("QuadMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(1)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(true);

			meshRendererComponent->SetStencilTestFrontFunc(StencilTestFunc::Never);
			meshRendererComponent->SetStencilFailFrontAction(StencilTestAction::Replace);
			meshRendererComponent->SetStencilTestFrontRef(0xFD);
			meshRendererComponent->SetStencilBackMask(0x00);
			meshRendererComponent->SetGeometryRenderingPriority(-1);
		}

		//Right Mask
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(2.5 - 0.05f, 0, 0));
			meshRendererComponent->SetWorldRotation(Quaternion::FromEulerAngle(Vector3D(0, 0, 90)));

			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("QuadMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(1)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(true);

			meshRendererComponent->SetStencilTestFrontFunc(StencilTestFunc::Never);
			meshRendererComponent->SetStencilFailFrontAction(StencilTestAction::Replace);
			meshRendererComponent->SetStencilTestFrontRef(0xFC);
			meshRendererComponent->SetStencilBackMask(0x00);
			meshRendererComponent->SetGeometryRenderingPriority(-1);
		}

		//Back Mask
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(0, 0, -2.5 + 0.05f));
			meshRendererComponent->SetWorldRotation(Quaternion::FromEulerAngle(Vector3D(90, 0, 0)));

			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("QuadMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(1)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(true);

			meshRendererComponent->SetStencilTestFrontFunc(StencilTestFunc::Never);
			meshRendererComponent->SetStencilFailFrontAction(StencilTestAction::Replace);
			meshRendererComponent->SetStencilTestFrontRef(0xFB);
			meshRendererComponent->SetStencilBackMask(0x00);
			meshRendererComponent->SetGeometryRenderingPriority(-1);

		}

		//Front Mask
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(0, 0, 2.5 - 0.05f));
			meshRendererComponent->SetWorldRotation(Quaternion::FromEulerAngle(Vector3D(-90, 0, 0)));

			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("QuadMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(1)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(true);

			meshRendererComponent->SetStencilTestFrontFunc(StencilTestFunc::Never);
			meshRendererComponent->SetStencilFailFrontAction(StencilTestAction::Replace);
			meshRendererComponent->SetStencilTestFrontRef(0xFA);
			meshRendererComponent->SetStencilBackMask(0x00);
			meshRendererComponent->SetGeometryRenderingPriority(-1);
		}

		//Left Object
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(0, 0, 0));
			meshRendererComponent->SetWorldScale(Vector3D(1, 1, 1));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Red"));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(true);

			meshRendererComponent->SetStencilTestFrontFunc(StencilTestFunc::Equal);

			meshRendererComponent->SetStencilTestFrontRef(0xFD);
			meshRendererComponent->SetGeometryRenderingPriority(0);
		}

		//Right Object
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(0, 0, 0));
			meshRendererComponent->SetWorldScale(Vector3D(1, 1, 1));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Blue"));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(true);

			meshRendererComponent->SetStencilTestFrontFunc(StencilTestFunc::Equal);
			meshRendererComponent->SetStencilTestFrontRef(0xFC);
			meshRendererComponent->SetGeometryRenderingPriority(0);
		}

		//Back Object
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(0, 0, 0));
			meshRendererComponent->SetWorldScale(Vector3D(1, 1, 1));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Cyan"));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(true);

			meshRendererComponent->SetStencilTestFrontFunc(StencilTestFunc::Equal);
			meshRendererComponent->SetStencilTestFrontRef(0xFB);
			meshRendererComponent->SetGeometryRenderingPriority(0);
		}

		//Front Object
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(0, 0, 0));
			meshRendererComponent->SetWorldScale(Vector3D(1, 1, 1));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("WhiteM"));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(true);

			meshRendererComponent->SetStencilTestFrontFunc(StencilTestFunc::Equal);
			meshRendererComponent->SetStencilTestFrontRef(0xFA);
			meshRendererComponent->SetGeometryRenderingPriority(0);
		}


		//Red Border
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(0, -2.5f + 0.05f, -2.5f + 0.05f));
			meshRendererComponent->SetWorldScale(Vector3D(5, 0.1, 0.1));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(0)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(false);
			meshRendererComponent->SetGeometryRenderingPriority(1);
		}

		//Red Border
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(0, -2.5f + 0.05f, 2.5f - 0.05f));
			meshRendererComponent->SetWorldScale(Vector3D(5, 0.1, 0.1));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(0)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(false);
			meshRendererComponent->SetGeometryRenderingPriority(2);
		}

		//Red Border
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(-2.5f + 0.05f, -2.5f + 0.05f, 0));
			meshRendererComponent->SetWorldScale(Vector3D(0.1, 0.1, 5));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(0)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(false);
			meshRendererComponent->SetGeometryRenderingPriority(2);
		}

		//Red Border
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(2.5f - 0.05f, -2.5f + 0.05f, 0));
			meshRendererComponent->SetWorldScale(Vector3D(0.1, 0.1, 5));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(0)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(false);
			meshRendererComponent->SetGeometryRenderingPriority(2);
		}

		//Red Border
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(0, 2.5f - 0.05f, -2.5f + 0.05f));
			meshRendererComponent->SetWorldScale(Vector3D(5, 0.1, 0.1));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(0)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(false);
			meshRendererComponent->SetGeometryRenderingPriority(2);
		}

		//Red Border
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(0, 2.5f - 0.05f, 2.5f - 0.05f));
			meshRendererComponent->SetWorldScale(Vector3D(5, 0.1, 0.1));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(0)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(false);
			meshRendererComponent->SetGeometryRenderingPriority(2);
		}

		//Red Border
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(-2.5f + 0.05f, 2.5f - 0.05f, 0));
			meshRendererComponent->SetWorldScale(Vector3D(0.1, 0.1, 5));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(0)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(false);
			meshRendererComponent->SetGeometryRenderingPriority(2);
		}

		//Red Border
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(2.5f - 0.05f, 2.5f - 0.05f, 0));
			meshRendererComponent->SetWorldScale(Vector3D(0.1, 0.1, 5));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(0)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(false);
			meshRendererComponent->SetGeometryRenderingPriority(2);
		}

		//Red Border
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(-2.5f + 0.05f, 0, -2.5f + 0.05f));
			meshRendererComponent->SetWorldScale(Vector3D(0.1, 5, 0.1));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(0)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(false);
			meshRendererComponent->SetGeometryRenderingPriority(2);
		}

		//Red Border
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(2.5f - 0.05f, 0, -2.5f + 0.05f));
			meshRendererComponent->SetWorldScale(Vector3D(0.1, 5, 0.1));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(0)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(false);
			meshRendererComponent->SetGeometryRenderingPriority(2);
		}

		//Red Border
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(2.5f - 0.05f, 0, 2.5f - 0.05f));
			meshRendererComponent->SetWorldScale(Vector3D(0.1, 5, 0.1));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(0)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(false);
			meshRendererComponent->SetGeometryRenderingPriority(2);
		}

		//Red Border
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(-2.5f + 0.05f, 0, 2.5f - 0.05f));
			meshRendererComponent->SetWorldScale(Vector3D(0.1, 5, 0.1));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(0)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(false);
			meshRendererComponent->SetGeometryRenderingPriority(2);
		}
	}
	
	//Cull Face Example
	if (exampleIndex == 4)
	{
		bool enableCullFace = true;
		CullFace cullFace = CullFace::Front;

		//Down Face
		Entity* cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(0, -2.5 + 0.05f, 0));
			meshRendererComponent->SetWorldRotation(Quaternion::FromEulerAngle(Vector3D(0, 0, 0)));

			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("QuadMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(1)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableCullFace(enableCullFace);
			meshRendererComponent->SetCullFace(cullFace);
		}

		//Left Face
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(-2.5 + 0.05f, 0, 0));
			meshRendererComponent->SetWorldRotation(Quaternion::FromEulerAngle(Vector3D(0, 0, -90)));

			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("QuadMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(1)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableCullFace(enableCullFace);
			meshRendererComponent->SetCullFace(cullFace);
		}

		//Right Face
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(2.5 - 0.05f, 0, 0));
			meshRendererComponent->SetWorldRotation(Quaternion::FromEulerAngle(Vector3D(0, 0, 90)));

			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("QuadMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(1)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableCullFace(enableCullFace);
			meshRendererComponent->SetCullFace(cullFace);
		}

		//Back Face
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(0, 0, -2.5 + 0.05f));
			meshRendererComponent->SetWorldRotation(Quaternion::FromEulerAngle(Vector3D(90, 0, 0)));

			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("QuadMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(1)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableCullFace(enableCullFace);
			meshRendererComponent->SetCullFace(cullFace);
		}

		//Front Face
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(0, 0, 2.5 - 0.05f));
			meshRendererComponent->SetWorldRotation(Quaternion::FromEulerAngle(Vector3D(-90, 0, 0)));

			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("QuadMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(1)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableCullFace(enableCullFace);
			meshRendererComponent->SetCullFace(cullFace);
		}


		//Border
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(0, -2.5f + 0.05f, -2.5f + 0.05f));
			meshRendererComponent->SetWorldScale(Vector3D(5, 0.1, 0.1));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(0)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(false);
		}

		//Border
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(0, -2.5f + 0.05f, 2.5f - 0.05f));
			meshRendererComponent->SetWorldScale(Vector3D(5, 0.1, 0.1));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(0)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(false);
		}

		//Border
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(-2.5f + 0.05f, -2.5f + 0.05f, 0));
			meshRendererComponent->SetWorldScale(Vector3D(0.1, 0.1, 5));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(0)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(false);
		}

		//Border
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(2.5f - 0.05f, -2.5f + 0.05f, 0));
			meshRendererComponent->SetWorldScale(Vector3D(0.1, 0.1, 5));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(0)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(false);
		}

		//Border
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(0, 2.5f - 0.05f, -2.5f + 0.05f));
			meshRendererComponent->SetWorldScale(Vector3D(5, 0.1, 0.1));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(0)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(false);
		}

		//Border
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(0, 2.5f - 0.05f, 2.5f - 0.05f));
			meshRendererComponent->SetWorldScale(Vector3D(5, 0.1, 0.1));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(0)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(false);
		}

		//Border
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(-2.5f + 0.05f, 2.5f - 0.05f, 0));
			meshRendererComponent->SetWorldScale(Vector3D(0.1, 0.1, 5));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(0)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(false);
		}

		//Border
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(2.5f - 0.05f, 2.5f - 0.05f, 0));
			meshRendererComponent->SetWorldScale(Vector3D(0.1, 0.1, 5));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(0)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(false);
		}

		//Border
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(-2.5f + 0.05f, 0, -2.5f + 0.05f));
			meshRendererComponent->SetWorldScale(Vector3D(0.1, 5, 0.1));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(0)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(false);
		}

		//Border
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(2.5f - 0.05f, 0, -2.5f + 0.05f));
			meshRendererComponent->SetWorldScale(Vector3D(0.1, 5, 0.1));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(0)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(false);
			meshRendererComponent->SetGeometryRenderingPriority(2);
		}

		//Border
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(2.5f - 0.05f, 0, 2.5f - 0.05f));
			meshRendererComponent->SetWorldScale(Vector3D(0.1, 5, 0.1));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(0)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(false);
			meshRendererComponent->SetGeometryRenderingPriority(2);
		}

		//Border
		cubeEntity = objLoaderScene.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetWorldPosition(Vector3D(-2.5f + 0.05f, 0, 2.5f - 0.05f));
			meshRendererComponent->SetWorldScale(Vector3D(0.1, 5, 0.1));
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("Material" + std::to_string(0)));
			meshRendererComponent->EnableDepthTest(true);
			meshRendererComponent->EnableStencilTest(false);
			meshRendererComponent->SetGeometryRenderingPriority(2);
		}

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
