#ifndef _RUNTIMEPIPELINE_H_
#define _RUNTIMEPIPELINE_H_

#include <vector>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include "Components/PostProcessing.h"

struct RuntimeEffectInstance
{
    std::vector<std::pair<std::shared_ptr<EffectSettings>, float>> weightedEffects;
    float averageOrder = 0.0f;
};

class RuntimePipelineBuilder
{
public:
    static std::vector<RuntimeEffectInstance> Build(const std::vector<PostProcessingVolume*>& volumes, const Vector3D& cameraPosition);
};

#endif