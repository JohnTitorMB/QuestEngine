#ifndef _BLOOM_EFFECT_H_
#define _BLOOM_EFFECT_H_

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

//=====================
// Blomm Settings
//=====================
class BloomSettings : public EffectSettings
{
public:
    float radius = 1.0f;
    float sigma = 1.0f / 3.0f;
    bool autoSigma = true;
    float sigmaFactor = 1.0f / 3.0f;
    const float MaxRadius = 513.0f;

    float intensity = 1.0f;
    float threshold = 1.3f;
    float knee = 0.5f;

    float GetSigmaFinal() const {
        return autoSigma ? radius * sigmaFactor : sigma;
    }

    std::shared_ptr<EffectSettings> BlendWith(const std::vector<std::pair<std::shared_ptr<EffectSettings>, float>>& others) const override;
    std::type_index GetTypeIndex() const override { return typeid(BloomSettings); }
    const char* GetTypeName() const override { return "Bloom"; }
};

//=====================
// Bloom Effect Renderer
//=====================

class BloomEffect : public EffectRenderer<BloomEffect, BloomSettings> {
public:
    void Init() override;
    void Render(const RenderContext& ctx, std::shared_ptr<BloomSettings> settings) override;

private:
    Material* m_hdrFilterMaterial = nullptr;
    Shader* m_hdrFilterShader = nullptr;

    Material* m_gaussianMaterial = nullptr;
    Shader* m_gaussianShader = nullptr;

    Material* m_blendMaterial = nullptr;
    Shader* m_blendShader = nullptr;

    RenderTexture2D* m_hdrRenderTextureA = nullptr;
    RenderTexture2D* m_hdrRenderTextureB = nullptr;

    RenderTexture2D* CreateHDRPostProcessRenderTexture();
    void ApplyGaussianBlur(Window* window, RenderTexture2D* source, RenderTexture2D* target, float radius, float sigma, bool autoSigma, float sigmaFactor);
    std::vector<float> ComputeKernel(int radius, float sigma);
    float Gauss(float x, float sigma);
};


#endif