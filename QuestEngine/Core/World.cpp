#include "World.h"
World* World::m_world = nullptr;
RenderingType World::m_renderingType = RenderingType::Default;

#include "AssetsManager.h"
#include "../Utilities/MeshUtilities.h"
#include "Components/DirectionalLight.h"
#include "Components/SpotLight.h"
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
#include "Assets/RenderTexture2D.h"
#include "../Game/AntiAliasingTestComponent.h"
#include "Graphics.h"
#include "Components/PostProcessing.h"
#include "PostProcessing/TintEffect.h"
#include "PostProcessing/GaussianBlur.h"
#include "PostProcessing/ColorGrading.h"

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

void World::Destroy()
{
	delete m_world;
	m_world = nullptr;
}

World::~World()
{
	DestroyWorldEntity();
}

void World::InitAssets()
{
	//Initialise Shader
	Shader* shader = AssetsManager::CreateShader("BlinnPhongShader", "Assets/BlinnPhongShader.vert", "Assets/BlinnPhongShader.frag");

	//Double apha shader
	Shader* shader2 = AssetsManager::CreateShader("BlinnPhongShaderAlphaPass1", "Assets/BlinnPhongShaderAlpha.vert", "Assets/BlinnPhongShaderAlphaPass1.frag");
	Shader* shader3 = AssetsManager::CreateShader("BlinnPhongShaderAlphaPass2", "Assets/BlinnPhongShaderAlpha.vert", "Assets/BlinnPhongShaderAlphaPass2.frag");
	
	//Sky Shader
	Shader* skyboxShader = AssetsManager::CreateShader("SkyboxShader", "Assets/SkyboxShader.vert", "Assets/SkyboxShader.frag");

	//Shader for display the renderTexture on the screen
	Shader* renderShader = AssetsManager::CreateShader("RenderShader", "Assets/BlinnPhongShader.vert", "Assets/RenderShader.frag");


	//PostProcessShader
	Shader* tintShader = AssetsManager::CreateShader("TintShader", "Assets/ScreenShader.vert", "Assets/TintShader.frag");
	Shader* gaussianBlurShader = AssetsManager::CreateShader("GaussianBlurShader", "Assets/ScreenShader.vert", "Assets/GaussianBlurShader.frag");
	Shader* colorGradingShader = AssetsManager::CreateShader("ColorGradingShader", "Assets/ScreenShader.vert", "Assets/ColorGradingShader.frag");
	Shader* lut2DShader = AssetsManager::CreateShader("LUT2DShader", "Assets/ScreenShader.vert", "Assets/LUT2DShader.frag");

	//Initialise Textures
	Texture* whiteTexture = AssetsManager::CreateTexture2D("White","Assets/WhiteTexture.png");	
	Texture* skyboxTexture = AssetsManager::CreateCubeMap("SkyboxTexture", "Assets/Skybox.png");
	Texture* tileTexture = AssetsManager::CreateTexture2D("TileTexture", "Assets/Texture.png");
	Texture* boxDiffuseTexture = AssetsManager::CreateTexture2D("BoxDiffuseTexture","Assets/Box/BoxDiffuse.png");
	Texture* boxAlphaTexture = AssetsManager::CreateTexture2D("BoxAlphaTexture","Assets/Box/BoxAlpha.png");
	Texture* gradiant = AssetsManager::CreateTexture2D("Gradiant","Assets/Gradiant.png");
	Texture2D* lut = AssetsManager::CreateTexture2D("lutYellow", "Assets/lutYellow.png");
	Texture2D* lutCustom = AssetsManager::CreateTexture2D("LUTCustom", "Assets/LutCustom.png");
	Texture2D* NightBlade = AssetsManager::CreateTexture2D("NightBlade", "Assets/NightBlade.png");

	
	lut->SetTextureInternalFormat(InternalFormat::RGBA8);
	lut->SetMinification(MinificationFilter::Point);
	lut->SetMagnification(MagnificationFilter::Point);
	lut->SetWrapHorizontalParameter(Wrap::ClampToEdge);
	lut->SetWrapVerticalParameter(Wrap::ClampToEdge);
	lut->SetAnisotropy(1);

	//Render Texture
	RenderTexture2D* renderTexture = AssetsManager::CreateRenderTexture2D("RenderTexture", 1920, 1080);
	renderTexture->AttachColorTextureBuffer(ColorRenderableFormat::RGBA8, ColorFormat::RGBA, DataType::UNSIGNED_BYTE,0);
	renderTexture->AttachDepthStencilRenderBuffer(DepthStencilRenderableFormat::DEPTH24_STENCIL8);
	renderTexture->SetTextureColorManagementParam(ColorManagement::RGBColorSpaceType::SRGB, Texture::TextureConversionMode::GPUConvert, 0);
	///Initialise Materials

	//White Material
	Material* whiteMaterial = AssetsManager::CreateBlinnPhongMaterial("WhiteMaterial", whiteTexture, whiteTexture, whiteTexture, ColorRGB(1,1,1,1), ColorRGB(1, 1, 1, 1), ColorRGB(1, 1, 1, 1),32.0f);

	// CubeMap Material
	Material* skyboxMaterial = AssetsManager::CreateMaterial("SkyboxMaterial");
	skyboxMaterial->SetTexture("cubemap", skyboxTexture);

	//Materaial for display the renderTexture on the screen
	Material* renderMaterial = AssetsManager::CreateMaterial("RenderMaterial");
	renderMaterial->SetColor("material.color", ColorRGB(1, 1, 1, 1));
	renderMaterial->SetTexture("material.texture", renderTexture, 0);
	renderMaterial->SetVector4D("material.textureST", Vector4D(0, 0, -1, 1));

	//Initialise Mesh
	Mesh* cubeMesh = MeshUtilities::CreateCube("CubeMesh", 1.0f);
	Mesh* sphereMesh = MeshUtilities::CreateUVSphere("SphereMesh", 0.5f, 32, 32);
	Mesh* quadMesh = MeshUtilities::CreatePlane("QuadMesh", 1.0f);
	Mesh* quadScreenMesh = MeshUtilities::CreateQuad("QuadScreenMesh", 2.0f);

	//Initialise OBJ
	OBJLibrary::OBJLoader::LoadOBJ("Stair", "Assets/Stair/stair.obj");
	OBJLibrary::OBJLoader::LoadOBJ("Table", "Assets/Table/mesa v27.obj");
	OBJLibrary::OBJLoader::LoadOBJ("Bowl", "Assets/Bowl/Glass bowl.obj");
	OBJLibrary::OBJLoader::LoadOBJ("Camera", "Assets/Camera/camera.obj");
	OBJLibrary::OBJLoader::LoadOBJ("Wall", "Assets/Wall/wall.obj");

	EntityGroupAsset* entityGroupAsset = OBJLibrary::OBJLoader::LoadOBJ("Desk", "Assets/Desk/desk.obj");
	Entity* screenEntity = entityGroupAsset->GetEntityAt(4); // Get the screen entity of desk
	MeshRendererComponent* meshRenderComponent = screenEntity->GetComponent<MeshRendererComponent>();
	meshRenderComponent->SetMaterial(renderMaterial);
	meshRenderComponent->SetShader(renderShader);	


	//Initialise Entity for display Post-Process effect;
	EntityGroupAsset* postProcessEG = AssetsManager::CreateEntityGroup("PostProcessEntityGroup");
	Entity* entity = postProcessEG->CreateEntity<Entity>();
	MeshRendererComponent* meshRendererComponent = entity->AddComponent<MeshRendererComponent>(true);
	meshRendererComponent->SetMesh(quadScreenMesh);
	meshRendererComponent->EnableCullFace(false);
	meshRendererComponent->EnableDepthTest(false);
	meshRendererComponent->EnableStencilTest(false);
	meshRendererComponent->EnableBlend(false);
}

void World::InitWorld()
{
	//glEnable(GL_FRAMEBUFFER_SRGB);

	Graphics::GetInstance()->SetAntiAliasingType(Graphics::AntiAliasingType::MSAA);
	Graphics::GetInstance()->SetMSAASample(16);

	InitAssets();

	LightingSettings::m_globalAmbiantColor = ColorRGB(0.1f, 0.1f, 0.1f,1);
	Scene& scene1 = SceneManager::Instance()->CreateScene();
	Entity* cameraEntity = scene1.CreateEntity<Entity>();
	{
		CameraComponent* cameraComponent = cameraEntity->AddComponent<CameraComponent>(true);	
		cameraEntity->SetRootComponent(cameraComponent);
		cameraComponent->SetNear(0.01f);
		cameraComponent->SetFar(1000.0);
		cameraComponent->SetProjectionMode(CameraComponent::EProjectionMode::PERSPECTIVE);
		cameraComponent->SetFov(60);
		cameraComponent->SetWorldPosition(Vector3D(-0.33, 1, -6.6));
		cameraComponent->SetRenderingPriority(1);
		cameraComponent->m_enableMultiSampling = true;
		CameraController* cameraController = cameraEntity->AddComponent<CameraController>(true);
		cameraController->m_scrollMove = 10;
		cameraController->movementSpeed = 0.5f;
		PostProcessing* postProcessing = cameraEntity->AddComponent<PostProcessing>(true);
	
	}

	Entity* postProcessVolumeEntity = scene1.CreateEntity<Entity>();
	{
		PostProcessingVolume* postProcessVolume = postProcessVolumeEntity->AddComponent<PostProcessingVolume>();
		std::shared_ptr<ColorGradingSettings> colorGradingSetting = std::make_shared<ColorGradingSettings>();
		postProcessVolume->AddEffect(colorGradingSetting);
				
		postProcessVolume->SetGlobal(true);

		PostProcessing* postProcessing = cameraEntity->GetComponent<PostProcessing>();
		postProcessing->AddPostProcessingVolume(postProcessVolume);

		MeshRendererComponent* meshRendererComponent = postProcessVolumeEntity->AddComponent<MeshRendererComponent>(true);
		meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
		meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
		meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("WhiteMaterial"));

	}




	CameraComponent* cameraComponent2 = nullptr;
	Entity* cameraEntity2 = scene1.CreateEntity<Entity>();
	{
		cameraComponent2 = cameraEntity2->AddComponent<CameraComponent>(true);
		cameraComponent2->SetNear(0.01f);
		cameraComponent2->SetFar(1000.0);
		cameraComponent2->SetProjectionMode(CameraComponent::EProjectionMode::PERSPECTIVE);
		cameraComponent2->SetFov(60);
		cameraEntity2->SetRootComponent(cameraComponent2);
		cameraComponent2->SetWorldPosition(Vector3D(0, 1, -4.75));
		cameraComponent2->SetRenderTexture(AssetsManager::GetAsset<RenderTexture2D>("RenderTexture"));
	}
	
	//Initialise Directional Light entity	
	DirectionalLightComponent* dLightComponent = nullptr;
	Entity* lightEntity = scene1.CreateEntity<Entity>();
	{
		dLightComponent = lightEntity->AddComponent<DirectionalLightComponent>(true);
		dLightComponent->m_ambiantColor = ColorRGB(0.1f, 0.1f, 0.1f, 1.0f);
		dLightComponent->m_diffuseColor = ColorRGB(1.0f, 1.0f, 1.0f, 1.0f);
		dLightComponent->m_specularColor = ColorRGB(1.0f, 1.0f, 1.0f, 0.0f);
		dLightComponent->m_intensity = 1.0f;
		 
		dLightComponent->SetWorldRotation(Quaternion::FromEulerAngle(Vector3D(50.0f, 0.0f, 0)));
		DirectionalLightControllerComponent* dLightControllerComponent = lightEntity->AddComponent<DirectionalLightControllerComponent>(true);
		dLightControllerComponent->SetDirectionalLightComponent(dLightComponent);
	}
	
	Entity* skyboxEntity = scene1.CreateEntity<Entity>();
	{
		MeshRendererComponent* meshRendererComponent = skyboxEntity->AddComponent<MeshRendererComponent>(true);
		meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
		meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("SkyboxShader"));
		meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("SkyboxMaterial"));
		meshRendererComponent->EnableCullFace(false);
		meshRendererComponent->m_useViewMatrixWithoutTranslation = true;
		meshRendererComponent->SetDepthTestFunc(DepthTestFunc::Lequal);
		meshRendererComponent->SetGeometryRenderingPriority(10000);
	}

	EntityGroupAsset* entityAssets = AssetsManager::GetAsset<EntityGroupAsset>("Stair");
	scene1.CloneGroupEntityToScene(entityAssets);
	
	Entity* firstEntity = nullptr;
	scene1.CloneGroupEntityToScene(AssetsManager::GetAsset<EntityGroupAsset>("Table"), firstEntity);

	if (firstEntity)
	{
		SceneComponent* sceneComponent = firstEntity->GetComponent<SceneComponent>();
		if (sceneComponent)
		{
			sceneComponent->SetWorldPosition(Vector3D(0, 0, -3.5f));
			sceneComponent->SetWorldScale(Vector3D(0.15f, 0.15f, 0.15f));
		}
	}

	scene1.CloneGroupEntityToScene(AssetsManager::GetAsset<EntityGroupAsset>("Bowl"), firstEntity);

	if (firstEntity)
	{
		SceneComponent* sceneComponent = firstEntity->GetComponent<SceneComponent>();
		if (sceneComponent)
		{
			sceneComponent->SetWorldPosition(Vector3D(-2.5, 0.89f, -3.0f));
			sceneComponent->SetWorldScale(Vector3D(1.3f, 1.3f, 1.3f));
		}
	}

	scene1.CloneGroupEntityToScene(AssetsManager::GetAsset<EntityGroupAsset>("Desk"), firstEntity);

	if (firstEntity)
	{
		SceneComponent* sceneComponent = firstEntity->GetComponent<SceneComponent>();
		if (sceneComponent)
		{
			sceneComponent->SetWorldPosition(Vector3D(0.0f, 0.0f, -2.0f));
			sceneComponent->SetWorldRotation(Quaternion::FromEulerAngle(Vector3D(0,270,0)));
			sceneComponent->SetWorldScale(Vector3D(1.0f, 1.0f, 1.0f));
		}
	}




	/*scene1.CloneGroupEntityToScene(AssetsManager::GetAsset<EntityGroupAsset>("Camera"), firstEntity);

	if (firstEntity)
	{
		SceneComponent* sceneComponent = firstEntity->GetComponent<SceneComponent>();
		if (sceneComponent)
		{
			sceneComponent->SetWorldPosition(Vector3D(0.0f, 1.0f, -4.9f));
			sceneComponent->SetWorldScale(Vector3D(0.001f, 0.001f, 0.001f));
		}
	}*/

	
	scene1.CloneGroupEntityToScene(AssetsManager::GetAsset<EntityGroupAsset>("Wall"), firstEntity);

	if (firstEntity)
	{
		SceneComponent* sceneComponent = firstEntity->GetComponent<SceneComponent>();
		if (sceneComponent)
		{
			sceneComponent->SetWorldPosition(Vector3D(-8.0f, 0.0f, -5.2f));
			sceneComponent->SetWorldRotation(Quaternion::FromEulerAngle(Vector3D(0, 270, 0)));
			sceneComponent->SetWorldScale(Vector3D(1.0f, 1.0f, 1.0f));
		}
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
	{
		std::cout << "Error" << std::endl;
		return;
	}

	if (!m_isOpaqueMeshRendererOrdered)
		OrdoredOpaqueMeshRenderer();

	std::vector<CameraComponent*> sortCamera(m_cameras.begin(), m_cameras.end());
	std::sort(sortCamera.begin(), sortCamera.end(), [](const CameraComponent* a, const CameraComponent* b) {
		return a->GetRenderingPriority() < b->GetRenderingPriority();
		});

	for (auto it = sortCamera.begin(); it != sortCamera.end(); ++it)
	{
		CameraComponent* camera = *it;

		RenderTexture2D* cameraRT = camera->GetRenderTexture();
		
		int viewportWidth = window->GetWidth();
		int viewportHeight = window->GetHeight();
		if (cameraRT)
		{
			cameraRT->BindFramebuffer();
			viewportWidth = cameraRT->GetWidth();
			viewportHeight = cameraRT->GetHeight();
		}
		else
			Graphics::GetInstance()->BindMainFrameBuffer();
			
		float bCornerX = camera->m_viewportBottomCornerX * viewportWidth;
		float bCornerY = camera->m_viewportBottomCornerY * viewportHeight;

		float tCornerX = camera->m_viewportTopCornerX * viewportWidth;
		float tCornerY = camera->m_viewportTopCornerY * viewportHeight;

		glViewport(bCornerX, bCornerY, tCornerX - bCornerX, tCornerY - bCornerY);

		Graphics::GetInstance()->Clear(camera, bCornerX, bCornerY, tCornerX - bCornerX, tCornerY - bCornerY);
		if (camera->m_enableMultiSampling)
			glEnable(GL_MULTISAMPLE);
		else
			glDisable(GL_MULTISAMPLE);
		

		for (auto rendererIt = m_opaqueMeshRenderers.begin(); rendererIt != m_opaqueMeshRenderers.end(); ++rendererIt)
		{
			MeshRendererComponent* meshRenderer = *rendererIt;
			meshRenderer->Draw(camera, m_lights, window, cameraRT);
		}
		
		OrdoredTransparenceMeshRenderer(camera);
		for (auto rendererIt = m_transparentMeshRenderers.begin(); rendererIt != m_transparentMeshRenderers.end(); ++rendererIt)
		{
			MeshRendererComponent* meshRenderer = *rendererIt;
			meshRenderer->Draw(camera, m_lights, window, cameraRT);
		}
		

		PostProcessing* postProcessing = camera->GetOwnEntity()->GetComponent<PostProcessing>();
		if (postProcessing)
			postProcessing->DisplayEffects(window, cameraRT, camera);


		if (cameraRT)
		{
			cameraRT->SwapBuffer();
			cameraRT->GenerateAllMipmap(false);
			cameraRT->UnBindFramebuffer();

			glViewport(0, 0, window->GetWidth(), window->GetHeight());
		}
	}
}

void World::RefreshPriorityRenderingComponent(MeshRendererComponent* component)
{
	if (m_meshRenderers.find(component) != m_meshRenderers.end())
	{
		if (!component->IsBlendEnabled())
			m_isOpaqueMeshRendererOrdered = false;
	}
}

void World::RefreshBlendRenderingComponent(MeshRendererComponent* component, bool isBlendEnable)
{
	if (m_meshRenderers.find(component) != m_meshRenderers.end())
	{
		if (isBlendEnable)
		{
			auto it = std::find(m_opaqueMeshRenderers.begin(), m_opaqueMeshRenderers.end(), component);
			if (it != m_opaqueMeshRenderers.end()) {
				m_opaqueMeshRenderers.erase(it);
			}

			m_transparentMeshRenderers.push_back(component);
		}
		else
		{
			auto it = std::find(m_transparentMeshRenderers.begin(), m_transparentMeshRenderers.end(), component);
			if (it != m_transparentMeshRenderers.end()) {
				m_transparentMeshRenderers.erase(it);
			}
			m_opaqueMeshRenderers.push_back(component);

			m_isOpaqueMeshRendererOrdered = false;
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
	{
		m_meshRenderers.insert(m);
		if (m->IsBlendEnabled())
		{
			m_transparentMeshRenderers.push_back(m);
		}
		else
		{
			m_opaqueMeshRenderers.push_back(m);
			m_isOpaqueMeshRendererOrdered = false;
		}
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

		if (m->IsBlendEnabled())
		{
			auto it = std::find(m_transparentMeshRenderers.begin(), m_transparentMeshRenderers.end(), m);
			if (it != m_transparentMeshRenderers.end()) {
				m_transparentMeshRenderers.erase(it);
			}
		}
		else
		{
			auto it = std::find(m_opaqueMeshRenderers.begin(), m_opaqueMeshRenderers.end(), m);
			if (it != m_opaqueMeshRenderers.end()) {
				m_opaqueMeshRenderers.erase(it);
			}
		}		
	}

	LightComponent* l = dynamic_cast<LightComponent*>(component);
	if (l)
		m_lights.erase(l);

	CameraComponent* c = dynamic_cast<CameraComponent*>(component);
	if (c)
		m_cameras.erase(c);
}

void World::OrdoredTransparenceMeshRenderer(CameraComponent* camera)
{
	Vector3D cameraForward = camera->GetForwardVector();
	Vector3D cameraPosition = camera->GetWorldPosition();
	std::sort(m_transparentMeshRenderers.begin(), m_transparentMeshRenderers.end(),
		[cameraPosition, cameraForward](MeshRendererComponent* a, MeshRendererComponent* b) {
			int geometryRenderingPriorityA = a->GetGeometryRenderingPriority();
			int geometryRenderingPriorityB = b->GetGeometryRenderingPriority();

			if (geometryRenderingPriorityA == geometryRenderingPriorityB)
			{
				Vector3D positionA = a->GetWorldPosition();
				Vector3D positionB = b->GetWorldPosition();
				float distanceA = Vector3D::DotProduct((positionA - cameraPosition), cameraForward);
				float distanceB = Vector3D::DotProduct((positionB - cameraPosition), cameraForward);

				return distanceA > distanceB;
			}
			else
				return a->GetGeometryRenderingPriority() < b->GetGeometryRenderingPriority();
		});
}

void World::OrdoredOpaqueMeshRenderer()
{
	std::sort(m_opaqueMeshRenderers.begin(), m_opaqueMeshRenderers.end(),
		[](MeshRendererComponent* a, MeshRendererComponent* b) {
			int geometryRenderingPriorityA = a->GetGeometryRenderingPriority();
			int geometryRenderingPriorityB = b->GetGeometryRenderingPriority();
			
			return a->GetGeometryRenderingPriority() < b->GetGeometryRenderingPriority();
				
		});

	m_isOpaqueMeshRendererOrdered = true;
}
