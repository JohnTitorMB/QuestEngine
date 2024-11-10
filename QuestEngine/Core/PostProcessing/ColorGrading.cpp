#include "ColorGrading.h"

void ColorGrading::Init()
{
	m_shader = AssetsManager::GetAsset<Shader>("ColorGradingShader");
	m_material = AssetsManager::CreateMaterial("ColorGradingMaterial");
	m_material->SetColor("tintColor", m_color);
}

void ColorGrading::DisplayEffect(Window* window, RenderTexture2D* renderTextureSource, RenderTexture2D* renderTextureTarget, CameraComponent* cameraComponent)
{
	m_material->SetTexture("texture2D", renderTextureSource);
	Graphics::GetInstance()->RenderImage(window, renderTextureTarget, m_shader, m_material);
}