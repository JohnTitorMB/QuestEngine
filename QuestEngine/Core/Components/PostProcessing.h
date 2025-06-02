#ifndef _POSTPROCESSING_H_
#define _POSTPROCESSING_H_

#include "../Component.h"
#include "../Window.h"
#include "Camera.h"
#include "../Assets/Material.h"
#include "../Assets/Shader.h"
#include "../AssetsManager.h"
#include "../Graphics.h"
#include "PostProcessingVolume.h"
#include "../PostProcessing/EffectSettings.h"
#include "../PostProcessing/RenderContext.h"
#include "../PostProcessing/EffectRenderer.h"

class PostProcessing : public Component {
private:
    RenderTexture2D* m_renderTexture = nullptr;
    RenderTexture2D* m_renderTextureHDR = nullptr;
    std::vector<PostProcessingVolume*> m_postProcessingVolumes;

    Vector3D WorldBaseChangementNoScale(Vector3D _point, Vector3D _targetBaseTranslation, Quaternion _targetBaseRotation)
    {
        Vector3D newPoint = _targetBaseRotation.Inverse() * (_point - _targetBaseTranslation);
        return newPoint;
    };

    inline bool IsPointOnBox(const Vector3D& point, const Vector3D& boxPosition, const Quaternion& boxRotation, const Vector3D& boxScale) {
        Vector3D localPoint = WorldBaseChangementNoScale(point, boxPosition, boxRotation);
        Vector3D halfSize = boxScale * 0.5f;

        if (localPoint.m_x < -halfSize.m_x || localPoint.m_x > halfSize.m_x) return false;
        if (localPoint.m_y < -halfSize.m_y || localPoint.m_y > halfSize.m_y) return false;
        if (localPoint.m_z < -halfSize.m_z || localPoint.m_z > halfSize.m_z) return false;

        return true;
    }

protected:
    void Update() override;
    void Start() override;

public:
    PostProcessing();
    PostProcessing(const PostProcessing& other);
    ~PostProcessing();

    Component* Clone() override;
    void AssignPointerAndReference() override;

    void AddPostProcessingVolume(PostProcessingVolume* volume);
    void DisplayEffects(Window* window, RenderTexture2D* source, CameraComponent* camera);
    void CreatePostProcessRenderTexture();
    void CreateHDRPostProcessRenderTexture();
};

#endif
