#include "CustomColorRGB.h"
#include "../Math/Mathf.h"

CustomColorRGB::CustomColorRGB(std::shared_ptr<ColorSpace> colorSpace, float r, float g, float b, float a)
    : m_colorSpace(colorSpace)
{
    m_r = r;
    m_g = g;
    m_b = b;
    m_alpha = a;
}

const std::shared_ptr<ColorSpace> CustomColorRGB::GetColorSpace() const {
    return m_colorSpace;
}
