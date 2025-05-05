#include "ColorRGBY.h"
#include "ColorManagement/ColorManagement.h"

ColorRGBY::ColorRGBY()
    : LinearColorRGB(0.0f, 0.0f, 0.0f, 1.0f), m_y(1.0f) {
}

ColorRGBY::ColorRGBY(float r, float g, float b, float y, float a)
    : LinearColorRGB(r, g, b, a), m_y(y) {
}

Vector4D ColorRGBY::ToVector4D() const {
    return Vector4D(m_r, m_g, m_b, m_y);
}

void ColorRGBY::FromVector4D(const Vector4D& v) {
    m_r = v.m_x;
    m_g = v.m_y;
    m_b = v.m_z;
    m_y = v.m_w;
}

float ColorRGBY::Luma()
{
    return m_y;
}

LinearColorRGB ColorRGBY::ToLinear() const {
    float rMapped = (m_r * 0.5f + 0.5f) * m_y;
    float gMapped = (m_g * 0.5f + 0.5f) * m_y;
    float bMapped = (m_b * 0.5f + 0.5f) * m_y;

    return LinearColorRGB(rMapped, gMapped, bMapped, m_alpha);
}
