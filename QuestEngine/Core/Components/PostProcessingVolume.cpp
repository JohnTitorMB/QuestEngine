#include "PostProcessingVolume.h"
#include "PostProcessing.h"
PostProcessingVolume::PostProcessingVolume(const PostProcessingVolume& postProcessingVolume)
{
	m_effects = postProcessingVolume.m_effects;
	m_isGlobal = postProcessingVolume.m_isGlobal;
}
Component* PostProcessingVolume::Clone()
{
	PostProcessingVolume* postProcessingVolume = new PostProcessingVolume(*this);
	clonnedObject = postProcessingVolume;
	clonnedObject->baseObject = this;
	return postProcessingVolume;
}

void PostProcessingVolume::AssignPointerAndReference()
{
	SceneComponent::AssignPointerAndReference();
}

void PostProcessingVolume::AddEffect(std::shared_ptr<EffectSettings> effect)
{
	m_effects.push_back(effect);
}

bool PostProcessingVolume::IsGlobal()const
{
	return m_isGlobal;
}

void PostProcessingVolume::SetGlobal(bool isGlobal)
{
	m_isGlobal = isGlobal;
}

const std::vector<std::shared_ptr<EffectSettings>>& PostProcessingVolume::GetEffects() const
{
	return m_effects;
}
