#ifndef _EFFECT_RENDERER_H_
#define _EFFECT_RENDERER_H_

#include "EffectRegistry.h"
#include "RenderContext.h"


template<typename TDerived, typename TSettings>
class EffectRenderer {
public:
    virtual void Init() = 0;
    virtual void Render(const RenderContext& ctx, std::shared_ptr<TSettings> settings) = 0;
    virtual ~EffectRenderer() = default;

protected:
    static void Blit(const RenderContext& ctx)
    {
        int viewportWidth = ctx.source ? ctx.source->GetWidth() : ctx.window->GetWidth();
        int viewportHeight = ctx.source ? ctx.source->GetHeight() : ctx.window->GetHeight();

        float bCornerX = ctx.camera->m_viewportBottomCornerX * viewportWidth;
        float bCornerY = ctx.camera->m_viewportBottomCornerY * viewportHeight;

        float tCornerX = ctx.camera->m_viewportTopCornerX * viewportWidth;
        float tCornerY = ctx.camera->m_viewportTopCornerY * viewportHeight;

        float targetWidth = ctx.target ? ctx.target->GetWidth() : ctx.window->GetWidth();
        float targetHeight = ctx.target ? ctx.target->GetHeight() : ctx.window->GetHeight();

        RenderTexture2D::Blit(ctx.source, ctx.target, bCornerX, bCornerY, tCornerX, tCornerY,
            0, 0, targetWidth, targetHeight,
            BlitBitField::COLOR_BIT, BlitFilter::NEAREST);
    }

private:
    struct AutoRegister {
        AutoRegister() {
            EffectRegistry::Instance().Register(
                typeid(TSettings),
                [](const RenderContext& ctx, std::shared_ptr<EffectSettings> baseSettings) {
                    static TDerived instance;
                    static bool initialized = false;
                    if (!initialized) {
                        instance.Init();
                        initialized = true;
                    }
                    instance.Render(ctx, std::static_pointer_cast<TSettings>(baseSettings));
                }
            );
        }
    };

    static inline AutoRegister s_autoRegister;
};

#endif
