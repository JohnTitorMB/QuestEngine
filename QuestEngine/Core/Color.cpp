#include "Color.h"

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