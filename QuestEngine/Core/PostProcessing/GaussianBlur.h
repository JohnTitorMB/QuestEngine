#ifndef _GAUSSIAN_BLUR_EFFECT_H_
#define _GAUSSIAN_BLUR_EFFECT_H_

#include "../Components/PostProcessing.h"
#include "../Component.h"
#include "../Window.h"
#include "../Components/Camera.h"
#include "../Assets/Material.h"
#include "../Assets/Shader.h"
#include "../AssetsManager.h"
#include "../Graphics.h"
#include "EffectSettings.h"
#include "EffectRenderer.h"
#include "../../Math/Mathf.h"

//=====================
// Gaussian Blur Settings
//=====================
class GaussianBlurSettings : public EffectSettings
{
public:
    float radius = 1.0f;
    float sigma = 1.0f / 3.0f;
    bool autoSigma = true;
    float sigmaFactor = 1.0f / 3.0f;
    const float MaxRadius = 513.0f;

    float GetSigmaFinal() const {
        return autoSigma ? radius * sigmaFactor : sigma;
    }

    std::shared_ptr<EffectSettings> BlendWith(const std::vector<std::pair<std::shared_ptr<EffectSettings>, float>>& others) const override;

    std::type_index GetTypeIndex() const override { return typeid(GaussianBlurSettings); }
    const char* GetTypeName() const override { return "GaussianBlur"; }
};

//=====================
// Gaussian Blur Effect
//=====================
class GaussianBlurEffect : public EffectRenderer<GaussianBlurEffect, GaussianBlurSettings>
{
public:
    void Init() override;
    void Render(const RenderContext& ctx, std::shared_ptr<GaussianBlurSettings> settings) override;

private:
    std::vector<float> ComputeKernel(int radius, float sigma);
    float Gauss(float x, float sigma);

    Material* m_material = nullptr;
    Shader* m_shader = nullptr;
};

#endif
