#include "Bloom3Effect.h"
#include "../AssetsManager.h"
#include "../Graphics.h"

std::shared_ptr<EffectSettings> Bloom3Settings::BlendWith(const std::vector<std::pair<std::shared_ptr<EffectSettings>, float>>& others) const
{
    float totalWeight = 1.0f;

    float sumIntensity = intensity;
    float sumThreshold = threshold;
    float sumKnee = knee;

    int sumAdditiveBlurCount = additiveBlurCount;

    auto result = std::make_shared<Bloom3Settings>();

    for (const auto& [settings, weight] : others)
    {
        auto other = std::dynamic_pointer_cast<Bloom3Settings>(settings);
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

void Bloom3Effect::Init()
{
    m_hdrFilterShader = AssetsManager::GetAsset<Shader>("HDRFilterShader");
    m_hdrFilterMaterial = AssetsManager::CreateMaterial("HDRFilterMaterial");

    m_boxBlurShader = AssetsManager::GetAsset<Shader>("BoxBlurShader");
    m_boxBlurMaterial = AssetsManager::CreateMaterial("BoxBlurMaterial");

    m_blendShader = AssetsManager::GetAsset<Shader>("BlendShader");
    m_blendMaterial = AssetsManager::CreateMaterial("BlendMaterial");

    m_hdrRenderTextureA = CreateHDRPostProcessRenderTexture();
    m_hdrRenderTextureB = CreateHDRPostProcessRenderTexture();

    m_upScaleShader = AssetsManager::GetAsset<Shader>("UpScaleShader");
    m_upScaleMaterial = AssetsManager::CreateMaterial("UpScaleMaterial");

    for (int i = 0; i < additiveBlurMaxCount; i++)
    {
        m_additiveBlurRt.push_back(CreateHDRPostProcessRenderTexture());
    }
}

void Bloom3Effect::Render(const RenderContext& ctx, std::shared_ptr<Bloom3Settings> settings)
{
    //HDR Filter Pass
    m_hdrFilterMaterial->SetTexture("texture2D", ctx.source);
    m_hdrRenderTextureA->Resize(ctx.source->GetWidth(), ctx.source->GetHeight());

    m_hdrFilterMaterial->SetFloat("threshold", settings->threshold);
    float knee = settings->knee;
    float threshold = settings->threshold;
    m_hdrFilterMaterial->SetVector3D("curve", Vector3D(threshold - knee, knee * 2.0f, 0.25f / (knee + Mathf::Epsilon8)));

    Graphics::GetInstance()->RenderImage(ctx.window, m_hdrRenderTextureA, m_hdrFilterShader, m_hdrFilterMaterial);

    //DownSampling
    int maxStep = settings->additiveBlurCount;
    for (int i = 0; i < settings->additiveBlurCount; i++)
    {
        RenderTexture2D* lastRt = i == 0 ? m_hdrRenderTextureA : m_additiveBlurRt[i-1];
        int width = lastRt->GetWidth() / 2.0f;
        int height = lastRt->GetHeight() / 2.0f;

        m_boxBlurMaterial->SetTexture("texture2D", lastRt);
        m_boxBlurMaterial->SetVector2D("pixelSpacement", Vector2D(1.0f / lastRt->GetWidth(), 1.0f / lastRt->GetHeight()));

        if (width < 2 || height < 2)
        {
            maxStep = i;
            break;
        }
        m_additiveBlurRt[i]->Resize(width, height);
        Graphics::GetInstance()->RenderImage(ctx.window, m_additiveBlurRt[i], m_boxBlurShader, m_boxBlurMaterial);
    }
    
    //UpSampling
    for (int i = maxStep - 1; i > -1; --i)
    {
        RenderTexture2D* low = m_additiveBlurRt[i];
        RenderTexture2D* high = i == 0 ? m_hdrRenderTextureA : m_additiveBlurRt[i - 1];
        m_hdrRenderTextureB->Resize(high->GetWidth(), high->GetHeight());
        m_upScaleMaterial->SetTexture("lowTexture", low);

        RenderTexture2D::Blit(high, m_hdrRenderTextureB, 0, 0, high->GetWidth(), high->GetHeight(), 0, 0, high->GetWidth(), high->GetHeight(), BlitBitField::COLOR_BIT, BlitFilter::NEAREST);
        m_upScaleMaterial->SetTexture("highTexture", m_hdrRenderTextureB);
        m_upScaleMaterial->SetVector2D("pixelSpacement", Vector2D(1.0f / low->GetWidth(), 1.0f / low->GetHeight()));

        Graphics::GetInstance()->RenderImage(ctx.window, high, m_upScaleShader, m_upScaleMaterial);
    }

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

RenderTexture2D* Bloom3Effect::CreateHDRPostProcessRenderTexture()
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