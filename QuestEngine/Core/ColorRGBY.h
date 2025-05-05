#ifndef _COLORRGBY_H_
#define _COLORRGBY_H_

#include "LinearColorRGB.h"
#include "../Math/Vector4D.h"

class ColorRGBY : public LinearColorRGB {
public:
    float m_y;

    ColorRGBY();
    ColorRGBY(float r, float g, float b, float y = 1.0f, float a = 1.0f);

    Vector4D ToVector4D() const;
    void FromVector4D(const Vector4D& v);

    float Luma();
    LinearColorRGB ToLinear() const;
};

inline ColorRGBY operator*(const ColorRGBY& color, float scalar) {
    return ColorRGBY(
        color.m_r * scalar,
        color.m_g * scalar,
        color.m_b * scalar,
        color.m_y * scalar,
        color.m_alpha * scalar
    );
};

inline ColorRGBY operator*(float scalar, const ColorRGBY& color) {
    return color * scalar;
};

inline ColorRGBY& operator+=(ColorRGBY& lhs, const ColorRGBY& rhs) {
    lhs.m_r += rhs.m_r;
    lhs.m_g += rhs.m_g;
    lhs.m_b += rhs.m_b;
    lhs.m_y += rhs.m_y;
    lhs.m_alpha += rhs.m_alpha;
    return lhs;
};

inline ColorRGBY& operator*=(ColorRGBY& color, float scalar) {
    color.m_r *= scalar;
    color.m_g *= scalar;
    color.m_b *= scalar;
    color.m_y *= scalar;
    color.m_alpha *= scalar;
    return color;
};

#endif // _COLORRGBY_H_