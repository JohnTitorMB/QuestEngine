#ifndef _POSTPROCESSING_H_
#define _POSTPROCESSING_H_

#include "../Component.h"
#include "../Window.h"
#include "Camera.h"
#include "../Assets/Material.h"
#include "../Assets/Shader.h"
#include "../AssetsManager.h"
#include "../Graphics.h"

class Effect
{
protected:
	void Blit(Window* window, RenderTexture2D* rtSource, RenderTexture2D* rtTarget, CameraComponent* camera);
public:
	virtual void DisplayEffect(Window* window, RenderTexture2D* rtFrom, RenderTexture2D* rtTo, CameraComponent* camera) = 0;
	virtual void Init() = 0;

	bool m_enable = true;
};



class PostProcessing : public Component
{
private:
	RenderTexture2D* m_renderTexture;
	std::vector<std::shared_ptr<Effect>> m_effects;
protected:
	void Update() override;
	void Start() override;
public:
	PostProcessing();
	PostProcessing(const PostProcessing& postProcessing);
	~PostProcessing();
	Component* Clone()override;
	void AssignPointerAndReference()override;
	void DisplayEffects(Window* window, RenderTexture2D* renderTextureSource, CameraComponent* cameraComponent);
	void CreatePostProcessRenderTexture();
	void AddEffect(std::shared_ptr<Effect> effect);
};

#endif

