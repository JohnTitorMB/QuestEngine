#include "RGBModel.h"
#include "../../Math/Mathf.h"
RGBModel::RGBModel() :
	m_r{ 0.0f }, m_g{ 0.0f }, m_b{ 0.0f }
{
}

RGBModel::RGBModel(float r, float g, float b) :
	m_r { r }, m_g { g }, m_b { b }
{

}

RGBModel::RGBModel(const Vector3D& rgb) :
	m_r{ rgb.m_x }, m_g{ rgb.m_y }, m_b{ rgb.m_z }
{

}

Vector3D RGBModel::GetRGB() const
{
	return Vector3D(m_r, m_g, m_b);
}

Vector3D RGBModel::ToHSV(Vector3D rgb)
{
    float r = rgb.m_x;
    float g = rgb.m_y;
    float b = rgb.m_z;

    float max = Mathf::Max(r, g, b);
    float min = Mathf::Min(r, g, b);
    float delta = max - min;

    float h = 0.0f;
    float s = (max == 0.0f) ? 0.0f : delta / max;
    float v = max;

    if (delta > 0.0f) {
        if (max == r)
            h = fmodf((g - b) / delta, 6.0f);
        else if (max == g)
            h = ((b - r) / delta) + 2.0f;
        else
            h = ((r - g) / delta) + 4.0f;

        h /= 6.0f;
        if (h < 0.0f) h += 1.0f;
    }

    return Vector3D(h, s, v);
}
