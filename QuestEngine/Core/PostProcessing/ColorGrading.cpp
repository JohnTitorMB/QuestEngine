// ColorGrading.cpp
#include "ColorGrading.h"
#include "EffectRegistry.h"
#include "../AssetsManager.h"
#include "../Graphics.h"
#include "../ColorManagement/ColorManagement.h"

void ColorGradingEffect::Init() {
    m_shader = AssetsManager::GetAsset<Shader>("ColorGradingShader");
    m_lutShader = AssetsManager::GetAsset<Shader>("LUT2DShader");
    m_material = AssetsManager::CreateMaterial("ColorGradingMaterial");
    m_lutMaterial = AssetsManager::CreateMaterial("LUT2DMaterial");
    m_colorWheel = AssetsManager::CreateTexture2D("ColorWheelTexture", "Assets/ColorWheel.png");
    m_hdrRenderTextureA = CreateHDRPostProcessRenderTexture();

}

void ColorGradingEffect::Render(const RenderContext& ctx, std::shared_ptr<ColorGradingSettings> settings) {
    if (!settings) return;

    m_hdrRenderTextureA->Resize(ctx.source->GetWidth(), ctx.source->GetHeight());

    if (settings->useLUT) {
        if (settings->m_lutTexture) {
            m_lutMaterial->SetTexture("texture2D", ctx.source);
            m_lutMaterial->SetTexture("lut", settings->m_lutTexture);
            m_lutMaterial->SetFloat("lutSize", settings->lutSize);
            Graphics::GetInstance()->RenderImage(ctx.window, m_hdrRenderTextureA, m_lutShader, m_lutMaterial);
            
        }
    }
    else
    {
        m_material->SetTexture("texture2D", ctx.source);

        SetGradingParams("Global", settings->global);
        SetGradingParams("Shadows", settings->shadow);
        SetGradingParams("Midtones", settings->midtone);
        SetGradingParams("Highlights", settings->highlight);

        m_material->SetFloat("exposure", settings->exposure);
        m_material->SetFloat("temperature", settings->temperature);
        m_material->SetFloat("temperatureTint", settings->temperatureTint);

        Graphics::GetInstance()->RenderImage(ctx.window, m_hdrRenderTextureA, m_shader, m_material);
    }



    //Blit Final result to target
    int viewportWidth = ctx.target ? ctx.target->GetWidth() : ctx.window->GetWidth();
    int viewportHeight = ctx.target ? ctx.target->GetHeight() : ctx.window->GetHeight();

    float bCornerX = ctx.camera->m_viewportBottomCornerX * viewportWidth;
    float bCornerY = ctx.camera->m_viewportBottomCornerY * viewportHeight;

    float tCornerX = ctx.camera->m_viewportTopCornerX * viewportWidth;
    float tCornerY = ctx.camera->m_viewportTopCornerY * viewportHeight;

    RenderTexture2D::Blit(m_hdrRenderTextureA, ctx.target, 0, 0, m_hdrRenderTextureA->GetWidth(), m_hdrRenderTextureA->GetHeight(),
        bCornerX, bCornerY, tCornerX, tCornerY,
        BlitBitField::COLOR_BIT, BlitFilter::NEAREST);
}
void ColorGradingEffect::SetGradingParams(const std::string& name, const ColorGradingSettings::GradingParams& p) {
    m_material->SetColorY(name + ".Saturation", p.Saturation);
    m_material->SetColorY(name + ".Contrast", p.Contrast);
    m_material->SetColorY(name + ".Lift", p.Lift);
    m_material->SetColorY(name + ".Gain", p.Gain);
    m_material->SetColorY(name + ".Midtone", p.Midtone);
    m_material->SetColorY(name + ".Gamma", p.Gamma);
    m_material->SetColorY(name + ".Offset", p.Offset);
    m_material->SetColorY(name + ".Tint", p.Tint);
    m_material->SetFloat(name + ".HueShift", p.HueShift);
    m_material->SetFloat(name + ".MinRange", p.MinRange);
    m_material->SetFloat(name + ".MaxRange", p.MaxRange);
}

ColorGradingSettings::ColorGradingSettings()
{
    ResetDefaults();
}

void ColorGradingSettings::ResetDefaults()
{
    auto resetGrading = [](GradingParams& p, float minR, float maxR) {
        p.Saturation = ColorRGBY(0, 0, 0, 1);
        p.Contrast = ColorRGBY(0, 0, 0, 1);
        p.Lift = ColorRGBY(0, 0, 0, 1);
        p.Gain = ColorRGBY(0, 0, 0, 1);
        p.Midtone = ColorRGBY(0, 0, 0, 1);
        p.Gamma = ColorRGBY(0, 0, 0, 1);
        p.Offset = ColorRGBY(0, 0, 0, 1);
        p.Tint = ColorRGBY(1, 1, 1, 1);
        p.HueShift = 0.0f;
        p.MinRange = minR;
        p.MaxRange = maxR;
        p.Padding = 0.0f;
        };

    resetGrading(global, 0.0f, 1.0f);
    resetGrading(shadow, 0.0f, 0.3f);
    resetGrading(midtone, 0.3f, 0.6f);
    resetGrading(highlight, 0.6f, 1.0f);

    temperature = 0.0f;
    temperatureTint = 0.0f;
    exposure = 0.0f;

    useLUT = false;
    m_lutTexture = nullptr;
    lutSize = 128.0f;
}


std::shared_ptr<EffectSettings> ColorGradingSettings::BlendWith(const std::vector<std::pair<std::shared_ptr<EffectSettings>, float>>& others) const {
    auto result = std::make_shared<ColorGradingSettings>();

    float totalWeight = 1.0f;
    float maxWeight = 1.0f;

    auto blendParams = [](const GradingParams& base, const std::vector<std::pair<GradingParams, float>>& inputs, float invTotalWeight) -> GradingParams {
        GradingParams result = base;
        for (const auto& [p, w] : inputs) {
            result.Saturation += p.Saturation * w;
            result.Contrast += p.Contrast * w;
            result.Lift += p.Lift * w;
            result.Gain += p.Gain * w;
            result.Midtone += p.Midtone * w;
            result.Gamma += p.Gamma * w;
            result.Offset += p.Offset * w;
            result.Tint += p.Tint * w;
            result.HueShift += p.HueShift * w;
            result.MinRange += p.MinRange * w;
            result.MaxRange += p.MaxRange * w;
        }
        result.Saturation *= invTotalWeight;
        result.Contrast *= invTotalWeight;
        result.Lift *= invTotalWeight;
        result.Gain *= invTotalWeight;
        result.Midtone *= invTotalWeight;
        result.Gamma *= invTotalWeight;
        result.Offset *= invTotalWeight;
        result.Tint *= invTotalWeight;
        result.HueShift *= invTotalWeight;
        result.MinRange *= invTotalWeight;
        result.MaxRange *= invTotalWeight;
        return result;
        };

    std::vector<std::pair<GradingParams, float>> globals, shadows, midtones, highlights;

    for (const auto& [s, weight] : others) {
        auto other = std::dynamic_pointer_cast<ColorGradingSettings>(s);
        if (!other) continue;

        globals.emplace_back(other->global, weight);
        shadows.emplace_back(other->shadow, weight);
        midtones.emplace_back(other->midtone, weight);
        highlights.emplace_back(other->highlight, weight);

        result->temperature += other->temperature * weight;
        result->temperatureTint += other->temperatureTint * weight;
        result->exposure += other->exposure * weight;

        if (weight >= maxWeight) {
            result->useLUT = other->useLUT;
            result->m_lutTexture = other->m_lutTexture;
            result->lutSize = other->lutSize;
            maxWeight = weight;
        }

        totalWeight += weight;
    }

    float invWeight = 1.0f / totalWeight;
    result->global = blendParams(global, globals, invWeight);
    result->shadow = blendParams(shadow, shadows, invWeight);
    result->midtone = blendParams(midtone, midtones, invWeight);
    result->highlight = blendParams(highlight, highlights, invWeight);
    result->temperature *= invWeight;
    result->temperatureTint *= invWeight;
    result->exposure *= invWeight;

    return result;
}

RenderTexture2D* ColorGradingEffect::CreateHDRPostProcessRenderTexture()
{
    //Post-Process RenderTexture
    RenderTexture2D* rt = AssetsManager::CreateRenderTexture2D("PostProcessHDRRenderTexture", 1, 1);
    Texture::LayerTextureInfo layerTextureInfo = Texture::LayerTextureInfo();
    layerTextureInfo.m_minificationFilter = MinificationFilter::Bilinear;
    layerTextureInfo.m_magnificationFilter = MagnificationFilter::Bilinear;
    layerTextureInfo.m_wrapHorizontalParameter = Wrap::MirrorRepeat;
    layerTextureInfo.m_wrapVerticalParameter = Wrap::MirrorRepeat;
    layerTextureInfo.m_generateMimpap = false;
    rt->AttachColorTextureBuffer(ColorRenderableFormat::RGBA16F, ColorFormat::RGBA, DataType::FLOAT, 0, layerTextureInfo);

    return rt;
}