#ifndef _POSTPROCESSINGVOLUME_H_
#define _POSTPROCESSINGVOLUME_H_
#include "SceneComponent.h"
#include "../PostProcessing/EffectSettings.h"

class PostProcessingVolume : public SceneComponent
{
	std::vector<std::shared_ptr<EffectSettings>> m_effects;
	bool m_isGlobal = true;
	

public:
	PostProcessingVolume() = default;
	PostProcessingVolume(const PostProcessingVolume& postProcessingVolume);
	PostProcessingVolume& operator=(const PostProcessingVolume& other) = default;

	Component* Clone()override;
	void AssignPointerAndReference()override;
	void AddEffect(std::shared_ptr<EffectSettings> effect);

	bool IsGlobal()const;
	void SetGlobal(bool isGlobal);

	const std::vector<std::shared_ptr<EffectSettings>>& GetEffects()const;
};

#endif;
