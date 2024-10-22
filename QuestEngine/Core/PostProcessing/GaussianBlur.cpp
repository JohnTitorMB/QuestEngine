#include "GaussianBlur.h"
#include "../Graphics.h"
#include "../AssetsManager.h"

void GaussianBlur::Init()
{
	m_shader = AssetsManager::GetAsset<Shader>("GaussianBlurShader");
	m_material = AssetsManager::CreateMaterial("GaussianBlurMaterial");
	UpdateKernel();
}

void GaussianBlur::DisplayEffect(Window* window, RenderTexture2D* rtSource, RenderTexture2D* rtTarget, CameraComponent* camera)
{
	if (updateKernel)
		UpdateKernel();

	float rtWidth = rtSource->GetWidth();
	float rtHeight = rtSource->GetHeight();

	Wrap hWrap = rtSource->GetHorizontalParameter();
	Wrap vWrap = rtSource->GetVerticalParameter();

	rtSource->SetWrapHorizontalParameter(Wrap::MirrorRepeat);
	rtSource->SetWrapVerticalParameter(Wrap::MirrorRepeat);

	rtSource->SetMagnification(MagnificationFilter::Bilinear); //GL_LINEAR
	rtSource->SetMinification(MinificationFilter::Bilinear); //GL_LINEAR


	m_material->SetTexture("texture2D", rtSource);
	
	//Horizontal Pass
	m_material->SetVector2D("pixelSpacement", Vector2D(1.0f / rtWidth, 0));
	Graphics::GetInstance()->RenderImage(window, rtTarget, m_shader, m_material);

	//Copy target Framebuffer to postProcess Framebuffer (rtSource)
	Blit(window, rtTarget, rtSource, camera);

	//Vertical Pass
	m_material->SetVector2D("pixelSpacement", Vector2D(0, 1.0f / rtHeight));
	Graphics::GetInstance()->RenderImage(window, rtTarget, m_shader, m_material);

	rtSource->SetWrapHorizontalParameter(hWrap);
	rtSource->SetWrapVerticalParameter(vWrap);
}

void GaussianBlur::UpdateKernel()
{
	if (autoSigma)
		m_sigma = m_radius * m_sigmaFactor;

	std::vector<float> kernel = ComputeKernal(m_radius, m_sigma);
	m_material->SetFloatArray("kernel", kernel);
	int radius = (int)m_radius;
	m_material->SetInt("radius", radius);
}

std::vector<float> GaussianBlur::ComputeKernal(int radius, float sigma)
{
	std::vector<float> kernel;
	float sum = 0.0f;
	for (int i = -radius; i <= radius; i++)
	{
		float value = Gauss(i, sigma);
		kernel.push_back(value);
		sum += value;
	}

	float factor = 1.0f / sum;
	for (int i = 0; i < kernel.size(); i++)
		kernel[i] *= factor;

	return kernel;
}

float GaussianBlur::Gauss(float x, float sigma)
{
	float sigmaSQ = sigma * sigma;
	return expf(-(x * x) / (2.0f * sigmaSQ)) / (sqrtf(2.0f * Mathf::PI) * sigma);
}

float GaussianBlur::GetRadius()
{
	return m_radius;
}

void GaussianBlur::SetRadius(float radius)
{
	if (radius != m_radius)
	{
		m_radius = Mathf::Clamp(radius, 1, MaxRadius);
		updateKernel = true;
	}
}

float GaussianBlur::GetSigma()
{
	return m_sigma;
}

void GaussianBlur::SetSigma(float sigma)
{
	if (sigma != m_sigma)
	{
		m_sigma = sigma;
		updateKernel = true;
	}
}
