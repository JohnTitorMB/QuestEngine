#include "LinearColorRGB.h"
#include "ColorManagement/ColorManagement.h"
#include "../Math/Mathf.h"

const std::shared_ptr<ColorSpace> LinearColorRGB::GetColorSpace() const {
    return ColorManagement::Instance().GetWorkingLinearSpace();
}

LinearColorRGB LinearColorRGB::Lerp(const LinearColorRGB& colorA, const LinearColorRGB& colorB, float t) {
	float r = Mathf::Lerp(colorA.m_r, colorB.m_r, t);
	float g = Mathf::Lerp(colorA.m_g, colorB.m_g, t);
	float b = Mathf::Lerp(colorA.m_b, colorB.m_b, t);
	float a = Mathf::Lerp(colorA.m_alpha, colorB.m_alpha, t);
	return LinearColorRGB(r, g, b, a);
}
