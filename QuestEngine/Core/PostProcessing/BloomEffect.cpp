#include "BloomEffect.h"
#include "../AssetsManager.h"
#include "../Graphics.h"

std::shared_ptr<EffectSettings> BloomSettings::BlendWith(const std::vector<std::pair<std::shared_ptr<EffectSettings>, float>>& others) const
{
    float totalWeight = 1.0f;

    float sumIntensity = intensity;
    float sumThreshold = threshold;
    float sumKnee = knee;

    float sumRadius = radius;
    float sumSigma = GetSigmaFinal();

    for (const auto& [settings, weight] : others)
    {
        auto other = std::dynamic_pointer_cast<BloomSettings>(settings);
        if (!other) continue;

        sumIntensity += other->intensity * weight;
        sumThreshold += other->threshold * weight;
        sumKnee += other->threshold * weight;

        sumRadius += other->radius * weight;
        sumSigma += other->GetSigmaFinal() * weight;

        totalWeight += weight;
    }

    auto result = std::make_shared<BloomSettings>();

    result->intensity = sumIntensity / totalWeight;
    result->threshold = sumThreshold / totalWeight;
    result->knee = sumKnee / totalWeight;

    result->radius = std::clamp(sumRadius / totalWeight, 1.0f, MaxRadius);
    result->sigma = sumSigma / totalWeight;
    result->autoSigma = false;

    return result;
}

void BloomEffect::Init()
{
    m_hdrFilterShader = AssetsManager::GetAsset<Shader>("HDRFilterShader");
    m_hdrFilterMaterial = AssetsManager::CreateMaterial("HDRFilterMaterial");

    m_gaussianShader = AssetsManager::GetAsset<Shader>("GaussianBlurShader");
    m_gaussianMaterial = AssetsManager::CreateMaterial("GaussianBlurMaterial");

    m_blendShader = AssetsManager::GetAsset<Shader>("BlendShader");
    m_blendMaterial = AssetsManager::CreateMaterial("BlendMaterial");

    m_hdrRenderTextureA = CreateHDRPostProcessRenderTexture();
    m_hdrRenderTextureB = CreateHDRPostProcessRenderTexture();
}

void BloomEffect::Render(const RenderContext& ctx, std::shared_ptr<BloomSettings> settings)
{
    //HDR Filter Pass
    m_hdrFilterMaterial->SetTexture("texture2D", ctx.source);
    m_hdrRenderTextureA->Resize(ctx.source->GetWidth(), ctx.source->GetHeight());

    m_hdrFilterMaterial->SetFloat("threshold", settings->threshold);
    float knee = settings->knee;
    float threshold = settings->threshold;
    m_hdrFilterMaterial->SetVector3D("curve", Vector3D(threshold - knee, knee * 2.0f, 0.25f / (knee + Mathf::Epsilon8)));

    Graphics::GetInstance()->RenderImage(ctx.window, m_hdrRenderTextureA, m_hdrFilterShader, m_hdrFilterMaterial);

    //Gaussian Blur Pass
    ApplyGaussianBlur(ctx.window, m_hdrRenderTextureA, m_hdrRenderTextureA, settings->radius, settings->sigma, settings->autoSigma, settings->sigmaFactor);

    //Blending Pass (Source + Blurred HDR)
    m_blendMaterial->SetTexture("source", ctx.source);
    m_blendMaterial->SetTexture("HDR", m_hdrRenderTextureA);
    m_blendMaterial->SetFloat("intensity", settings->intensity);
    Graphics::GetInstance()->RenderImage(ctx.window, m_hdrRenderTextureB, m_blendShader, m_blendMaterial);

    //Blit Final result to target
    int viewportWidth = ctx.target ? ctx.target->GetWidth() : ctx.window->GetWidth();
    int viewportHeight = ctx.target ? ctx.target->GetHeight() : ctx.window->GetHeight();

    float bCornerX = ctx.camera->m_viewportBottomCornerX * viewportWidth;
    float bCornerY = ctx.camera->m_viewportBottomCornerY * viewportHeight;

    float tCornerX = ctx.camera->m_viewportTopCornerX * viewportWidth;
    float tCornerY = ctx.camera->m_viewportTopCornerY * viewportHeight;

    RenderTexture2D::Blit(m_hdrRenderTextureB, ctx.target, 0, 0, m_hdrRenderTextureB->GetWidth(), m_hdrRenderTextureB->GetHeight(),
        bCornerX, bCornerY, tCornerX, tCornerY,
        BlitBitField::COLOR_BIT, BlitFilter::NEAREST);
}

void BloomEffect::ApplyGaussianBlur(Window* window, RenderTexture2D* source, RenderTexture2D* target, float radius, float _sigma, bool autoSigma, float sigmaFactor)
{
    // Horizontal pass
    float sigma = autoSigma ? radius * sigmaFactor : _sigma;

    std::vector<float> kernel = ComputeKernel((int)radius, sigma);
    m_gaussianMaterial->SetFloatArray("kernel", kernel);
    m_gaussianMaterial->SetInt("radius", (int)radius);
    m_gaussianMaterial->SetTexture("texture2D", source);
    m_gaussianMaterial->SetVector2D("pixelSpacement", Vector2D(1.0f / source->GetWidth(), 0));
    m_hdrRenderTextureB->Resize(target->GetWidth(), target->GetHeight());
    Graphics::GetInstance()->RenderImage(window, m_hdrRenderTextureB, m_gaussianShader, m_gaussianMaterial);

    m_gaussianMaterial->SetTexture("texture2D", m_hdrRenderTextureB);
    // Vertical pass
    m_gaussianMaterial->SetVector2D("pixelSpacement", Vector2D(0, 1.0f / m_hdrRenderTextureB->GetHeight()));
    Graphics::GetInstance()->RenderImage(window, target, m_gaussianShader, m_gaussianMaterial);
}

std::vector<float> BloomEffect::ComputeKernel(int radius, float sigma)
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

float BloomEffect::Gauss(float x, float sigma)
{
    float sigmaSq = sigma * sigma;
    return expf(-(x * x) / (2.0f * sigmaSq)) / (sqrtf(2.0f * Mathf::PI) * sigma);
}

RenderTexture2D* BloomEffect::CreateHDRPostProcessRenderTexture()
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