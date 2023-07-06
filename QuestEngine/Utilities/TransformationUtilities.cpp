#include "TransformationUtilities.h"
#include <iostream>
#include <math.h>
#include <algorithm>
# define M_PI           3.14159265358979323846  /* pi */

Vector2D TransformationUtilities::Identity(const Vector2D& a)
{
	return a;
}

Vector2D TransformationUtilities::Translation(const Vector2D& a, const Vector2D& b)
{
	return a + b;
}

Vector2D TransformationUtilities::Scale(const Vector2D& a, float s)
{
	return a * s;
}

Vector2D TransformationUtilities::Homothety(const Vector2D& a, const Vector2D& b, float s)
{
	return (a - b) * s + b;
}

Vector2D TransformationUtilities::PointReflection(const Vector2D& a, const Vector2D& center)
{
	return (center - a) * 2 + a;
}

float TransformationUtilities::DotProduct(const Vector2D& a, const Vector2D& b)
{
	return a.m_x * b.m_x + a.m_y * b.m_y;
}

Vector2D TransformationUtilities::AxialSymetry(const Vector2D& a, const Vector2D& axe)
{
	float axeMagitude = axe.Magnitude();
	return a + (axe * (DotProduct(a, axe) / (axeMagitude * axeMagitude)) - a) * 2.0f;
}

Vector2D TransformationUtilities::AxialSymetry(const Vector2D& a, const Vector2D& axe, const Vector2D& center)
{
	float axeMagitude = axe.Magnitude();
	return a + (axe * (DotProduct(a - center, axe) / (axeMagitude * axeMagitude)) - (a - center)) * 2.0f;
}

Vector2D TransformationUtilities::Rotation(const Vector2D& a, const float angle)
{
	float angleDegToRad = angle * M_PI / 180;
	float x = a.m_x * cos(angleDegToRad) - a.m_y * sin(angleDegToRad);
	float y = a.m_y * cos(angleDegToRad) + a.m_x * sin(angleDegToRad);
	return Vector2D(x, y);
}

Vector2D TransformationUtilities::Rotation(const Vector2D& a, const Vector2D& center, const float angle)
{
	float angleDegToRad = angle * M_PI / 180;
	float x = center.m_x + (a.m_x - center.m_x) * cos(angleDegToRad) - (a.m_y - center.m_y) * sin(angleDegToRad);
	float y = center.m_y + (a.m_y - center.m_y) * cos(angleDegToRad) + (a.m_x - center.m_x) * sin(angleDegToRad);
	return Vector2D(x, y);
}

// Loops the value t, so that it is never larger than length and never smaller than 0.
float TransformationUtilities::Repeat(float t, float length)
{
	return std::clamp(t - std::floorf(t / length) * length, 0.0f, length);
}

// PingPongs the value t, so that it is never larger than length and never smaller than 0.
float TransformationUtilities::PingPong(float t, float length)
{
	t = Repeat(t, length * 2.0f);
	return length - std::abs(t - length);
}