#include "HSVModel.h"

HSVModel::HSVModel() :
    m_h{ 0.0f }, m_s{ 0.0f }, m_v{ 0.0f }
{
}

HSVModel::HSVModel(float h, float s, float v) :
    m_h{ h }, m_s{ s }, m_v{ v }
{

}

RGBModel HSVModel::ToRGBModel() const
{
    Vector3D rgb = ToRGB();
    return RGBModel(rgb.m_x, rgb.m_y, rgb.m_z);
}

Vector3D HSVModel::ToRGB() const
{
    return ToRGB(Vector3D(m_h, m_s, m_v));
}

Vector3D HSVModel::ToRGB(Vector3D hsv)
{
    float h = hsv.m_x * 360.0f;
    float s = hsv.m_y;
    float v = hsv.m_z;

    float c = v * s;
    float x = c * (1 - std::fabs(std::fmod(h / 60.0f, 2) - 1));
    float m = v - c;

    float r = 0, g = 0, b = 0;

    if (0 <= h && h < 60) { r = c; g = x; b = 0; }
    else if (60 <= h && h < 120) { r = x; g = c; b = 0; }
    else if (120 <= h && h < 180) { r = 0; g = c; b = x; }
    else if (180 <= h && h < 240) { r = 0; g = x; b = c; }
    else if (240 <= h && h < 300) { r = x; g = 0; b = c; }
    else if (300 <= h && h < 360) { r = c; g = 0; b = x; }

    return Vector3D(r + m, g + m, b + m);
}

void HSVModel::FromEncodedRGB(const Vector3D& encodedRGB)
{
    float R = encodedRGB.m_x;
    float G = encodedRGB.m_y;
    float B = encodedRGB.m_z;

    float maxC = std::max({ R, G, B });
    float minC = std::min({ R, G, B });
    float delta = maxC - minC;

    // Hue
    if (delta == 0.0f)
        m_h = 0.0f;
    else if (maxC == R)
        m_h = fmodf((60.0f * ((G - B) / delta) + 360.0f), 360.0f);
    else if (maxC == G)
        m_h = fmodf((60.0f * ((B - R) / delta) + 120.0f), 360.0f);
    else // maxC == B
        m_h = fmodf((60.0f * ((R - G) / delta) + 240.0f), 360.0f);

    // Saturation
    m_s = (maxC == 0.0f) ? 0.0f : (delta / maxC);

    // Value
    m_v = maxC;
}