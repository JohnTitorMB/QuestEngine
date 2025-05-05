#ifndef _EFFECT_REGISTRY_H_
#define _EFFECT_REGISTRY_H_

#include <unordered_map>
#include <memory>
#include <typeindex>
#include <functional>
#include "EffectSettings.h"
#include "RenderContext.h"

class EffectRegistry {
public:
    using RenderFunction = std::function<void(const RenderContext&, std::shared_ptr<EffectSettings>)>;

    static EffectRegistry& Instance() {
        static EffectRegistry instance;
        return instance;
    }

    void Register(std::type_index type, RenderFunction fn) {
        m_registry[type] = std::move(fn);
    }

    void Render(const RenderContext& ctx, std::shared_ptr<EffectSettings> settings) const {
        auto it = m_registry.find(typeid(*settings));
        if (it != m_registry.end()) {
            it->second(ctx, std::move(settings));
        }
    }

private:
    std::unordered_map<std::type_index, RenderFunction> m_registry;

    EffectRegistry() = default;
    EffectRegistry(const EffectRegistry&) = delete;
    EffectRegistry& operator=(const EffectRegistry&) = delete;
};

#endif
