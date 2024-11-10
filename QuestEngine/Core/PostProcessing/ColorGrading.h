#ifndef _COLOR_GRADING_H_
#define _COLOR_GRADING_H_

#include "../Components/PostProcessing.h"
#include "../Component.h"
#include "../Window.h"
#include "../Components/Camera.h"
#include "../Assets/Material.h"
#include "../Assets/Shader.h"
#include "../AssetsManager.h"
#include "../Graphics.h"

class ColorGrading : public Effect
{
	Material* m_material;
	Shader* m_shader;
public:
	Color m_color = Color(1, 0, 0, 1);
	void Init() override;
	void DisplayEffect(Window* window, RenderTexture2D* renderTextureSource, RenderTexture2D* renderTextureTarget, CameraComponent* cameraComponent) override;
};

#endif