#include "TintEffect.h"

void TintEffect::Init()
{
	m_shader = AssetsManager::CreateShader("TintShader", "Assets/ScreenShader.vert", "Assets/TintShader.frag");
	m_material = AssetsManager::CreateMaterial("TintMaterial");
	m_material->SetColor("tintColor", m_color);
}

void TintEffect::DisplayEffect(Window* window, RenderTexture2D* renderTextureSource, RenderTexture2D* renderTextureTarget, CameraComponent* cameraComponent)
{
	m_material->SetTexture("texture2D", renderTextureSource);

	Graphics::GetInstance()->RenderImage(window, renderTextureTarget, m_shader, m_material);
}