#ifndef _COLORRGB_H_
#define _COLORRGB_H_
#include "LinearColorRGB.h"

#include "ColorManagement/ColorRGBBase.h"
#include "ColorManagement/ColorManagement.h"
struct ColorRGB : public ColorRGBBase {

    using ColorRGBBase::ColorRGBBase;

    static ColorRGB Lerp(const ColorRGB& colorA, const ColorRGB& colorB, float t);
    
    ColorManagement::RGBColorSpaceType colorSpaceType = ColorManagement::RGBColorSpaceType::WorkingSpace;

    const std::shared_ptr<ColorSpace> GetColorSpace() const override;
    virtual LinearColorRGB ToLinear() const;


};

inline ColorRGB operator*(const ColorRGB& color, float scalar) {
    return ColorRGB(
        color.m_r * scalar,
        color.m_g * scalar,
        color.m_b * scalar,
        color.m_alpha * scalar
    );
}

inline ColorRGB operator*(float scalar, const ColorRGB& color) {
    return color * scalar;
}

inline ColorRGB& operator+=(ColorRGB& lhs, const ColorRGB& rhs) {
    lhs.m_r += rhs.m_r;
    lhs.m_g += rhs.m_g;
    lhs.m_b += rhs.m_b;
    lhs.m_alpha += rhs.m_alpha;
    return lhs;
}

inline ColorRGB operator/(const ColorRGB& color, float scalar) {
    return ColorRGB(
        color.m_r / scalar,
        color.m_g / scalar,
        color.m_b / scalar,
        color.m_alpha / scalar
    );
}

inline ColorRGB& operator/=(ColorRGB & lhs, float scalar) {
    lhs.m_r /= scalar;
    lhs.m_g /= scalar;
    lhs.m_b /= scalar;
    lhs.m_alpha /= scalar;
    return lhs;
}

#endif