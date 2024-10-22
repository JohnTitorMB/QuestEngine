#include "PostProcessing.h"


PostProcessing::PostProcessing() : Component()
{
	CreatePostProcessRenderTexture();
}

PostProcessing::PostProcessing(const PostProcessing& postProcessing) : Component(postProcessing)
{
	CreatePostProcessRenderTexture();
	m_effects = postProcessing.m_effects;
}

void PostProcessing::Start()
{
	Component::Start();
}


void PostProcessing::Update()
{
	Component::Update();
}




PostProcessing::~PostProcessing()
{
}

Component* PostProcessing::Clone()
{	
	PostProcessing* postProcessing = new PostProcessing(*this);
	clonnedObject = postProcessing;
	clonnedObject->baseObject = this;
	return postProcessing;
}

void PostProcessing::AssignPointerAndReference()
{
	Component::AssignPointerAndReference();
}

void PostProcessing::DisplayEffects(Window* window, RenderTexture2D* renderTextureSource,CameraComponent* cameraComponent)
{
	int viewportWidth = window->GetWidth();
	int viewportHeight = window->GetHeight();
	if (renderTextureSource)
	{
		viewportWidth = renderTextureSource->GetWidth();
		viewportHeight = renderTextureSource->GetHeight();
	}
	
	float bCornerX = cameraComponent->m_viewportBottomCornerX * viewportWidth;
	float bCornerY = cameraComponent->m_viewportBottomCornerY * viewportHeight;

	float tCornerX = cameraComponent->m_viewportTopCornerX * viewportWidth;
	float tCornerY = cameraComponent->m_viewportTopCornerY * viewportHeight;

	m_renderTexture->Resize(tCornerX - bCornerX, tCornerY - bCornerY);


	for (std::shared_ptr<Effect> effect : m_effects)
	{
		if (effect->m_enable)
		{
			RenderTexture2D::Blit(renderTextureSource, m_renderTexture, bCornerX, bCornerY, tCornerX, tCornerY,
				0, 0, m_renderTexture->GetWidth(), m_renderTexture->GetHeight(),
				BlitBitField::COLOR_BIT, BlitFilter::NEAREST);

			effect->DisplayEffect(window, m_renderTexture, renderTextureSource, cameraComponent);
		}
	}
}

void PostProcessing::CreatePostProcessRenderTexture()
{
	//Post-Process RenderTexture
	m_renderTexture = AssetsManager::CreateRenderTexture2D("PostProcessRenderTexture", 1, 1);
	Texture::LayerTextureInfo layerTextureInfo = Texture::LayerTextureInfo();
	layerTextureInfo.m_minificationFilter = MinificationFilter::Bilinear;
	layerTextureInfo.m_magnificationFilter = MagnificationFilter::Bilinear;

	layerTextureInfo.m_generateMimpap = false;
	m_renderTexture->AttachColorTextureBuffer(ColorRenderableFormat::RGBA8, ColorFormat::RGBA, DataType::UNSIGNED_BYTE, 0, layerTextureInfo);
}

void PostProcessing::AddEffect(std::shared_ptr<Effect> effect)
{
	effect->Init();
	m_effects.push_back(effect);
}

void Effect::Blit(Window* window, RenderTexture2D* rtFrom, RenderTexture2D* rtTo, CameraComponent* camera)
{
	int viewportWidth = rtFrom ? rtFrom->GetWidth() : window->GetWidth();
	int viewportHeight = rtFrom ? rtFrom->GetHeight() : window->GetHeight();

	float bCornerX = camera->m_viewportBottomCornerX * viewportWidth;
	float bCornerY = camera->m_viewportBottomCornerY * viewportHeight;

	float tCornerX = camera->m_viewportTopCornerX * viewportWidth;
	float tCornerY = camera->m_viewportTopCornerY * viewportHeight;

	RenderTexture2D::Blit(rtFrom, rtTo, bCornerX, bCornerY, tCornerX, tCornerY,
		0, 0, rtTo->GetWidth(), rtTo->GetHeight(),
		BlitBitField::COLOR_BIT, BlitFilter::NEAREST);
}
