#include "ColorRGB.h"
#include "ColorManagement/ColorManagement.h"
#include "../Math/Mathf.h"

const std::shared_ptr<ColorSpace> ColorRGB::GetColorSpace() const {

    switch (colorSpaceType) {
    case ColorManagement::RGBColorSpaceType::SRGB:
        return ColorManagement::SRGB();
        break;
    case ColorManagement::RGBColorSpaceType::LinearSRGB:
        return ColorManagement::LinearSRGB();
        break;
    case ColorManagement::RGBColorSpaceType::ACEScc:
        return ColorManagement::ACEScc();
        break;
    case ColorManagement::RGBColorSpaceType::ACEScct:
        return ColorManagement::ACEScct();
        break;
    case ColorManagement::RGBColorSpaceType::LinearACEScg:
        ColorManagement::LinearACES();
        break;
    }

    return ColorManagement::Instance().GetWorkingSpace();
}

LinearColorRGB ColorRGB::ToLinear() const
{
    auto eotf = std::dynamic_pointer_cast<RGBColorSpace>(
        GetColorSpace()
    )->GetEOTF();

    float r = eotf->Decode(m_r);
    float g = eotf->Decode(m_g);
    float b = eotf->Decode(m_b);

    return LinearColorRGB(r, g, b, m_alpha);
}

ColorRGB ColorRGB::Lerp(const ColorRGB& colorA, const ColorRGB& colorB, float t) {
	float r = Mathf::Lerp(colorA.m_r, colorB.m_r, t);
	float g = Mathf::Lerp(colorA.m_g, colorB.m_g, t);
	float b = Mathf::Lerp(colorA.m_b, colorB.m_b, t);
	float a = Mathf::Lerp(colorA.m_alpha, colorB.m_alpha, t);
	return ColorRGB(r, g, b, a);
}
