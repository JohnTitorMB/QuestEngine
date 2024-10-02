#include "PostProcessComponent.h"
#include "../AssetsManager.h"
PostProcessComponent::PostProcessComponent() : Component()
{
	m_renderTexture2D = AssetsManager::CreateRenderTexture2D("PostProcessRenderTexture", 1, 1);
	Texture::LayerTextureInfo layerTextureInfo = Texture::LayerTextureInfo();
	layerTextureInfo.m_minificationFilter = MinificationFilter::Bilinear;
	layerTextureInfo.m_magnificationFilter = MagnificationFilter::Bilinear;

	layerTextureInfo.m_generateMimpap = false;
	m_renderTexture2D->AttachColorTextureBuffer(ColorRenderableFormat::RGBA8, ColorFormat::RGBA, DataType::UNSIGNED_BYTE, 0, layerTextureInfo);

}

PostProcessComponent::PostProcessComponent(const PostProcessComponent& postProcessComponent) : Component(postProcessComponent)
{
	m_global = postProcessComponent.m_global;

	m_renderTexture2D = AssetsManager::CreateRenderTexture2D("PostProcessRenderTexture", 1, 1);
	Texture::LayerTextureInfo layerTextureInfo = Texture::LayerTextureInfo();
	layerTextureInfo.m_minificationFilter = MinificationFilter::Bilinear;
	layerTextureInfo.m_magnificationFilter = MagnificationFilter::Bilinear;
	layerTextureInfo.m_generateMimpap = false;

	m_renderTexture2D->AttachColorTextureBuffer(ColorRenderableFormat::RGBA8, ColorFormat::RGBA, DataType::UNSIGNED_BYTE, 0, layerTextureInfo);

	m_effects = postProcessComponent.m_effects;
}

void PostProcessComponent::Start()
{

	Component::Start();
}


void PostProcessComponent::Update()
{
	Component::Update();
}




PostProcessComponent::~PostProcessComponent()
{
}

Component* PostProcessComponent::Clone()
{
	PostProcessComponent* component = new PostProcessComponent(*this);
	clonnedObject = component;
	clonnedObject->baseObject = this;
	return component;
}

void PostProcessComponent::AssignPointerAndReference()
{
	Component::AssignPointerAndReference();
	PostProcessComponent* baseComponent = (PostProcessComponent*)baseObject;

}

void PostProcessComponent::Display(Window* window, CameraComponent* cameraComponent, RenderTexture2D* target)
{
	
	for (std::shared_ptr<Effect> effect : m_effects)
	{
		int viewportWidth = window->GetWidth();
		int viewportHeight = window->GetHeight();
		if (target)
		{
			viewportWidth = target->GetWidth();
			viewportHeight = target->GetHeight();
		}

		float bCornerX = cameraComponent->m_viewportBottomCornerX * viewportWidth;
		float bCornerY = cameraComponent->m_viewportBottomCornerY * viewportHeight;

		float tCornerX = cameraComponent->m_viewportTopCornerX * viewportWidth;
		float tCornerY = cameraComponent->m_viewportTopCornerY * viewportHeight;

		m_renderTexture2D->Resize(tCornerX - bCornerX, tCornerY - bCornerY);
		RenderTexture2D::Blit(target, m_renderTexture2D, bCornerX, bCornerY, tCornerX, tCornerY, 0, 0, m_renderTexture2D->GetWidth(), m_renderTexture2D->GetHeight(), BlitBitField::COLOR_BIT, BlitFilter::NEAREST);

		effect->Render(window, cameraComponent, target,m_renderTexture2D);
	}

	if (target == nullptr)
	{
		Graphics::GetInstance()->BindMainFrameBuffer();
	}
	else
	{
		target->BindFramebuffer();
	}
}

void PostProcessComponent::AddEffect(std::shared_ptr<Effect> effect)
{
	effect->Init();
	m_effects.push_back(effect);
}
