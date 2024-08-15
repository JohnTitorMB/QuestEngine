#include "Color.h"
#include "../Math/Mathf.h"
Color::Color()
{
	m_r = 1.0f;
	m_g = 1.0f;
	m_b = 1.0f;
	m_a = 1.0f;
}

Color::Color(float r, float g, float b, float a)
{
	m_r = r;
	m_g = g;
	m_b = b;
	m_a = a;
}

Color Color::Lerp(const Color& colorA, const Color& colorB, float t)
{
	float r = Mathf::Lerp(colorA.m_r, colorB.m_r, t);
	float g = Mathf::Lerp(colorA.m_g, colorB.m_g, t);
	float b = Mathf::Lerp(colorA.m_b, colorB.m_b, t);
	float a = Mathf::Lerp(colorA.m_a, colorB.m_a, t);
	return Color(r,g,b,a);
}
