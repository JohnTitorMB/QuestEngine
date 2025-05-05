#include "RuntimePipeline.h"

std::vector<RuntimeEffectInstance> RuntimePipelineBuilder::Build(const std::vector<PostProcessingVolume*>& volumes, const Vector3D& cameraPosition)
{
    struct OrderedEntry
    {
        std::shared_ptr<EffectSettings> effect;
        float weight;
        int order;
    };

    std::vector<OrderedEntry> allEntries;

    for (auto* volume : volumes)
    {
        if (!volume) continue;
        const auto& effects = volume->GetEffects();

        float weight = 1.0f;
        if (!volume->IsGlobal()) {
            float distance = (cameraPosition - volume->GetWorldPosition()).Magnitude();
            float radius = volume->GetWorldScale().Magnitude();
            weight = std::clamp(1.0f - distance / radius, 0.0f, 1.0f);
        }

        for (int i = 0; i < effects.size(); ++i)
        {
            if (effects[i] && effects[i]->m_enable)
            {
                allEntries.push_back({ effects[i], weight, i });
            }
        }
    }

    // Group by effect instance order + type (using typeid)
    std::vector<RuntimeEffectInstance> runtimePipeline;

    while (!allEntries.empty())
    {
        auto current = allEntries.front();
        std::type_index currentType = typeid(*current.effect);
        allEntries.erase(allEntries.begin());

        RuntimeEffectInstance instance;
        instance.weightedEffects.emplace_back(current.effect, current.weight);
        instance.averageOrder = static_cast<float>(current.order);

        for (auto it = allEntries.begin(); it != allEntries.end();)
        {
            if (std::type_index(typeid(*it->effect)) == currentType && it->order == current.order)
            {
                instance.weightedEffects.emplace_back(it->effect, it->weight);
                instance.averageOrder += static_cast<float>(it->order);
                it = allEntries.erase(it);
            }
            else ++it;
        }

        instance.averageOrder /= static_cast<float>(instance.weightedEffects.size());
        runtimePipeline.push_back(instance);
    }

    std::sort(runtimePipeline.begin(), runtimePipeline.end(), [](const RuntimeEffectInstance& a, const RuntimeEffectInstance& b) {
        return a.averageOrder < b.averageOrder;
        });

    return runtimePipeline;
}