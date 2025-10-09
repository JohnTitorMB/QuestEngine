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
#include "PostProcessing/BloomEffect.h"
#include "PostProcessing/Bloom2Effect.h"
#include "PostProcessing/Bloom3Effect.h"
#include "../Library/stb_image.h"
#define _CRT_SECURE_NO_WARNINGS
#include "../Library/stb_image_write.h"
#include "Assets/RenderCubeMap.h"
#include "Components/PointLight.h"
#include <unordered_set>

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
	
	//ReflectedShader
	Shader* shader4 = AssetsManager::CreateShader("ReflectionShader", "Assets/ReflectionShader.vert", "Assets/ReflectionShader.frag");

	//Sky Shader
	Shader* skyboxShader = AssetsManager::CreateShader("SkyboxShader", "Assets/SkyboxShader.vert", "Assets/SkyboxShader.frag");

	//Shader for display the renderTexture on the screen
	Shader* renderShader = AssetsManager::CreateShader("RenderShader", "Assets/BlinnPhongShader.vert", "Assets/RenderShader.frag");


	//PostProcessShader
	Shader* tintShader = AssetsManager::CreateShader("TintShader", "Assets/ScreenShader.vert", "Assets/TintShader.frag");
	Shader* gaussianBlurShader = AssetsManager::CreateShader("GaussianBlurShader", "Assets/ScreenShader.vert", "Assets/GaussianBlurShader.frag");
	Shader* colorGradingShader = AssetsManager::CreateShader("ColorGradingShader", "Assets/ScreenShader.vert", "Assets/ColorGradingShader.frag");
	Shader* lut2DShader = AssetsManager::CreateShader("LUT2DShader", "Assets/ScreenShader.vert", "Assets/LUT2DShader.frag");
	Shader* hdrFilterShader = AssetsManager::CreateShader("HDRFilterShader", "Assets/ScreenShader.vert", "Assets/HDRFilterShader.frag");
	Shader* blendShader = AssetsManager::CreateShader("BlendShader", "Assets/ScreenShader.vert", "Assets/BlendShader.frag");
	Shader* additiveShader = AssetsManager::CreateShader("AdditiveShader", "Assets/ScreenShader.vert", "Assets/AdditiveShader.frag");
	Shader* boxBlurShader = AssetsManager::CreateShader("BoxBlurShader", "Assets/ScreenShader.vert", "Assets/BoxBlurShader.frag");
	Shader* upScaleShader = AssetsManager::CreateShader("UpScaleShader", "Assets/ScreenShader.vert", "Assets/UpScaleShader.frag");
	Shader* shadowShader = AssetsManager::CreateShader("ShadowShader", "Assets/ShadowVertexShader.vert", "Assets/ShadowFragmentShader.frag");
	Shader* pointShadowShader = AssetsManager::CreateShader("PointShadowShader", "Assets/PointShadowFragmentShader.vert", "Assets/PointShadowFragmentShader.frag");
	Shader* depthShader = AssetsManager::CreateShader("DepthShader", "Assets/DepthShader.vert", "Assets/DepthShader.frag");
	Shader* refractorShader = AssetsManager::CreateShader("RefractorShader", "Assets/RefractorShader.vert", "Assets/RefractorShader.frag");


	
	//Initialise Textures
	Texture* whiteTexture = AssetsManager::CreateTexture2D("White","Assets/WhiteTexture.png");	
	Texture* blackTexture = AssetsManager::CreateTexture2D("Black","Assets/BlackTexture.png");	
	Texture* skyboxTexture = AssetsManager::CreateCubeMap("SkyboxTexture", "Assets/Skybox.png");
	Texture* tileTexture = AssetsManager::CreateTexture2D("TileTexture", "Assets/Texture.png");
	Texture* boxDiffuseTexture = AssetsManager::CreateTexture2D("BoxDiffuseTexture","Assets/Box/BoxDiffuse.png");
	Texture* boxAlphaTexture = AssetsManager::CreateTexture2D("BoxAlphaTexture","Assets/Box/BoxAlpha.png");
	Texture* gradiant = AssetsManager::CreateTexture2D("Gradiant","Assets/Gradiant.png");
	Texture2D* lut = AssetsManager::CreateTexture2D("lutYellow", "Assets/lutYellow.png");
	Texture2D* lutCustom = AssetsManager::CreateTexture2D("LUTCustom", "Assets/LutCustom.png");
	Texture2D* NightBlade = AssetsManager::CreateTexture2D("NightBlade", "Assets/NightBlade.png");
	Texture2D* BumpTexture = AssetsManager::CreateTexture2D("BumpTexture", "Assets/bumpTexture.jpg");

	
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

	//HDR Render Texture
	RenderTexture2D* HDRRenderTexture = AssetsManager::CreateRenderTexture2D("HDRRenderTexture", 1920, 1080);
	Texture::LayerTextureInfo layerTextureInfo = Texture::LayerTextureInfo();
	layerTextureInfo.m_minificationFilter = MinificationFilter::Point;
	layerTextureInfo.m_magnificationFilter = MagnificationFilter::Point;
	layerTextureInfo.m_generateMimpap = false;

	layerTextureInfo.m_useMultisampledTexture = true;
	layerTextureInfo.m_samples = 16;

	HDRRenderTexture->AttachColorTextureBufferMS(ColorRenderableFormat::RGBA16F,0, layerTextureInfo);

	RenderTexture2D::RenderBufferInfo renderBufferInfo = RenderTexture2D::RenderBufferInfo();
	renderBufferInfo.m_useMultisampledRenderBuffer = true;
	renderBufferInfo.m_samples = 16;
	HDRRenderTexture->AttachDepthStencilRenderBuffer(DepthStencilRenderableFormat::DEPTH24_STENCIL8, renderBufferInfo);

	//Render Texture Temp
	RenderTexture2D* renderTextureTemp = AssetsManager::CreateRenderTexture2D("RenderTextureTemp", 1920, 1080);
	layerTextureInfo = Texture::LayerTextureInfo();
	layerTextureInfo.m_minificationFilter = MinificationFilter::Point;
	layerTextureInfo.m_magnificationFilter = MagnificationFilter::Point;
	layerTextureInfo.m_generateMimpap = false;

	renderTextureTemp->AttachColorTextureBuffer(ColorRenderableFormat::RGBA8, ColorFormat::RGBA, DataType::UNSIGNED_BYTE, 0, layerTextureInfo);
	renderTextureTemp->AttachDepthStencilRenderBuffer(DepthStencilRenderableFormat::DEPTH24_STENCIL8);

	///Initialise Materials

	//White Material
	Material* whiteMaterial = AssetsManager::CreateBlinnPhongMaterial("WhiteMaterial", whiteTexture, whiteTexture, whiteTexture, blackTexture, ColorRGB(1,1,1,1), ColorRGB(1, 1, 1, 1), ColorRGB(1, 1, 1, 1), ColorRGB(0, 0, 0, 1),32.0f);
	
	Material* reflectedMaterial = AssetsManager::CreateBlinnPhongMaterial("ReflectedMaterial", whiteTexture, whiteTexture, whiteTexture, blackTexture, ColorRGB(1,1,1,1), ColorRGB(1, 1, 1, 1), ColorRGB(1, 1, 1, 1), ColorRGB(0, 0, 0, 1),32.0f);
	reflectedMaterial->SetTexture("reflectedCubeMap", skyboxTexture);

	Material* reflactorMaterial = AssetsManager::CreateBlinnPhongMaterial("ReflactorMaterial", whiteTexture, whiteTexture, whiteTexture, blackTexture, ColorRGB(1, 1, 1, 1), ColorRGB(1, 1, 1, 1), ColorRGB(1, 1, 1, 1), ColorRGB(0, 0, 0, 1), 32.0f);
	reflactorMaterial->SetTexture("bumpTexture", BumpTexture);
	reflactorMaterial->SetTexture("reflectionTexture", skyboxTexture);
	
	// CubeMap Material
	Material* skyboxMaterial = AssetsManager::CreateMaterial("SkyboxMaterial");
	skyboxMaterial->SetTexture("cubemap", skyboxTexture);

	//Materaial for display the renderTexture on the screen
	Material* renderMaterial = AssetsManager::CreateMaterial("RenderMaterial");
	renderMaterial->SetColor("material.color", ColorRGB(1, 1, 1, 1));
	renderMaterial->SetTexture("material.texture", renderTexture, 0);
	renderMaterial->SetVector4D("material.textureST", Vector4D(0, 0, -1, 1));
	renderMaterial->SetFloat("material.intensity", 2.0f);

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

	entityGroupAsset = OBJLibrary::OBJLoader::LoadOBJ("Bunker", "Assets/Bunker/Bunker.obj");
	screenEntity = entityGroupAsset->GetEntityAt(56); // Get the screen entity of desk
	meshRenderComponent = screenEntity->GetComponent<MeshRendererComponent>();
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
	//Old Scene
	Scene& scene1 = SceneManager::Instance()->CreateScene();
	{
		LightingSettings::m_globalAmbiantColor = ColorRGB(0.05, 0.05f, 0.05f, 1);
		Entity* cameraEntity = scene1.CreateEntity<Entity>();
		{
			CameraComponent* cameraComponent = cameraEntity->AddComponent<CameraComponent>(true);
			cameraEntity->SetRootComponent(cameraComponent);
			cameraComponent->SetNear(0.1f);
			cameraComponent->SetFar(1000.0);
			cameraComponent->SetProjectionMode(CameraComponent::EProjectionMode::PERSPECTIVE);
			cameraComponent->SetFov(60);
			cameraComponent->SetWorldPosition(Vector3D(-0.33, 1, -6.6));



			cameraComponent->SetRenderingPriority(1);
			cameraComponent->m_enableMultiSampling = true;
			cameraComponent->m_enableHDR = true;
			CameraController* cameraController = cameraEntity->AddComponent<CameraController>(true);
			cameraController->m_scrollMove = 10;
			cameraController->movementSpeed = 0.5f;
			PostProcessing* postProcessing = cameraEntity->AddComponent<PostProcessing>(true);

		}

		Entity* postProcessVolumeEntity = scene1.CreateEntity<Entity>();
		{
			PostProcessingVolume* postProcessVolume = postProcessVolumeEntity->AddComponent<PostProcessingVolume>();

			std::shared_ptr<Bloom3Settings> bloom3Setting = std::make_shared<Bloom3Settings>();
			postProcessVolume->AddEffect(bloom3Setting);


			std::shared_ptr<ColorGradingSettings> colorGradingSetting = std::make_shared<ColorGradingSettings>();
			postProcessVolume->AddEffect(colorGradingSetting);

			postProcessVolume->SetGlobal(true);

			PostProcessing* postProcessing = cameraEntity->GetComponent<PostProcessing>();
			postProcessing->AddPostProcessingVolume(postProcessVolume);
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
			dLightComponent->SetShadowBlurResolution(5);
			dLightComponent->SetWorldRotation(Quaternion::FromEulerAngle(Vector3D(50.0f, -30.0f, 0)));
			dLightComponent->SetShadowFar(25.0f);
			dLightComponent->SetShadowSize(20.0f);
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
			meshRendererComponent->EnableDepthMask(false);
			meshRendererComponent->SetCastShadow(false);
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
				sceneComponent->SetWorldRotation(Quaternion::FromEulerAngle(Vector3D(0, 270, 0)));
				sceneComponent->SetWorldScale(Vector3D(1.0f, 1.0f, 1.0f));
			}
		}

		scene1.CloneGroupEntityToScene(AssetsManager::GetAsset<EntityGroupAsset>("Camera"), firstEntity);

		if (firstEntity)
		{
			SceneComponent* sceneComponent = firstEntity->GetComponent<SceneComponent>();
			if (sceneComponent)
			{
				sceneComponent->SetWorldPosition(Vector3D(0.0f, 1.0f, -4.9f));
				sceneComponent->SetWorldScale(Vector3D(0.001f, 0.001f, 0.001f));
			}
		}

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
	}

	//Reflexion Scene
	Scene& scene2 = SceneManager::Instance()->CreateScene();
	{
		LightingSettings::m_globalAmbiantColor = ColorRGB(0.05, 0.05f, 0.05f, 1);
		Entity* cameraEntity = scene2.CreateEntity<Entity>();
		{
			CameraComponent* cameraComponent = cameraEntity->AddComponent<CameraComponent>(true);
			cameraEntity->SetRootComponent(cameraComponent);
			cameraComponent->SetNear(0.1f);
			cameraComponent->SetFar(1000.0);
			cameraComponent->SetProjectionMode(CameraComponent::EProjectionMode::PERSPECTIVE);
			cameraComponent->SetFov(60);
			cameraComponent->SetWorldPosition(Vector3D(-0.33, 1, -6.6));



			cameraComponent->SetRenderingPriority(1);
			cameraComponent->m_enableMultiSampling = true;
			cameraComponent->m_enableHDR = true;
			CameraController* cameraController = cameraEntity->AddComponent<CameraController>(true);
			cameraController->m_scrollMove = 10;
			cameraController->movementSpeed = 0.5f;
			PostProcessing* postProcessing = cameraEntity->AddComponent<PostProcessing>(true);

		}

		Entity* postProcessVolumeEntity = scene2.CreateEntity<Entity>();
		{
			PostProcessingVolume* postProcessVolume = postProcessVolumeEntity->AddComponent<PostProcessingVolume>();

			std::shared_ptr<Bloom3Settings> bloom3Setting = std::make_shared<Bloom3Settings>();
			bloom3Setting->intensity = 0.5;
			postProcessVolume->AddEffect(bloom3Setting);


			std::shared_ptr<ColorGradingSettings> colorGradingSetting = std::make_shared<ColorGradingSettings>();
			postProcessVolume->AddEffect(colorGradingSetting);

			postProcessVolume->SetGlobal(true);

			PostProcessing* postProcessing = cameraEntity->GetComponent<PostProcessing>();
			postProcessing->AddPostProcessingVolume(postProcessVolume);
		}

		//Initialise Directional Light entity	
		DirectionalLightComponent* dLightComponent = nullptr;
		Entity* lightEntity = scene2.CreateEntity<Entity>();
		{
			dLightComponent = lightEntity->AddComponent<DirectionalLightComponent>(true);
			dLightComponent->m_ambiantColor = ColorRGB(0.1f, 0.1f, 0.1f, 1.0f);
			dLightComponent->m_diffuseColor = ColorRGB(1.0f, 1.0f, 1.0f, 1.0f);
			dLightComponent->m_specularColor = ColorRGB(1.0f, 1.0f, 1.0f, 0.0f);
			dLightComponent->m_intensity = 1.0f;
			dLightComponent->SetShadowBlurResolution(5);
			dLightComponent->SetWorldRotation(Quaternion::FromEulerAngle(Vector3D(50.0f, -30.0f, 0)));
			dLightComponent->SetShadowFar(25.0f);
			dLightComponent->SetShadowSize(20.0f);
			DirectionalLightControllerComponent* dLightControllerComponent = lightEntity->AddComponent<DirectionalLightControllerComponent>(true);
			dLightControllerComponent->SetDirectionalLightComponent(dLightComponent);
		}

		Entity* skyboxEntity = scene2.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = skyboxEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("SkyboxShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("SkyboxMaterial"));
			meshRendererComponent->EnableCullFace(false);
			meshRendererComponent->m_useViewMatrixWithoutTranslation = true;
			meshRendererComponent->SetDepthTestFunc(DepthTestFunc::Lequal);
			meshRendererComponent->EnableDepthMask(false);
			meshRendererComponent->SetCastShadow(false);
			meshRendererComponent->SetGeometryRenderingPriority(10000);
		}

		Entity* cubeEntity = scene2.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("ReflectionShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("ReflectedMaterial"));
		}
	}
	
	//Reflexion/Refraction Scene
	Scene& scene3 = SceneManager::Instance()->CreateScene();
	{
		LightingSettings::m_globalAmbiantColor = ColorRGB(0.05, 0.05f, 0.05f, 1);
		Entity* cameraEntity = scene3.CreateEntity<Entity>();
		{
			CameraComponent* cameraComponent = cameraEntity->AddComponent<CameraComponent>(true);
			cameraEntity->SetRootComponent(cameraComponent);
			cameraComponent->SetNear(0.1f);
			cameraComponent->SetFar(1000.0);
			cameraComponent->SetProjectionMode(CameraComponent::EProjectionMode::PERSPECTIVE);
			cameraComponent->SetFov(60);
			cameraComponent->SetWorldPosition(Vector3D(-0.33, 1, -6.6));



			cameraComponent->SetRenderingPriority(1);
			cameraComponent->m_enableMultiSampling = true;
			cameraComponent->m_enableHDR = true;
			CameraController* cameraController = cameraEntity->AddComponent<CameraController>(true);
			cameraController->m_scrollMove = 10;
			cameraController->movementSpeed = 0.5f;
			PostProcessing* postProcessing = cameraEntity->AddComponent<PostProcessing>(true);

		}

		Entity* postProcessVolumeEntity = scene3.CreateEntity<Entity>();
		{
			PostProcessingVolume* postProcessVolume = postProcessVolumeEntity->AddComponent<PostProcessingVolume>();

			std::shared_ptr<Bloom3Settings> bloom3Setting = std::make_shared<Bloom3Settings>();
			bloom3Setting->intensity = 0.5;
			postProcessVolume->AddEffect(bloom3Setting);


			std::shared_ptr<ColorGradingSettings> colorGradingSetting = std::make_shared<ColorGradingSettings>();
			postProcessVolume->AddEffect(colorGradingSetting);

			postProcessVolume->SetGlobal(true);

			PostProcessing* postProcessing = cameraEntity->GetComponent<PostProcessing>();
			postProcessing->AddPostProcessingVolume(postProcessVolume);
		}

		//Initialise Directional Light entity	
		DirectionalLightComponent* dLightComponent = nullptr;
		Entity* lightEntity = scene3.CreateEntity<Entity>();
		{
			dLightComponent = lightEntity->AddComponent<DirectionalLightComponent>(true);
			dLightComponent->m_ambiantColor = ColorRGB(0.1f, 0.1f, 0.1f, 1.0f);
			dLightComponent->m_diffuseColor = ColorRGB(1.0f, 1.0f, 1.0f, 1.0f);
			dLightComponent->m_specularColor = ColorRGB(1.0f, 1.0f, 1.0f, 0.0f);
			dLightComponent->m_intensity = 1.0f;
			dLightComponent->SetShadowBlurResolution(5);
			dLightComponent->SetWorldRotation(Quaternion::FromEulerAngle(Vector3D(50.0f, -30.0f, 0)));
			dLightComponent->SetShadowFar(25.0f);
			dLightComponent->SetShadowSize(20.0f);
			DirectionalLightControllerComponent* dLightControllerComponent = lightEntity->AddComponent<DirectionalLightControllerComponent>(true);
			dLightControllerComponent->SetDirectionalLightComponent(dLightComponent);
		}

		Entity* skyboxEntity = scene3.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = skyboxEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("SkyboxShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("SkyboxMaterial"));
			meshRendererComponent->EnableCullFace(false);
			meshRendererComponent->m_useViewMatrixWithoutTranslation = true;
			meshRendererComponent->SetDepthTestFunc(DepthTestFunc::Lequal);
			meshRendererComponent->EnableDepthMask(false);
			meshRendererComponent->SetCastShadow(false);
			meshRendererComponent->SetGeometryRenderingPriority(10000);
		}

		Entity* refractedObject = scene3.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = refractedObject->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("RefractorShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("ReflactorMaterial"));
			meshRendererComponent->SetWorldScale(Vector3D(10,1,10));
			meshRendererComponent->SetCastShadow(false);
			meshRendererComponent->SetReceiveShadow(false);
			meshRendererComponent->EnableRefracted(true);
		}
		
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				Entity* cubeEntity = scene3.CreateEntity<Entity>();
				{
					MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
					meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
					meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("BlinnPhongShader"));
					meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("WhiteMaterial"));
					meshRendererComponent->SetWorldPosition(Vector3D(-4.0f + i*2.0f,-5.0f,-4.0f + j*2.0f));
				}
			}
		}

	}

	// PBR Scene
	Scene& scene4 = SceneManager::Instance()->CreateScene();
	{
		LightingSettings::m_globalAmbiantColor = ColorRGB(0.05, 0.05f, 0.05f, 1);

		// Camera
		Entity* cameraEntity = scene4.CreateEntity<Entity>();
		{
			CameraComponent* cameraComponent = cameraEntity->AddComponent<CameraComponent>(true);
			cameraEntity->SetRootComponent(cameraComponent);
			cameraComponent->SetNear(0.1f);
			cameraComponent->SetFar(1000.0);
			cameraComponent->SetProjectionMode(CameraComponent::EProjectionMode::PERSPECTIVE);
			cameraComponent->SetFov(60);
			cameraComponent->SetWorldPosition(Vector3D(0.0f, 6.0f, 10.0f));
			cameraComponent->SetRenderingPriority(1);
			cameraComponent->m_enableMultiSampling = true;
			cameraComponent->m_enableHDR = true;

			CameraController* cameraController = cameraEntity->AddComponent<CameraController>(true);
			cameraController->m_scrollMove = 10;
			cameraController->movementSpeed = 0.5f;

			PostProcessing* postProcessing = cameraEntity->AddComponent<PostProcessing>(true);
		}

		// Directional light
		DirectionalLightComponent* dLightComponent = nullptr;
		Entity* lightEntity = scene4.CreateEntity<Entity>();
		{
			dLightComponent = lightEntity->AddComponent<DirectionalLightComponent>(true);
			dLightComponent->m_ambiantColor = ColorRGB(0.1f, 0.1f, 0.1f, 1.0f);
			dLightComponent->m_diffuseColor = ColorRGB(1.0f, 1.0f, 1.0f, 1.0f);
			dLightComponent->m_specularColor = ColorRGB(1.0f, 1.0f, 1.0f, 1.0f);
			dLightComponent->m_intensity = 1.0f;
			dLightComponent->SetShadowBlurResolution(5);
			dLightComponent->SetWorldRotation(Quaternion::FromEulerAngle(Vector3D(50.0f, -30.0f, 0)));
			dLightComponent->SetShadowFar(50.0f);
			dLightComponent->SetShadowSize(30.0f);
			dLightComponent->SetShadowMinBias(0.15f);
			dLightComponent->SetShadowMaxBias(0.25f);
			DirectionalLightControllerComponent* dLightControllerComponent = lightEntity->AddComponent<DirectionalLightControllerComponent>(true);
			dLightControllerComponent->SetDirectionalLightComponent(dLightComponent);
		}

		Entity* postProcessVolumeEntity = scene4.CreateEntity<Entity>();
		{
			PostProcessingVolume* postProcessVolume = postProcessVolumeEntity->AddComponent<PostProcessingVolume>();

			std::shared_ptr<Bloom3Settings> bloom3Setting = std::make_shared<Bloom3Settings>();
			postProcessVolume->AddEffect(bloom3Setting);
			
			postProcessVolume->SetGlobal(true);

			PostProcessing* postProcessing = cameraEntity->GetComponent<PostProcessing>();
			postProcessing->AddPostProcessingVolume(postProcessVolume);
		}

		CameraComponent* cameraComponent2 = nullptr;
		Entity* cameraEntity2 = scene4.CreateEntity<Entity>();
		{
			cameraComponent2 = cameraEntity2->AddComponent<CameraComponent>(true);
			cameraComponent2->SetNear(0.01f);
			cameraComponent2->SetFar(1000.0);
			cameraComponent2->SetProjectionMode(CameraComponent::EProjectionMode::PERSPECTIVE);
			cameraComponent2->SetFov(60);
			cameraComponent2->SetWorldPosition(Vector3D(-2.87488, 4.18062, 7.05849));
			cameraComponent2->SetWorldRotation(Quaternion::FromEulerAngle(Vector3D(0.0f, 180.0f, 0.0f)));
			cameraEntity2->SetRootComponent(cameraComponent2);
			cameraComponent2->SetRenderTexture(AssetsManager::GetAsset<RenderTexture2D>("RenderTexture"));
		}

		/*Entity* cubeEntity = scene4.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = cubeEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("ReflectionShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("ReflectedMaterial"));
			meshRendererComponent->SetWorldPosition(Vector3D(-2.87488, 4.18062 , 7.05849));
			meshRendererComponent->SetWorldScale(Vector3D(0.1f, 0.1f, 0.1f));
		}*/

		// Skybox
		Entity* skyboxEntity = scene4.CreateEntity<Entity>();
		{
			MeshRendererComponent* meshRendererComponent = skyboxEntity->AddComponent<MeshRendererComponent>(true);
			meshRendererComponent->SetMesh(AssetsManager::GetAsset<Mesh>("CubeMesh"));
			meshRendererComponent->SetShader(AssetsManager::GetAsset<Shader>("SkyboxShader"));
			meshRendererComponent->SetMaterial(AssetsManager::GetAsset<Material>("SkyboxMaterial"));
			meshRendererComponent->EnableCullFace(false);
			meshRendererComponent->m_useViewMatrixWithoutTranslation = true;
			meshRendererComponent->SetDepthTestFunc(DepthTestFunc::Lequal);
			meshRendererComponent->EnableDepthMask(false);
			meshRendererComponent->SetCastShadow(false);
			meshRendererComponent->SetGeometryRenderingPriority(10000);
		}

		EntityGroupAsset* entityAssets = AssetsManager::GetAsset<EntityGroupAsset>("Bunker");
		scene4.CloneGroupEntityToScene(entityAssets);


	}

	SceneManager::Instance()->LoadScene(3);

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

bool SaveDepthTextureToPNG(GLuint depthTextureID, int width, int height, const std::string& filename)
{
	glBindTexture(GL_TEXTURE_2D, depthTextureID);

	// Lire les valeurs de profondeur (float entre 0.0 et 1.0)
	std::vector<float> depthPixels(width * height);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, depthPixels.data());

	// Convertir en unsigned char (grayscale 0–255)
	std::vector<unsigned char> pixels8bit(width * height);
	for (int i = 0; i < width * height; ++i)
	{
		float depth = depthPixels[i];
		depth = Mathf::Clamp(depth, 0.0f, 1.0f);
		pixels8bit[i] = static_cast<unsigned char>(depth * 255.0f);
	}

	// Flip verticalement
	for (int y = 0; y < height / 2; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			std::swap(pixels8bit[y * width + x], pixels8bit[(height - 1 - y) * width + x]);
		}
	}

	int result = stbi_write_png(filename.c_str(), width, height, 1, pixels8bit.data(), width);
	glBindTexture(GL_TEXTURE_2D, 0);
	return result != 0;
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

	GL_SCOPE("Shadow Map");


	//Genere ShadowMap
	for (auto light : m_lights)
	{
		if (light->m_lightType == LightComponent::LightType::Point)
		{
			PointLightComponent* pointLight = static_cast<PointLightComponent*>(light);
			RenderCubeMap* shadowDepth = pointLight->GetShadowMap();
			CameraComponent lightCamera = CameraComponent();
			lightCamera.SetProjectionMode(CameraComponent::EProjectionMode::PERSPECTIVE);
			lightCamera.SetFov(90.0f);
			lightCamera.SetRenderTexture(shadowDepth);
			lightCamera.SetWorldPosition(pointLight->GetWorldPosition());
			lightCamera.SetNear(pointLight->GetShadowNear());
			lightCamera.SetFar(pointLight->GetShadowFar());


			for (int i = 0; i < 6; i++)
			{

				Vector3D rotation;

				switch (i)
				{
					case 0: rotation = Vector3D(0, 90, 0); break;  // +X
					case 1: rotation = Vector3D(0, -90, 0); break; // -X
					case 2: rotation = Vector3D(90, 0, 0); break; // +Y
					case 3: rotation = Vector3D(-90, 0, 0); break;  // -Y
					case 4: rotation = Vector3D(0, 0, 0); break;   // +Z
					case 5: rotation = Vector3D(0, 180, 0); break; // -Z
				}

				lightCamera.SetWorldRotation(Quaternion::FromEulerAngle(rotation));
				shadowDepth->AttachFaceToFramebuffer(BufferAttachment::DepthAttachment, i);

				shadowDepth->BindFramebuffer();
				glViewport(0, 0, shadowDepth->GetWidth(), shadowDepth->GetHeight());
				Graphics::GetInstance()->Clear(&lightCamera, 0, 0, shadowDepth->GetWidth(), shadowDepth->GetHeight());

				for (auto rendererIt = m_opaqueMeshRenderers.begin(); rendererIt != m_opaqueMeshRenderers.end(); ++rendererIt)
				{
					MeshRendererComponent* meshRenderer = *rendererIt;
					meshRenderer->Draw(&lightCamera, m_lights, window, shadowDepth, MeshRendererComponent::RenderingPassType::ShadowMap);
				}
			}		

			shadowDepth->SwapBuffer();
			shadowDepth->GenerateAllMipmap(false);
			shadowDepth->UnBindFramebuffer();
		}
		else
		{
			RenderTexture2D* shadowDepth = nullptr;
			CameraComponent lightCamera = CameraComponent();
			if (light->m_lightType == LightComponent::LightType::Directional)
			{
				DirectionalLightComponent* dirLight = static_cast<DirectionalLightComponent*>(light);
				shadowDepth = dirLight->GetShadowMap();

				Vector3D lightDir = light->GetForwardVector();
				Vector3D lightTarget = Vector3D::Zero;
				Vector3D lightPosition = dirLight->GetShadowMapTargetPos() - lightDir.Normalized() * dirLight->GetShadowDistance();

				lightCamera.SetProjectionMode(CameraComponent::EProjectionMode::ORTHOGRAPHIC);

				lightCamera.SetRenderTexture(shadowDepth);
				lightCamera.SetWorldPosition(lightPosition);
				lightCamera.SetWorldRotation(Quaternion::SetGoToRotation(lightCamera.GetForwardVector(), lightDir));
				lightCamera.SetSize(dirLight->GetShadowSize());
				lightCamera.SetNear(dirLight->GetShadowNear());
				lightCamera.SetFar(dirLight->GetShadowFar());
			}
			else
			{
				SpotLightComponent* spotLight = static_cast<SpotLightComponent*>(light);
				shadowDepth = spotLight->GetShadowMap();
				Vector3D lightDir = light->GetForwardVector();

				lightCamera.SetProjectionMode(CameraComponent::EProjectionMode::PERSPECTIVE);
				lightCamera.SetFov(spotLight->m_spotAngle);
				lightCamera.SetRenderTexture(shadowDepth);
				lightCamera.SetWorldPosition(spotLight->GetWorldPosition());
				lightCamera.SetWorldRotation(Quaternion::SetGoToRotation(lightCamera.GetForwardVector(), lightDir));
				lightCamera.SetNear(spotLight->GetShadowNear());
				lightCamera.SetFar(spotLight->GetShadowFar());

			}

			shadowDepth->BindFramebuffer();
			glViewport(0, 0, shadowDepth->GetWidth(), shadowDepth->GetHeight());
			Graphics::GetInstance()->Clear(&lightCamera, 0, 0, shadowDepth->GetWidth(), shadowDepth->GetHeight());

			for (auto rendererIt = m_opaqueMeshRenderers.begin(); rendererIt != m_opaqueMeshRenderers.end(); ++rendererIt)
			{
				MeshRendererComponent* meshRenderer = *rendererIt;
				meshRenderer->Draw(&lightCamera, m_lights, window, shadowDepth, MeshRendererComponent::RenderingPassType::ShadowMap);
			}

			shadowDepth->SwapBuffer();
			shadowDepth->GenerateAllMipmap(false);
			shadowDepth->UnBindFramebuffer();
		}

	}

	//Display Scene
	for (auto it = sortCamera.begin(); it != sortCamera.end(); ++it)
	{
		CameraComponent* camera = *it;




		RenderTexture2D* cameraRT = dynamic_cast<RenderTexture2D*>(camera->GetRenderTexture());
		
		bool displayOnMainFB = false;

		int viewportWidth = window->GetWidth();
		int viewportHeight = window->GetHeight();
		if (cameraRT)
		{
			cameraRT->BindFramebuffer();
			viewportWidth = cameraRT->GetWidth();
			viewportHeight = cameraRT->GetHeight();
		}
		else
		{
			if (camera->m_enableHDR)
			{
				cameraRT = AssetsManager::GetAsset<RenderTexture2D>("HDRRenderTexture");
				cameraRT->Resize(viewportWidth, viewportHeight);

				viewportWidth = cameraRT->GetWidth();
				viewportHeight = cameraRT->GetHeight();
			}
			else
			{
				cameraRT = AssetsManager::GetAsset<RenderTexture2D>("RenderTextureTemp");
				cameraRT->Resize(viewportWidth, viewportHeight);

				viewportWidth = cameraRT->GetWidth();
				viewportHeight = cameraRT->GetHeight();

				displayOnMainFB = true;
			}
		}

		Texture::LayerTextureInfo layerTextureInfo = Texture::LayerTextureInfo();
		layerTextureInfo.m_minificationFilter = MinificationFilter::Point;
		layerTextureInfo.m_magnificationFilter = MagnificationFilter::Point;
		layerTextureInfo.m_generateMimpap = false;

//		layerTextureInfo.m_useMultisampledTexture = true;
//		layerTextureInfo.m_samples = 16;

		if (camera->m_enableMultiSampling)
		{
			layerTextureInfo.m_useMultisampledTexture = true;
			layerTextureInfo.m_samples = 16;
			cameraRT->AttachColorTextureBufferMS(ColorRenderableFormat::RGBA16F, 1, layerTextureInfo);
		}
		else
			cameraRT->AttachColorTextureBuffer(ColorRenderableFormat::RGBA16F, ColorFormat::RGBA, DataType::FLOAT, 1, layerTextureInfo);

		float bCornerX = camera->m_viewportBottomCornerX * viewportWidth;
		float bCornerY = camera->m_viewportBottomCornerY * viewportHeight;

		float tCornerX = camera->m_viewportTopCornerX * viewportWidth;
		float tCornerY = camera->m_viewportTopCornerY * viewportHeight;
		glViewport(bCornerX, bCornerY, tCornerX - bCornerX, tCornerY - bCornerY);

		//Render depth Refracted Object
		int rCount = m_refractedMeshRenderers.size();
		while (m_refractedRTs.size() < rCount)
		{
			RenderTexture2D* depthRT = AssetsManager::CreateRenderTexture2D("DepthRT", viewportWidth, viewportHeight);

			Texture::LayerTextureInfo layerTextureInfo = Texture::LayerTextureInfo();
			layerTextureInfo.m_generateMimpap = false;
			layerTextureInfo.m_minificationFilter = MinificationFilter::Point;
			layerTextureInfo.m_magnificationFilter = MagnificationFilter::Point;
			layerTextureInfo.m_wrapVerticalParameter = Wrap::ClampToEdge;
			layerTextureInfo.m_wrapHorizontalParameter = Wrap::ClampToEdge;
			//depthRT->AttachDepthTextureBuffer(DepthRenderableFormat::DEPTH_COMPONENT, DataType::FLOAT, layerTextureInfo);
			depthRT->AttachColorTextureBuffer(ColorRenderableFormat::RGB32F, ColorFormat::RED, DataType::FLOAT, 0, layerTextureInfo);
			depthRT->AttachDepthTextureBuffer(DepthRenderableFormat::DEPTH_COMPONENT, DataType::FLOAT);

			m_refractedRTs.push_back(depthRT);

			RenderTexture2D* behindRT = AssetsManager::CreateRenderTexture2D("BehindRT", viewportWidth, viewportHeight);

			layerTextureInfo = Texture::LayerTextureInfo();
			layerTextureInfo.m_generateMimpap = false;
			layerTextureInfo.m_minificationFilter = MinificationFilter::Point;
			layerTextureInfo.m_magnificationFilter = MagnificationFilter::Point;
			layerTextureInfo.m_wrapVerticalParameter = Wrap::ClampToEdge;
			layerTextureInfo.m_wrapHorizontalParameter = Wrap::ClampToEdge;
			behindRT->AttachColorTextureBuffer(ColorRenderableFormat::RGBA16F, ColorFormat::RGBA, DataType::FLOAT, 0, layerTextureInfo);

			m_behindRefractedRTs.push_back(behindRT);
		}

		while (m_refractedRTs.size() > rCount)
		{
			RenderTexture2D* depthRT = m_refractedRTs[m_refractedRTs.size() - 1];
			auto it = std::find(m_refractedRTs.begin(), m_refractedRTs.end(), depthRT);
			if (it != m_refractedRTs.end()) {
				m_refractedRTs.erase(it);
			}

			if (depthRT != nullptr)
				AssetsManager::DestroyAsset(depthRT);
		}

		GL_SCOPE("Render Depth Refractor Object");

		for (int i = 0; i < m_refractedMeshRenderers.size(); i++)
		{
			m_refractedRTs[i]->BindFramebuffer();

			Graphics::ClearParams p;
			p.clearBufferMask = GL_DEPTH_BUFFER_BIT;
			p.depthWrite = GL_TRUE;
			p.colorMaskR = GL_TRUE;
			p.colorMaskG = p.colorMaskB = p.colorMaskA = GL_FALSE;
			p.r = -1.0f;
			p.depth = 1.0f;

			Graphics::GetInstance()->Clear(bCornerX, bCornerY, tCornerX - bCornerX, tCornerY - bCornerY, p);

			MeshRendererComponent* meshRenderer = m_refractedMeshRenderers[i];
			meshRenderer->Draw(camera, m_lights, window, m_refractedRTs[i], MeshRendererComponent::RenderingPassType::DepthMap);

			m_refractedRTs[i]->SwapBuffer();
			m_refractedRTs[i]->GenerateAllMipmap(false);
			m_refractedRTs[i]->UnBindFramebuffer();
		}

		//if(cameraRT)
		//	cameraRT->BindFramebuffer();
		//else
		//{
		//	if(camera->m_enableHDR)
		//		cameraRT->BindFramebuffer();
		//	else
		//		Graphics::GetInstance()->BindMainFrameBuffer();
		//}

		GL_SCOPE("Render scene");

		cameraRT->BindFramebuffer();

		static const GLenum bufs2[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, bufs2);

		Graphics::GetInstance()->Clear(camera, bCornerX, bCornerY, tCornerX - bCornerX, tCornerY - bCornerY);
		if (camera->m_enableMultiSampling)
		{
			glEnable(GL_MULTISAMPLE);
			glEnable(GL_SAMPLE_SHADING);
			glMinSampleShading(1.0f);
		}
		else
			glDisable(GL_MULTISAMPLE);

		//float red[4] = { 1,0,0,1 };
		//float blue[4] = { 0,0,1,1 };
		//glClearBufferfv(GL_COLOR, 0, red);    // ATT0 = rouge
		//glClearBufferfv(GL_COLOR, 1, blue);  // ATT1 = blue

		for (auto rendererIt = m_opaqueMeshRenderers.begin(); rendererIt != m_opaqueMeshRenderers.end(); ++rendererIt)
		{
			MeshRendererComponent* meshRenderer = *rendererIt;
			meshRenderer->Draw(camera, m_lights, window, cameraRT);
		}
		
		std::vector<MeshRendererComponent*> sortedRenderers = OrdoredTransparenceRefractedMeshRenderer(camera);
		//OrdoredTransparenceMeshRenderer(camera);
		for (auto rendererIt = sortedRenderers.begin(); rendererIt != sortedRenderers.end(); ++rendererIt)
		{
			MeshRendererComponent* meshRenderer = *rendererIt;

			if (meshRenderer->IsRefracted())
			{
				RenderTexture2D::Blit(cameraRT, 1, m_behindRefractedRTs[0], 0, bCornerX, bCornerY, tCornerX, tCornerY,
					bCornerX, bCornerY, tCornerX, tCornerY,
					BlitBitField::COLOR_BIT, BlitFilter::NEAREST);

				

			}

			meshRenderer->Draw(camera, m_lights, window, cameraRT);
		}

		GLenum onlyScene2[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, onlyScene2);

		if (camera->m_enableMultiSampling)
		{
			glDisable(GL_SAMPLE_SHADING);
			glDisable(GL_MULTISAMPLE);
		}

		GL_SCOPE("Post Process");

		PostProcessing* postProcessing = camera->GetOwnEntity()->GetComponent<PostProcessing>();
		if (postProcessing)
			postProcessing->DisplayEffects(window, cameraRT, camera);


		if (camera->m_enableHDR && camera->GetRenderTexture() == nullptr)
		{
			GL_SCOPE("BLIT HDR to scene");

			RenderTexture2D::Blit(cameraRT, nullptr, bCornerX, bCornerY, tCornerX, tCornerY,
				bCornerX, bCornerY, tCornerX, tCornerY,
				BlitBitField::COLOR_BIT, BlitFilter::NEAREST);
		}

		if (displayOnMainFB)
		{
			RenderTexture2D::Blit(cameraRT, nullptr, bCornerX, bCornerY, tCornerX, tCornerY,
				bCornerX, bCornerY, tCornerX, tCornerY,
				BlitBitField::COLOR_BIT, BlitFilter::NEAREST);
		}

		GL_SCOPE("Swap camera RT");

		if (cameraRT)
		{
			cameraRT->SwapBuffer();
			cameraRT->GenerateAllMipmap(false);
			cameraRT->UnBindFramebuffer();

			glViewport(0, 0, window->GetWidth(), window->GetHeight());
		}

		GL_SCOPE("BLIT TEST");




		
	//	RenderTexture2D::SaveTextureToPNG(m_behindRefractedRTs[0]->GetTextureLayerInfo(0).m_textureID, cameraRT->GetWidth(), cameraRT->GetHeight(), "BehindTexture.png");


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

void World::RefreshRefractedRenderingComponent(MeshRendererComponent* component, bool isRefractedEnable)
{
	if (m_meshRenderers.find(component) != m_meshRenderers.end())
	{
		if (isRefractedEnable)
		{
			auto it = std::find(m_opaqueMeshRenderers.begin(), m_opaqueMeshRenderers.end(), component);
			if (it != m_opaqueMeshRenderers.end()) {
				m_opaqueMeshRenderers.erase(it);
			}

			m_refractedMeshRenderers.push_back(component);
		}
		else
		{
			auto it = std::find(m_refractedMeshRenderers.begin(), m_refractedMeshRenderers.end(), component);
			if (it != m_refractedMeshRenderers.end()) {
				m_refractedMeshRenderers.erase(it);
			}
			m_opaqueMeshRenderers.push_back(component);
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
		if (m->IsRefracted())
			m_refractedMeshRenderers.push_back(m);
		else if (m->IsBlendEnabled())
			m_transparentMeshRenderers.push_back(m);
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

		if (m->IsRefracted())
		{
			auto it = std::find(m_refractedMeshRenderers.begin(), m_refractedMeshRenderers.end(), m);
			if (it != m_refractedMeshRenderers.end()) {
				m_refractedMeshRenderers.erase(it);
			}
		}
		else if (m->IsBlendEnabled())
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

std::vector<MeshRendererComponent*> World::OrdoredTransparenceRefractedMeshRenderer(CameraComponent* camera)
{
	std::vector<MeshRendererComponent*> ordered;
	ordered.reserve(m_transparentMeshRenderers.size() + m_refractedMeshRenderers.size());

	std::unordered_set<MeshRendererComponent*> refrSet(
		m_refractedMeshRenderers.begin(), m_refractedMeshRenderers.end());
	std::unordered_set<MeshRendererComponent*> seen;
	seen.reserve(ordered.capacity());

	auto push_unique = [&](MeshRendererComponent* mr) {
		if (mr && seen.insert(mr).second) ordered.push_back(mr);
		};

	for (auto* mr : m_transparentMeshRenderers) push_unique(mr);
	for (auto* mr : m_refractedMeshRenderers) push_unique(mr);

	const Vector3D camF = camera->GetForwardVector().Normalized();
	const Vector3D camP = camera->GetWorldPosition();

	std::stable_sort(ordered.begin(), ordered.end(),
		[&](MeshRendererComponent* a, MeshRendererComponent* b)
		{
			const int pa = a->GetGeometryRenderingPriority();
			const int pb = b->GetGeometryRenderingPriority();
			if (pa != pb) return pa < pb;

			const float da = Vector3D::DotProduct(a->GetWorldPosition() - camP, camF); // distance le long de la vue
			const float db = Vector3D::DotProduct(b->GetWorldPosition() - camP, camF);
			if (da != db) return da > db; // back-to-front

			const bool aRefr = (refrSet.find(a) != refrSet.end());
			const bool bRefr = (refrSet.find(b) != refrSet.end());
			if (aRefr != bRefr) return aRefr; // réfracteur avant transparent à profondeur égale

			return a < b; // tie-breaker stable
		});

	return ordered;
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
