#include "GaussianBlur.h"


std::shared_ptr<EffectSettings> GaussianBlurSettings::BlendWith(const std::vector<std::pair<std::shared_ptr<EffectSettings>, float>>& others) const {
    float totalWeight = 1.0f;
    float sumRadius = radius;
    float sumSigma = GetSigmaFinal();

    for (const auto& [settings, weight] : others)
    {
        auto other = std::dynamic_pointer_cast<GaussianBlurSettings>(settings);
        if (!other) continue;

        sumRadius += other->radius * weight;
        sumSigma += other->GetSigmaFinal() * weight;
        totalWeight += weight;
    }

    auto result = std::make_shared<GaussianBlurSettings>();
    result->radius = std::clamp(sumRadius / totalWeight, 1.0f, MaxRadius);
    result->sigma = sumSigma / totalWeight;
    result->autoSigma = false;

    return result;
}

void GaussianBlurEffect::Init()
{
    m_shader = AssetsManager::GetAsset<Shader>("GaussianBlurShader");
    m_material = AssetsManager::CreateMaterial("GaussianBlurMaterial");
}

void GaussianBlurEffect::Render(const RenderContext& ctx, std::shared_ptr<GaussianBlurSettings> settings)
{
    float radius = settings->radius;
    float sigma = settings->GetSigmaFinal();

    std::vector<float> kernel = ComputeKernel((int)radius, sigma);
    m_material->SetFloatArray("kernel", kernel);
    m_material->SetInt("radius", (int)radius);
    m_material->SetTexture("texture2D", ctx.source);

    // Horizontal pass
    m_material->SetVector2D("pixelSpacement", Vector2D(1.0f / ctx.source->GetWidth(), 0));
    Graphics::GetInstance()->RenderImage(ctx.window, ctx.target, m_shader, m_material);

    RenderContext ctx2;
    ctx2.window = ctx.window;
    ctx2.camera = ctx.camera;
    ctx2.source = ctx.target;
    ctx2.target = ctx.source;

    // Copy back to source
    Blit(ctx2);

    // Vertical pass
    m_material->SetVector2D("pixelSpacement", Vector2D(0, 1.0f / ctx.source->GetHeight()));
    Graphics::GetInstance()->RenderImage(ctx.window, ctx.target, m_shader, m_material);
}

std::vector<float> GaussianBlurEffect::ComputeKernel(int radius, float sigma)
{
    std::vector<float> kernel;
    float sum = 0.0f;

    for (int i = -radius; i <= radius; i++)
    {
        float value = Gauss(i, sigma);
        kernel.push_back(value);
        sum += value;
    }

    float invSum = 1.0f / sum;
    for (float& k : kernel)
        k *= invSum;

    return kernel;
}

float GaussianBlurEffect::Gauss(float x, float sigma)
{
    float sigmaSq = sigma * sigma;
    return expf(-(x * x) / (2.0f * sigmaSq)) / (sqrtf(2.0f * Mathf::PI) * sigma);
}
