#ifndef _TINT_EFFECT_H_
#define _TINT_EFFECT_H_

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
// Tint Settings
//=====================
class TintSettings : public EffectSettings
{
public:
    ColorRGB color = ColorRGB(1, 0, 0, 1);

    std::shared_ptr<EffectSettings> BlendWith(const std::vector<std::pair<std::shared_ptr<EffectSettings>, float>>& others) const override;
    std::type_index GetTypeIndex() const override { return typeid(TintSettings); }
    const char* GetTypeName() const override { return "Tint"; }
};

//=====================
// Tint Effect Renderer
//=====================

class TintEffect : public EffectRenderer<TintEffect, TintSettings> {
public:
    void Init() override;
    void Render(const RenderContext& ctx, std::shared_ptr<TintSettings> settings) override;

private:
    Material* m_material = nullptr;
    Shader* m_shader = nullptr;
};


#endif