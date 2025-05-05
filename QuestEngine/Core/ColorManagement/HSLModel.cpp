#include "HSLModel.h"
#include <algorithm>
#include <cmath>

HSLModel::HSLModel() : 
	m_h{ 0.0f }, m_s{ 0.0f }, m_l{ 0.0f }
{
}

HSLModel::HSLModel(float h, float s, float l) : 
	m_h{ h }, m_s{ s }, m_l{ l } 
{

}

static float HueToRGB(float p, float q, float t)
{
	if (t < 0.0f) t += 1.0f;
	if (t > 1.0f) t -= 1.0f;
	if (t < 1.0f / 6.0f) return p + (q - p) * 6.0f * t;
	if (t < 1.0f / 2.0f) return q;
	if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
	return p;
}

Vector3D HSLModel::ToRGB() const
{
	float h = m_h / 360.0f;
	float s = m_s;
	float l = m_l;

	float r, g, b;

	if (s == 0.0f) {
		r = g = b = l; // achromatic
	}
	else {
		float q = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
		float p = 2.0f * l - q;
		r = HueToRGB(p, q, h + 1.0f / 3.0f);
		g = HueToRGB(p, q, h);
		b = HueToRGB(p, q, h - 1.0f / 3.0f);
	}

	return Vector3D(r, g, b);
}

RGBModel HSLModel::ToRGBModel() const
{
	return RGBModel(ToRGB());
}

void HSLModel::FromEncodedRGB(const Vector3D& encodedRGB)
{
	float r = encodedRGB.m_x;
	float g = encodedRGB.m_y;
	float b = encodedRGB.m_z;

	float maxC = std::max({ r, g, b });
	float minC = std::min({ r, g, b });
	float delta = maxC - minC;

	m_l = (maxC + minC) / 2.0f;

	if (delta == 0.0f) {
		m_h = 0.0f;
		m_s = 0.0f;
	}
	else {
		m_s = m_l < 0.5f
			? delta / (maxC + minC)
			: delta / (2.0f - maxC - minC);

		if (maxC == r)
			m_h = fmodf((60.0f * ((g - b) / delta) + 360.0f), 360.0f);
		else if (maxC == g)
			m_h = fmodf((60.0f * ((b - r) / delta) + 120.0f), 360.0f);
		else // maxC == b
			m_h = fmodf((60.0f * ((r - g) / delta) + 240.0f), 360.0f);
	}
}
