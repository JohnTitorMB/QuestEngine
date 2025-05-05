#ifndef _RENDER_CONTEXT_H_
#define _RENDER_CONTEXT_H_
#include "../Assets/RenderTexture2D.h"
#include "../Window.h"
#include "../Components/Camera.h"

class Window;
class CameraComponent;
class RenderTexture2D;

struct RenderContext
{
    Window* window = nullptr;
    RenderTexture2D* source = nullptr;
    RenderTexture2D* target = nullptr;
    CameraComponent* camera = nullptr;

    RenderContext() = default;

    RenderContext(Window* w, RenderTexture2D* src, RenderTexture2D* tgt, CameraComponent* cam)
        : window(w), source(src), target(tgt), camera(cam) {
    }
};

#endif
