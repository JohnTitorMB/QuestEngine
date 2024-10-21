#ifndef _GAUSSIAN_BLUR_H_
#define _GAUSSIAN_BLUR_H_

#include "../Components/PostProcessing.h"
#include "../Component.h"
#include "../Window.h"
#include "../Components/Camera.h"
#include "../Assets/Material.h"
#include "../Assets/Shader.h"

class GaussianBlur : public Effect
{
	Material* m_material;
	Shader* m_shader;
	float m_radius = 1.0f;
	bool autoSigma = true;
	float m_sigma = 1.0f / 3.0f;
	const float MaxRadius = 15;
	
	bool updateKernel = false;
	float m_sigmaFactor = 1.0f/3.0f;
	void UpdateKernel();
public:
	Color m_color = Color(1, 0, 0, 1);
	void Init() override;
	void DisplayEffect(Window* window, RenderTexture2D* renderTextureSource, RenderTexture2D* renderTextureTarget, CameraComponent* cameraComponent) override;

	std::vector<float> ComputeKernal(int radius, float sigma);
	float Gauss(float x, float y, float sigma);

	float GetRadius();
	void SetRadius(float radius);

	float GetSigma();
	void SetSigma(float sigma);
};

#endif

