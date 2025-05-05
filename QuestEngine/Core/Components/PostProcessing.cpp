#include "PostProcessing.h"
#include "../PostProcessing/EffectRenderer.h"

PostProcessing::PostProcessing() : Component()
{
	CreatePostProcessRenderTexture();
}

PostProcessing::PostProcessing(const PostProcessing& postProcessing) : Component(postProcessing)
{
	CreatePostProcessRenderTexture();
}

void PostProcessing::Start()
{
	Component::Start();
}


void PostProcessing::Update()
{
	Component::Update();
}




PostProcessing::~PostProcessing()
{
}

Component* PostProcessing::Clone()
{	
	PostProcessing* postProcessing = new PostProcessing(*this);
	clonnedObject = postProcessing;
	clonnedObject->baseObject = this;
	return postProcessing;
}

void PostProcessing::AssignPointerAndReference()
{
	Component::AssignPointerAndReference();
    PostProcessing* basePostProcessing = (PostProcessing*)baseObject;

    if (basePostProcessing->m_postProcessingVolumes.size() > 0)
    {
        for (auto* var : basePostProcessing->m_postProcessingVolumes)
        {
            m_postProcessingVolumes.push_back((PostProcessingVolume*)var->clonnedObject);
        }
    }
}

void PostProcessing::AddPostProcessingVolume(PostProcessingVolume* volume) {
	m_postProcessingVolumes.push_back(volume);
}

void PostProcessing::DisplayEffects(Window* window, RenderTexture2D* source,CameraComponent* camera)
{
    if (!camera || !window) return;

    Vector3D camPos = camera->GetWorldPosition();

    struct WeightedEffectEntry {
        std::shared_ptr<EffectSettings> setting;
        float weight;
        int order;
    };

    std::vector<WeightedEffectEntry> allEntries;

    for (auto* volume : m_postProcessingVolumes) {
        if (!volume) continue;

        float weight = 0.0f;
        if (volume->IsGlobal()) {
             weight = 1.0f;
        }
        else {
            Vector3D boxPos = volume->GetWorldPosition();
            Vector3D boxScale = volume->GetWorldScale();
            Quaternion boxRot = volume->GetWorldRotation();

            if (IsPointOnBox(camPos, boxPos, boxRot, boxScale)) {
                weight = 1.0f; 
            }
        }

        if (weight > 0.0f) {
            const auto& effects = volume->GetEffects();
            for (int i = 0; i < effects.size(); ++i) {
                allEntries.push_back({ effects[i], weight, i });
            }
        }
    }

    // 3. Construire la pipeline de rendu blendée
    struct RuntimeEffectInstance {
        std::vector<std::pair<std::shared_ptr<EffectSettings>, float>> weightedEffects;
        float averageOrder = 0.0f;
    };

    std::vector<RuntimeEffectInstance> pipeline;

    while (!allEntries.empty()) {
        auto current = allEntries.front();
        std::type_index type = current.setting->GetTypeIndex();
        int order = current.order;
        allEntries.erase(allEntries.begin());

        RuntimeEffectInstance instance;
        instance.weightedEffects.emplace_back(current.setting, current.weight);
        instance.averageOrder = static_cast<float>(order);

        for (auto it = allEntries.begin(); it != allEntries.end(); ) {
            if (it->setting->GetTypeIndex() == type && it->order == order) {
                instance.weightedEffects.emplace_back(it->setting, it->weight);
                instance.averageOrder += static_cast<float>(it->order);
                it = allEntries.erase(it);
            }
            else {
                ++it;
            }
        }

        instance.averageOrder /= static_cast<float>(instance.weightedEffects.size());
        pipeline.push_back(instance);
    }

    // 4. Trier par ordre d'application
    std::sort(pipeline.begin(), pipeline.end(), [](const auto& a, const auto& b) {
        return a.averageOrder < b.averageOrder;
        });

    // 5. Blending
    std::vector<std::shared_ptr<EffectSettings>> blended;
    for (const auto& instance : pipeline) {
        if (instance.weightedEffects.empty()) continue;
        blended.push_back(instance.weightedEffects[0].first->BlendWith(instance.weightedEffects));
    }

    // 6. Application des effets
    int viewportWidth = window->GetWidth();
    int viewportHeight = window->GetHeight();
    if (source)
    {
        viewportWidth = source->GetWidth();
        viewportHeight = source->GetHeight();
    }

    float bCornerX = camera->m_viewportBottomCornerX * viewportWidth;
    float bCornerY = camera->m_viewportBottomCornerY * viewportHeight;

    float tCornerX = camera->m_viewportTopCornerX * viewportWidth;
    float tCornerY = camera->m_viewportTopCornerY * viewportHeight;

    m_renderTexture->Resize(tCornerX - bCornerX, tCornerY - bCornerY);



    RenderContext ctx;
    ctx.window = window;
    ctx.camera = camera;


    for (size_t i = 0; i < blended.size(); ++i) {
        ctx.source = m_renderTexture;
        ctx.target = source; 

        if (blended[i]) {

            		RenderTexture2D::Blit(source, m_renderTexture, bCornerX, bCornerY, tCornerX, tCornerY,
			0, 0, m_renderTexture->GetWidth(), m_renderTexture->GetHeight(),
			BlitBitField::COLOR_BIT, BlitFilter::NEAREST);

            EffectRegistry::Instance().Render(ctx, blended[i]);
        }
    }
}

void PostProcessing::CreatePostProcessRenderTexture()
{
	//Post-Process RenderTexture
	m_renderTexture = AssetsManager::CreateRenderTexture2D("PostProcessRenderTexture", 1, 1);
	Texture::LayerTextureInfo layerTextureInfo = Texture::LayerTextureInfo();
	layerTextureInfo.m_minificationFilter = MinificationFilter::Bilinear;
	layerTextureInfo.m_magnificationFilter = MagnificationFilter::Bilinear;

	layerTextureInfo.m_generateMimpap = false;
	m_renderTexture->AttachColorTextureBuffer(ColorRenderableFormat::RGBA8, ColorFormat::RGBA, DataType::UNSIGNED_BYTE, 0, layerTextureInfo);
}
