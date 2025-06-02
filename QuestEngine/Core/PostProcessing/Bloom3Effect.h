#ifndef _BLOOM3_EFFECT_H_
#define _BLOOM3_EFFECT_H_

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
class Bloom3Settings : public EffectSettings
{
public:
    int additiveBlurCount = 8;

    float intensity = 1.0f;
    float threshold = 1.3f;
    float knee = 0.5f;


    std::shared_ptr<EffectSettings> BlendWith(const std::vector<std::pair<std::shared_ptr<EffectSettings>, float>>& others) const override;
    std::type_index GetTypeIndex() const override { return typeid(Bloom3Settings); }
    const char* GetTypeName() const override { return "Bloom"; }
};

//=====================
// Bloom Effect Renderer
//=====================

class Bloom3Effect : public EffectRenderer<Bloom3Effect, Bloom3Settings> {
public:
    void Init() override;
    void Render(const RenderContext& ctx, std::shared_ptr<Bloom3Settings> settings) override;

    static const int additiveBlurMaxCount = 8;

private:
    Material* m_hdrFilterMaterial = nullptr;
    Shader* m_hdrFilterShader = nullptr;

    Material* m_boxBlurMaterial = nullptr;
    Shader* m_boxBlurShader = nullptr;

    Material* m_blendMaterial = nullptr;
    Shader* m_blendShader = nullptr;

    Material* m_upScaleMaterial = nullptr;
    Shader* m_upScaleShader = nullptr;

    RenderTexture2D* m_hdrRenderTextureA = nullptr;
    RenderTexture2D* m_hdrRenderTextureB = nullptr;
    std::vector<RenderTexture2D*> m_additiveBlurRt;

    RenderTexture2D* CreateHDRPostProcessRenderTexture();
};


#endif