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
#include "Assets/RenderTexture2D.h"
#include "../Game/AntiAliasingTestComponent.h"
#include "Graphics.h"

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
	Shader* ssaaShader = AssetsManager::CreateShader("SSAAShader", "Assets/SSAAShader.vert", "Assets/SSAAShader.frag");
	Shader* msaaShader = AssetsManager::CreateShader("MSAAShader", "Assets/MSAAShader.vert", "Assets/MSAAShader.frag");

	//Double apha shader
	Shader* shader7 = AssetsManager::CreateShader("BlinnPhongShaderAlphaPass1", "Assets/BlinnPhongShaderAlpha.vert", "Assets/BlinnPhongShaderAlphaPass1.frag");
	Shader* shader8 = AssetsManager::CreateShader("BlinnPhongShaderAlphaPass2", "Assets/BlinnPhongShaderAlpha.vert", "Assets/BlinnPhongShaderAlphaPass2.frag");
	
	//Sky Shader
	Shader* cubeMapShader = AssetsManager::CreateShader("CubeMapShader", "Assets/CubeMapShader.vert", "Assets/CubeMapShader.frag");
	Shader* skyboxShader = AssetsManager::CreateShader("SkyboxShader", "Assets/SkyboxShader.vert", "Assets/SkyboxShader.frag");
	Shader* skydomeShader = AssetsManager::CreateShader("SkydomeShader", "Assets/SkydomeShader.vert", "Assets/SkydomeShader.frag");


	//Shader for display the renderTexture on the screen
	Shader* renderShader = AssetsManager::CreateShader("RenderShader", "Assets/BlinnPhongShader.vert", "Assets/RenderShader.frag");

	//Initialise Textures
	Texture* whiteTexture = AssetsManager::CreateTexture2D("White","Assets/WhiteTexture.png");	
	Texture* cubeMap = AssetsManager::CreateCubeMap("CubeMapTexture","Assets/CubeMap.jpg");	
	Texture* simpleTexture = AssetsManager::CreateTexture2D("SimpleTexture","Assets/Texture.png");
	Texture* boxDiffuseTexture = AssetsManager::CreateTexture2D("BoxDiffuseTexture","Assets/Box/BoxDiffuse.png");
	Texture* boxAlphaTexture = AssetsManager::CreateTexture2D("BoxAlphaTexture","Assets/Box/BoxAlpha.png");
	
	//Render Texture
	RenderTexture2D* renderTexture = AssetsManager::CreateRenderTexture2D("RenderTexture", 1920, 1080);
	Texture::LayerTextureInfo layerTextureInfo = Texture::LayerTextureInfo();
	layerTextureInfo.m_minificationFilter = MinificationFilter::Bilinear;
	layerTextureInfo.m_generateMimpap = false;
	layerTextureInfo.m_enableDoubleBuffering = true;
	renderTexture->AttachColorTextureBuffer(ColorRenderableFormat::RGBA8, ColorFormat::RGBA, DataType::UNSIGNED_BYTE,0, layerTextureInfo);
	renderTexture->AttachDepthStencilRenderBuffer(DepthStencilRenderableFormat::DEPTH24_STENCIL8);


	//SSAA Render Texture
	RenderTexture2D* SSAARenderTexture = AssetsManager::CreateRenderTexture2D("SSAA RenderTexture", 1920, 1080);
	layerTextureInfo = Texture::LayerTextureInfo();
	layerTextureInfo.m_minificationFilter = MinificationFilter::Trilinear;
	layerTextureInfo.m_generateMimpap = true;

	SSAARenderTexture->AttachColorTextureBuffer(ColorRenderableFormat::RGBA8, ColorFormat::RGBA, DataType::UNSIGNED_BYTE, 0, layerTextureInfo);
	SSAARenderTexture->AttachDepthStencilRenderBuffer(DepthStencilRenderableFormat::DEPTH24_STENCIL8);

	//MSAA Render Texture temp
	RenderTexture2D* MSAARenderTextureTemp = AssetsManager::CreateRenderTexture2D("MSAA RenderTexture Temp", 1920, 1080);
	layerTextureInfo = Texture::LayerTextureInfo();
	layerTextureInfo.m_generateMimpap = false;
	layerTextureInfo.m_useMultisampledTexture = true;

	MSAARenderTextureTemp->AttachColorTextureBufferMS(ColorRenderableFormat::RGBA8,0 , layerTextureInfo);
	RenderTexture2D::RenderBufferInfo renderBufferInfo = RenderTexture2D::RenderBufferInfo();
	renderBufferInfo.m_useMultisampledRenderBuffer = true;
	MSAARenderTextureTemp->AttachDepthStencilRenderBuffer(DepthStencilRenderableFormat::DEPTH24_STENCIL8, renderBufferInfo);

	// MSAA Render Texture Target
	RenderTexture2D* MSAARenderTextureTarget = AssetsManager::CreateRenderTexture2D("MSAA RenderTexture Target", 1920, 1080);
	layerTextureInfo = Texture::LayerTextureInfo();
	layerTextureInfo.m_minificationFilter = MinificationFilter::Bilinear;
	layerTextureInfo.m_magnificationFilter = MagnificationFilter::Bilinear;
	layerTextureInfo.m_generateMimpap = false;

	MSAARenderTextureTarget->AttachColorTextureBuffer(ColorRenderableFormat::RGBA8, ColorFormat::RGBA, DataType::UNSIGNED_BYTE, 0, layerTextureInfo);

	///Initialise Materials

	//White Material
	Material* whiteMaterial = AssetsManager::CreateBlinnPhongMaterial("WhiteMaterial", whiteTexture, whiteTexture, whiteTexture,Color(1,1,1,1), Color(1, 1, 1, 1), Color(1, 1, 1, 1),32.0f);

	// CubeMap Material
	Material* cubeMapMaterial = AssetsManager::CreateMaterial("CubeMapMaterial");
	cubeMapMaterial->SetTexture("cubemap", cubeMap);

	//SkyDome Material
	Material* skyDomeMaterial = AssetsManager::CreateMaterial("SkydomeMaterial");
	skyDomeMaterial->SetVector3D("lightDirection", Vector3D(0,-1,0));
	skyDomeMaterial->SetColor("colorA", Color(60 / 255.0f, 127 / 255.0f, 170 / 255.0f, 1.0f));
	skyDomeMaterial->SetColor("colorB", Color(112 / 255.0f, 188 / 255.0f, 220 / 255.0f, 1.0f));
	skyDomeMaterial->SetColor("sunColor", Color(255 / 255.0f, 223 / 255.0f, 79 / 255.0f, 1.0f));
	skyDomeMaterial->SetFloat("horizonOffset", 0.53f);
	skyDomeMaterial->SetFloat("exposure", 8);
	skyDomeMaterial->SetFloat("sunSize", 2.0f * Mathf::DegToRad);
	skyDomeMaterial->SetFloat("sunSmoothThreshold", 0.2f * Mathf::DegToRad);

	//SSAA Material
	Material* SSAAMaterial = AssetsManager::CreateMaterial("SSAAMaterial");
	SSAAMaterial->SetTexture("texture2D", SSAARenderTexture);

	//MSAA Material
	Material* MSAAMaterial = AssetsManager::CreateMaterial("MSAAMaterial");
	MSAAMaterial->SetTexture("texture2D", MSAARenderTextureTarget);


	//Materaial for display the renderTexture on the screen
	Material* renderMaterial = AssetsManager::CreateMaterial("RenderMaterial");
	renderMaterial->SetColor("material.color", Color(1, 1, 1, 1));
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

	//Initialis Enitty for display SSAA Render Texture
	EntityGroupAsset* ssAATest = AssetsManager::CreateEntityGroup("SSAAEntityGroup");

	Entity* entity =  ssAATest->CreateEntity<Entity>();
	MeshRendererComponent * meshRendererComponent = entity->AddComponent<MeshRendererComponent>(true);
	meshRendererComponent->SetMesh(quadScreenMesh);
	meshRendererComponent->SetShader(ssaaShader);
	meshRendererComponent->SetMaterial(SSAAMaterial);
	meshRendererComponent->EnableCullFace(false);
	meshRendererComponent->EnableDepthTest(false);
	meshRendererComponent->EnableStencilTest(false);
	meshRendererComponent->EnableBlend(false);

	//Initialis Enitty for display MSAA Render Texture
	EntityGroupAsset* msAATest = AssetsManager::CreateEntityGroup("MSAAEntityGroup");

	entity = msAATest->CreateEntity<Entity>();
	meshRendererComponent = entity->AddComponent<MeshRendererComponent>(true);
	meshRendererComponent->SetMesh(quadScreenMesh);
	meshRendererComponent->SetShader(msaaShader);
	meshRendererComponent->SetMaterial(MSAAMaterial);
	meshRendererComponent->EnableCullFace(false);
	meshRendererComponent->EnableDepthTest(false);
	meshRendererComponent->EnableStencilTest(false);
	meshRendererComponent->EnableBlend(false);
}

void World::InitWorld()
{
	InitAssets();

	LightingSettings::m_globalAmbiantColor = Color(0.2f,0.2f,0.2f,1);
	Scene& objLoaderScene = SceneManager::Instance()->CreateScene();
	Entity* cameraEntity = objLoaderScene.CreateEntity<Entity>();
	{
		CameraComponent* cameraComponent = cameraEntity->AddComponent<CameraComponent>(true);	
		cameraEntity->SetRootComponent(cameraComponent);
		cameraComponent->SetNear(0.01f);
		cameraComponent->SetFar(1000.0);
		cameraComponent->SetProjectionMode(CameraComponent::EProjectionMode::PERSPECTIVE);
		cameraComponent->SetFov(60);
		cameraComponent->SetWorldPosition(Vector3D(0, 1, -10));
		cameraComponent->SetRenderingPriority(1);

		CameraController* cameraController = cameraEntity->AddComponent<CameraController>(true);

		cameraController->m_scrollMove = 0;
		AntiAliasingTestComponent* antiAliasingTestComponent = cameraEntity->AddComponent<AntiAliasingTestComponent>(true);
		antiAliasingTestComponent->m_cameraComponent = cameraComponent;
	}
	/*
	CameraComponent* cameraComponent2 = nullptr;
	Entity* cameraEntity2 = objLoaderScene.CreateEntity<Entity>();
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
	*/
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

	EntityGroupAsset* entityAssets = AssetsManager::GetAsset<EntityGroupAsset>("Stair");
	objLoaderScene.CloneGroupEntityToScene(entityAssets);
	
	Entity* firstEntity = nullptr;
	objLoaderScene.CloneGroupEntityToScene(AssetsManager::GetAsset<EntityGroupAsset>("Table"), firstEntity);

	if (firstEntity)
	{
		SceneComponent* sceneComponent = firstEntity->GetComponent<SceneComponent>();
		if (sceneComponent)
		{
			sceneComponent->SetWorldPosition(Vector3D(0, 0, -3.5f));
			sceneComponent->SetWorldScale(Vector3D(0.15f, 0.15f, 0.15f));
		}
	}

	objLoaderScene.CloneGroupEntityToScene(AssetsManager::GetAsset<EntityGroupAsset>("Bowl"), firstEntity);

	if (firstEntity)
	{
		SceneComponent* sceneComponent = firstEntity->GetComponent<SceneComponent>();
		if (sceneComponent)
		{
			sceneComponent->SetWorldPosition(Vector3D(-2.5, 0.89f, -3.0f));
			sceneComponent->SetWorldScale(Vector3D(1.3f, 1.3f, 1.3f));
		}
	}

	objLoaderScene.CloneGroupEntityToScene(AssetsManager::GetAsset<EntityGroupAsset>("Desk"), firstEntity);

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




	objLoaderScene.CloneGroupEntityToScene(AssetsManager::GetAsset<EntityGroupAsset>("Camera"), firstEntity);

	if (firstEntity)
	{
		SceneComponent* sceneComponent = firstEntity->GetComponent<SceneComponent>();
		if (sceneComponent)
		{
			sceneComponent->SetWorldPosition(Vector3D(0.0f, 1.0f, -4.9f));
			sceneComponent->SetWorldScale(Vector3D(0.001f, 0.001f, 0.001f));
		}
	}

	
	objLoaderScene.CloneGroupEntityToScene(AssetsManager::GetAsset<EntityGroupAsset>("Wall"), firstEntity);

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
	/*
	// Rendu vers le framebuffer hors écran
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glEnable(GL_DEPTH_TEST);  // Activer le test de profondeur
	glViewport(0, 0, 1600, 1200);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Dessiner le triangle
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Blit vers le framebuffer principal (fenêtre)
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, 1600, 1200, 0, 0, 800, 600, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	return;*/

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

	RenderTexture2D* fboSSAA = AssetsManager::GetAsset<RenderTexture2D>("SSAA RenderTexture");

	for (auto it = sortCamera.begin(); it != sortCamera.end(); ++it)
	{
		CameraComponent* camera = *it;

		RenderTexture2D* cameraRT = camera->GetRenderTexture();

		RenderTexture2D* renderTexture = cameraRT;
		
		if (renderTexture == nullptr)
		{
			if (Graphics::GetInstance()->GetAntiAliasingType() == Graphics::AntiAliasingType::SSAA)
			{
				renderTexture = AssetsManager::GetAsset<RenderTexture2D>("SSAA RenderTexture");
				int ssaaMultiplier = Graphics::GetInstance()->GetSSAAMultiplier();
				renderTexture->Resize(window->GetWidth() * ssaaMultiplier, window->GetHeight() * ssaaMultiplier);
				camera->SetRenderTexture(renderTexture);
			}
			else if (Graphics::GetInstance()->GetAntiAliasingType() == Graphics::AntiAliasingType::MSAA_RT)
			{
				renderTexture = AssetsManager::GetAsset<RenderTexture2D>("MSAA RenderTexture Temp");
				renderTexture->Resize(window->GetWidth(), window->GetHeight());
				camera->SetRenderTexture(renderTexture);
			}
		}
		

		int viewportWidth = window->GetWidth();
		int viewportHeight = window->GetHeight();
		if (renderTexture)
		{
			renderTexture->BindFramebuffer();
			viewportWidth = renderTexture->GetWidth();
			viewportHeight = renderTexture->GetHeight();
		}
		else
			Graphics::GetInstance()->BindMainFrameBuffer();
			
	

		float bCornerX = camera->m_viewportBottomCornerX * viewportWidth;
		float bCornerY = camera->m_viewportBottomCornerY * viewportHeight;

		float tCornerX = camera->m_viewportTopCornerX * viewportWidth;
		float tCornerY = camera->m_viewportTopCornerY * viewportHeight;

		glViewport(bCornerX, bCornerY, tCornerX - bCornerX, tCornerY - bCornerY);

		Graphics::GetInstance()->Clear(camera);
		if (camera->m_enableMultiSampling)
			glEnable(GL_MULTISAMPLE);
		else
			glDisable(GL_MULTISAMPLE);


		
		
		for (auto rendererIt = m_opaqueMeshRenderers.begin(); rendererIt != m_opaqueMeshRenderers.end(); ++rendererIt)
		{
			MeshRendererComponent* meshRenderer = *rendererIt;
			meshRenderer->Draw(camera, m_lights, window);
		}
		
		OrdoredTransparenceMeshRenderer(camera);
		for (auto rendererIt = m_transparentMeshRenderers.begin(); rendererIt != m_transparentMeshRenderers.end(); ++rendererIt)
		{
			MeshRendererComponent* meshRenderer = *rendererIt;
			meshRenderer->Draw(camera, m_lights, window);
		}
		
		if (renderTexture)
		{
			renderTexture->SwapBuffer();
			renderTexture->GenerateAllMipmap(false);
			renderTexture->UnBindFramebuffer();

			glViewport(0, 0, window->GetWidth(), window->GetHeight());
		}

		if (cameraRT != renderTexture)
		{
			camera->SetRenderTexture(cameraRT);
		}
	}

	


	if (Graphics::GetInstance()->GetAntiAliasingType() == Graphics::AntiAliasingType::SSAA)
	{
		EntityGroupAsset* entityGroupAsset = AssetsManager::GetAsset<EntityGroupAsset>("SSAAEntityGroup");
		Entity* entity = entityGroupAsset->GetEntityAt(0);

		MeshRendererComponent* renderer = entity->GetComponent<MeshRendererComponent>();
		renderer->Draw(nullptr, std::set<LightComponent*>(), window);	
	}
	else if(Graphics::GetInstance()->GetAntiAliasingType() == Graphics::AntiAliasingType::MSAA_RT)
	{
		RenderTexture2D* temp = AssetsManager::GetAsset<RenderTexture2D>("MSAA RenderTexture Temp");
		

		bool blitToScreen = true;

		if(blitToScreen)
			RenderTexture2D::Blit(temp, nullptr, 0, 0, temp->GetWidth(), temp->GetHeight(), 0, 0, window->GetWidth(), window->GetHeight(), BlitBitField::COLOR_BIT, BlitFilter::NEAREST);
		else
		{
			RenderTexture2D* target = AssetsManager::GetAsset<RenderTexture2D>("MSAA RenderTexture Target");
			target->Resize(window->GetWidth(), window->GetHeight());

			RenderTexture2D::Blit(temp, target, 0, 0, temp->GetWidth(), temp->GetHeight(), 0, 0, target->GetWidth(), target->GetHeight(), BlitBitField::COLOR_BIT, BlitFilter::NEAREST);

			EntityGroupAsset* entityGroupAsset = AssetsManager::GetAsset<EntityGroupAsset>("MSAAEntityGroup");
			Entity* entity = entityGroupAsset->GetEntityAt(0);

			MeshRendererComponent* renderer = entity->GetComponent<MeshRendererComponent>();
			renderer->Draw(nullptr, std::set<LightComponent*>(), window);
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
