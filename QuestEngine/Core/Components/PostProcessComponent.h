#ifndef _COMPONENTEXAMPLE_H_
#define _COMPONENTEXAMPLE_H_

#include "../Component.h"
#include "Camera.h"
#include "../Assets/RenderTexture2D.h"
#include "../Window.h"
#include "../Assets/Material.h"
#include "../Graphics.h"
#include "../AssetsManager.h"
struct Effect
{
	Effect() = default;
	virtual void Init() = 0;
	virtual void Render(Window* window, CameraComponent* cameraComponent, RenderTexture2D* source, RenderTexture2D* renderTexturePostProcess) = 0;
};

template<typename T>
struct EffectDescription : Effect
{
	T m_data;
	Material* m_material;
	EffectDescription(std::string effectMaterialName)
	{
		m_material = AssetsManager::CreateMaterial(effectMaterialName);
	}
};

struct PostProcessingData
{
	bool m_enable = true;
};

struct GaussianBlurData : PostProcessingData
{
	enum class ControlType
	{
		RadiusPixel,
		NormalisedWidthRadius,
		NormalisedHeightRadius,	
		CustomRadiusAndSigma,
	};

	ControlType controlType = ControlType::RadiusPixel;
	int radius = 1;
	float normalisedWidthRadius = Mathf::Epsilon8;
	float normalisedHeightRadius = Mathf::Epsilon8;
	float sigma = 5.0f;
};

struct GaussianBlurEffect: EffectDescription<GaussianBlurData>
{
	GaussianBlurEffect() : EffectDescription<GaussianBlurData>("BlurMaterial")
	{

	}
	Shader* horizontalBlurShader = nullptr;
	Shader* verticalBlurShader = nullptr;

	void ComputeKernal(int radius, float sigma, std::vector<float>& kernel, int& kernelSize)
	{
		int KERNEL_MAX_SIZE = 2048;
		kernelSize = 2 * radius + 1;
		kernelSize = Mathf::Clamp(kernelSize, 0, KERNEL_MAX_SIZE);
		int kernelHalfSize = kernelSize * 0.5f;
		float sigmaSquared = sigma * sigma;
		float sum = 0;
		float E = 2.71828f;
		for (int i = -kernelHalfSize; i <= kernelHalfSize; i++)
		{
			//float gaussian = 1.0f / (sqrtf(2.0f * Mathf::PI) * sigma) * exp(-(i * i) / (2.0f * sigmaSquared)); // A changer en utlisant exponentiel
			float gaussian = 1.0f / (sqrtf(2.0f * Mathf::PI) * sigma) * powf(E, -(i * i) / (2.0f * sigmaSquared));
			kernel.push_back(gaussian);
			sum += gaussian;
		}

		float divideCoeef = 1.0f / sum;
		for (int i = 0; i < kernel.size(); i++)
		{
			kernel[i] *= divideCoeef;
		}
	}
	void ComputeRadiusAndSigma(int& radius, float& sigma, int rtWidth, int rtHeight)
	{
		if (m_data.controlType == GaussianBlurData::ControlType::CustomRadiusAndSigma)
		{
			radius = m_data.radius;
			sigma = m_data.sigma;
		}
		else
		{
			if (m_data.controlType == GaussianBlurData::ControlType::RadiusPixel)
				radius = m_data.radius;
			else if (m_data.controlType == GaussianBlurData::ControlType::NormalisedWidthRadius)
				radius = m_data.normalisedWidthRadius * rtWidth;
			else if (m_data.controlType == GaussianBlurData::ControlType::NormalisedHeightRadius)
				radius = m_data.normalisedHeightRadius * rtHeight;

			sigma = radius / 3.0f;
		}
	}
	void Init() override
	{
		horizontalBlurShader = AssetsManager::GetAsset<Shader>("HorizontalBlurShader");
		verticalBlurShader = AssetsManager::GetAsset<Shader>("VerticalBlurShader");
	}
	void Render(Window* window, CameraComponent* cameraComponent, RenderTexture2D* source, RenderTexture2D* renderTexturePostProcess) override
	{
		
		float rtWidth = renderTexturePostProcess->GetWidth();
		float rtHeight = renderTexturePostProcess->GetHeight();
		//Display Horizontal Blur Effect
		m_material->SetFloat("pixelSpacement", 1.0f / rtWidth);
		m_material->SetTexture("texture2D", renderTexturePostProcess);

		int radius = 0;
		float sigma = 0.0f;
		ComputeRadiusAndSigma(radius, sigma, rtWidth, rtHeight);
		
		if (m_currentRadius != radius || m_currentSigma != sigma)
		{
			std::vector<float> kernel;
			int kernelSize = 0;
			ComputeKernal(radius,sigma, kernel, kernelSize);
			m_material->SetFloatArray("kernel", kernel);
			m_material->SetInt("kernelHalfWidth", kernelSize/2.0f);
			m_material->SetInt("kernelWidth", kernelSize);
			
			m_currentRadius = radius;
			m_currentSigma = sigma;
		}
		
		Wrap hWrap = renderTexturePostProcess->GetHorizontalParameter();
		Wrap vWrap = renderTexturePostProcess->GetVerticalParameter();
		renderTexturePostProcess->SetWrapHorizontalParameter(Wrap::MirrorRepeat);
		renderTexturePostProcess->SetWrapVerticalParameter(Wrap::MirrorRepeat);



		Graphics::GetInstance()->RenderImage(window, source, horizontalBlurShader, m_material);	

		int viewportWidth = window->GetWidth();
		int viewportHeight = window->GetHeight();
		if (source)
		{
			viewportWidth = source->GetWidth();
			viewportHeight = source->GetHeight();
		}

		float bCornerX = cameraComponent->m_viewportBottomCornerX * viewportWidth;
		float bCornerY = cameraComponent->m_viewportBottomCornerY * viewportHeight;

		float tCornerX = cameraComponent->m_viewportTopCornerX * viewportWidth;
		float tCornerY = cameraComponent->m_viewportTopCornerY * viewportHeight;

		RenderTexture2D::Blit(source, renderTexturePostProcess, bCornerX, bCornerY, tCornerX, tCornerY, 0, 0, renderTexturePostProcess->GetWidth(), renderTexturePostProcess->GetHeight(), BlitBitField::COLOR_BIT, BlitFilter::NEAREST);

		m_material->SetFloat("pixelSpacement", 1.0f / rtHeight);
		Graphics::GetInstance()->RenderImage(window, source, verticalBlurShader, m_material);

		renderTexturePostProcess->SetWrapHorizontalParameter(hWrap);
		renderTexturePostProcess->SetWrapVerticalParameter(vWrap);
	}

private :
	int m_currentRadius = 0;
	float m_currentSigma = 5.0f;
};

struct TintEffect : EffectDescription<PostProcessingData>
{
	TintEffect() : EffectDescription<PostProcessingData>("TintMaterial")
	{

	}
	Shader* m_tintShader = nullptr;

	void Init() override
	{
		m_tintShader = AssetsManager::GetAsset<Shader>("TintShader");
	}
	void Render(Window* window, CameraComponent* cameraComponent, RenderTexture2D* source, RenderTexture2D* renderTexturePostProcess) override
	{	
		m_material->SetTexture("texture2D", renderTexturePostProcess);
		Graphics::GetInstance()->RenderImage(window, source, m_tintShader, m_material);
	}

private:
	int m_currentRadius = 0;
	float m_currentSigma = 5.0f;
};


class PostProcessComponent : public Component
{
private:
	RenderTexture2D* m_renderTexture2D;
	bool m_global = false;
	std::vector<std::shared_ptr<Effect>> m_effects;

protected:
	void Update() override;
	void Start() override;
public:
	PostProcessComponent();
	~PostProcessComponent();
	PostProcessComponent(const PostProcessComponent& postProcessComponent);
	Component* Clone()override;
	void AssignPointerAndReference()override;
	
	void Display(Window* window, CameraComponent* cameraComponent, RenderTexture2D* source);

	void AddEffect(std::shared_ptr<Effect> effect);
};

#endif
