#include "Bloom2Effect.h"
#include "../AssetsManager.h"
#include "../Graphics.h"

std::shared_ptr<EffectSettings> Bloom2Settings::BlendWith(const std::vector<std::pair<std::shared_ptr<EffectSettings>, float>>& others) const
{
    float totalWeight = 1.0f;

    float sumIntensity = intensity;
    float sumThreshold = threshold;
    float sumKnee = knee;

    int sumAdditiveBlurCount = additiveBlurCount;

    auto result = std::make_shared<Bloom2Settings>();

    for (const auto& [settings, weight] : others)
    {
        auto other = std::dynamic_pointer_cast<Bloom2Settings>(settings);
        if (!other) continue;

        sumIntensity += other->intensity * weight;
        sumThreshold += other->threshold * weight;
        sumKnee += other->threshold * weight;
        sumAdditiveBlurCount += other->additiveBlurCount * weight;

        totalWeight += weight;
    }

    result->intensity = sumIntensity / totalWeight;
    result->threshold = sumThreshold / totalWeight;
    result->knee = sumKnee / totalWeight;
    result->additiveBlurCount = sumAdditiveBlurCount / totalWeight;

    return result;
}

void Bloom2Effect::Init()
{
    m_hdrFilterShader = AssetsManager::GetAsset<Shader>("HDRFilterShader");
    m_hdrFilterMaterial = AssetsManager::CreateMaterial("HDRFilterMaterial");

    m_gaussianShader = AssetsManager::GetAsset<Shader>("GaussianBlurShader");
    m_gaussianMaterial = AssetsManager::CreateMaterial("GaussianBlurMaterial");

    m_blendShader = AssetsManager::GetAsset<Shader>("BlendShader");
    m_blendMaterial = AssetsManager::CreateMaterial("BlendMaterial");

    m_hdrRenderTextureA = CreateHDRPostProcessRenderTexture();
    m_hdrRenderTextureB = CreateHDRPostProcessRenderTexture();

    m_additiveShader = AssetsManager::GetAsset<Shader>("AdditiveShader");
    m_additiveMaterial = AssetsManager::CreateMaterial("AdditiveMaterial");

    for (int i = 0; i < additiveBlurMaxCount; i++)
    {
        m_additiveBlurRt.push_back(CreateHDRPostProcessRenderTexture());
    }
}

void Bloom2Effect::Render(const RenderContext& ctx, std::shared_ptr<Bloom2Settings> settings)
{
    //HDR Filter Pass
    m_hdrFilterMaterial->SetTexture("texture2D", ctx.source);
    m_hdrRenderTextureA->Resize(ctx.source->GetWidth(), ctx.source->GetHeight());

    m_hdrFilterMaterial->SetFloat("threshold", settings->threshold);
    float knee = settings->knee;
    float threshold = settings->threshold;
    m_hdrFilterMaterial->SetVector3D("curve", Vector3D(threshold - knee, knee * 2.0f, 0.25f / (knee + Mathf::Epsilon8)));

    Graphics::GetInstance()->RenderImage(ctx.window, m_hdrRenderTextureA, m_hdrFilterShader, m_hdrFilterMaterial);

    //Apply Blur to multiple RenderTexture
    for (int i = 0; i < settings->additiveBlurCount; i++)
    {
        float radius = (i == 0) ? 2.0f : 5.0f * std::pow(2.0f, i);

        m_additiveBlurRt[i]->Resize(m_hdrRenderTextureA->GetWidth(), m_hdrRenderTextureA->GetHeight());

        ApplyGaussianBlur(ctx.window, m_hdrRenderTextureA, m_additiveBlurRt[i], radius, 0.0f, true, 1.0f / 3.0f);
    }

    //Merge Blurred Images
    m_hdrRenderTextureB->Resize(m_hdrRenderTextureA->GetWidth(), m_hdrRenderTextureA->GetHeight());
    for (int i = settings->additiveBlurCount - 1; i > 0; i--)
    {
        RenderTexture2D* low = m_additiveBlurRt[i];
        RenderTexture2D* high = m_additiveBlurRt[i - 1];
        m_additiveMaterial->SetTexture("lowTexture", low);
        RenderTexture2D::Blit(high, m_hdrRenderTextureB, 0, 0, high->GetWidth(), high->GetHeight(), 0, 0, high->GetWidth(), high->GetHeight(), BlitBitField::COLOR_BIT, BlitFilter::NEAREST);

        m_additiveMaterial->SetTexture("highTexture", m_hdrRenderTextureB);
        Graphics::GetInstance()->RenderImage(ctx.window, m_additiveBlurRt[i - 1], m_additiveShader, m_additiveMaterial);
    }

    //Blending Pass (Source + Blurred HDR)
    m_blendMaterial->SetTexture("source", ctx.source);
    m_blendMaterial->SetTexture("HDR", m_additiveBlurRt[0]);
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

void Bloom2Effect::ApplyGaussianBlur(Window* window, RenderTexture2D* source, RenderTexture2D* target, float radius, float _sigma, bool autoSigma, float sigmaFactor)
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

std::vector<float> Bloom2Effect::ComputeKernel(int radius, float sigma)
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

float Bloom2Effect::Gauss(float x, float sigma)
{
    float sigmaSq = sigma * sigma;
    return expf(-(x * x) / (2.0f * sigmaSq)) / (sqrtf(2.0f * Mathf::PI) * sigma);
}

RenderTexture2D* Bloom2Effect::CreateHDRPostProcessRenderTexture()
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