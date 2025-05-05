#include "TintEffect.h"
#include "../AssetsManager.h"
#include "../Graphics.h"

std::shared_ptr<EffectSettings> TintSettings::BlendWith(const std::vector<std::pair<std::shared_ptr<EffectSettings>, float>>& others) const
{
    ColorRGB accumulatedColor = color * 1.0f;
    float totalWeight = 1.0f;

    for (const auto& [otherBase, weight] : others)
    {
        auto other = std::dynamic_pointer_cast<TintSettings>(otherBase);
        if (!other) continue;

        accumulatedColor += other->color * weight;
        totalWeight += weight;
    }

    if (totalWeight > 0.0f)
    {
        accumulatedColor /= totalWeight;
    }

    auto result = std::make_shared<TintSettings>();
    result->color = accumulatedColor;
    return result;
}

void TintEffect::Init()
{
    m_shader = AssetsManager::GetAsset<Shader>("TintShader");
    m_material = AssetsManager::CreateMaterial("TintMaterial");

    if (!m_shader || !m_material)
    {
        std::cerr << "[TintEffect] Failed to load shader or material." << std::endl;
    }
}

void TintEffect::Render(const RenderContext& ctx, std::shared_ptr<TintSettings> settings)
{
    if (!m_shader || !m_material || !settings) return;

    m_material->SetTexture("texture2D", ctx.source);
    m_material->SetColor("tintColor", settings->color);

    Graphics::GetInstance()->RenderImage(ctx.window, ctx.target, m_shader, m_material);
}
