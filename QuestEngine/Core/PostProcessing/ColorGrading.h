// ColorGradingEffect.h
#ifndef _COLOR_GRADING_EFFECT_H_
#define _COLOR_GRADING_EFFECT_H_

#include "EffectSettings.h"
#include "EffectRenderer.h"
#include "../ColorRGBY.h"
#include "../Assets/Material.h"
#include "../Assets/Shader.h"
#include "../Graphics.h"
#include "../AssetsManager.h"
#include "../Assets/Texture2D.h"

//=====================
// Settings
//=====================
class ColorGradingSettings : public EffectSettings {
public:

    ColorGradingSettings();
    void ResetDefaults();
    struct GradingParams {
        ColorRGBY Saturation;
        ColorRGBY Contrast;
        ColorRGBY Lift;
        ColorRGBY Gain;
        ColorRGBY Midtone;
        ColorRGBY Gamma;
        ColorRGBY Offset;
        ColorRGBY Tint = ColorRGBY(1, 1, 1, 1);
        float HueShift = 0.0f;
        float MinRange = 0.0f;
        float MaxRange = 1.0f;
        float Padding = 0.0f;
    };

    GradingParams global;
    GradingParams shadow;
    GradingParams midtone;
    GradingParams highlight;

    float temperature = 0.0f;
    float temperatureTint = 0.0f;
    float exposure = 0.0f;

    // === LUT parameters ===
    bool useLUT = false;
    Texture2D* m_lutTexture = nullptr;
    float lutSize = 128.0f;

    std::shared_ptr<EffectSettings> BlendWith(const std::vector<std::pair<std::shared_ptr<EffectSettings>, float>>& others) const override;
    std::type_index GetTypeIndex() const override { return typeid(ColorGradingSettings); }
    const char* GetTypeName() const override { return "ColorGrading"; }
};

//=====================
// Effect
//=====================
class ColorGradingEffect : public EffectRenderer<ColorGradingEffect, ColorGradingSettings> {
public:
    void Init() override;
    void Render(const RenderContext& ctx, std::shared_ptr<ColorGradingSettings> settings) override;

private:

    Material* m_material = nullptr;
    Material* m_lutMaterial = nullptr;
    Shader* m_shader = nullptr;
    Shader* m_lutShader = nullptr;
    Texture2D* m_texture = nullptr;
    Texture2D* m_colorWheel = nullptr;
    RenderTexture2D* m_hdrRenderTextureA = nullptr;

    void SetGradingParams(const std::string& name, const ColorGradingSettings::GradingParams& params);
    RenderTexture2D* CreateHDRPostProcessRenderTexture();
};

#endif
