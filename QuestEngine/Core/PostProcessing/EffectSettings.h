#ifndef _EFFECT_SETTINGS_H_
#define _EFFECT_SETTINGS_H_

#include <typeindex>
#include <vector>
#include <memory>
#include <utility>

class EffectSettings
{
public:
    virtual ~EffectSettings() = default;

    virtual std::type_index GetTypeIndex() const = 0;

    virtual const char* GetTypeName() const = 0;

    virtual std::shared_ptr<EffectSettings> BlendWith(
        const std::vector<std::pair<std::shared_ptr<EffectSettings>, float>>& others
    ) const = 0;

    bool m_enable = true;
};

#endif
