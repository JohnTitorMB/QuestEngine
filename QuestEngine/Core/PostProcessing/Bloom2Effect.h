#ifndef _BLOOM2_EFFECT_H_
#define _BLOOM2_EFFECT_H_

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
class Bloom2Settings : public EffectSettings
{
public:
    int additiveBlurCount = 3;

    float intensity = 1.0f;
    float threshold = 1.3f;
    float knee = 0.5f;


    std::shared_ptr<EffectSettings> BlendWith(const std::vector<std::pair<std::shared_ptr<EffectSettings>, float>>& others) const override;
    std::type_index GetTypeIndex() const override { return typeid(Bloom2Settings); }
    const char* GetTypeName() const override { return "Bloom"; }
};

//=====================
// Bloom Effect Renderer
//=====================

class Bloom2Effect : public EffectRenderer<Bloom2Effect, Bloom2Settings> {
public:
    void Init() override;
    void Render(const RenderContext& ctx, std::shared_ptr<Bloom2Settings> settings) override;

    static const int additiveBlurMaxCount = 8;

private:
    Material* m_hdrFilterMaterial = nullptr;
    Shader* m_hdrFilterShader = nullptr;

    Material* m_gaussianMaterial = nullptr;
    Shader* m_gaussianShader = nullptr;

    Material* m_blendMaterial = nullptr;
    Shader* m_blendShader = nullptr;

    Material* m_additiveMaterial = nullptr;
    Shader* m_additiveShader = nullptr;

    RenderTexture2D* m_hdrRenderTextureA = nullptr;
    RenderTexture2D* m_hdrRenderTextureB = nullptr;
    std::vector<RenderTexture2D*> m_additiveBlurRt;

    RenderTexture2D* CreateHDRPostProcessRenderTexture();
    void ApplyGaussianBlur(Window* window, RenderTexture2D* source, RenderTexture2D* target, float radius, float sigma, bool autoSigma, float sigmaFactor);
    std::vector<float> ComputeKernel(int radius, float sigma);
    float Gauss(float x, float sigma);
};


#endif