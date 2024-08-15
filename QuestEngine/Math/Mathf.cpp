#include "Mathf.h"
#include <iostream>
const float Mathf::PI = 3.14159265358979323846f;
const float Mathf::RadToDeg = 57.2957795131f;
const float Mathf::DegToRad = 0.01745329252f;
const float Mathf::Epsilon7 = 1e-7f;
const float Mathf::Epsilon8 = 1e-8f;

float Mathf::Clamp(float value, float min, float max)
{
	if (value <= min)
		value = min;

	if (value >= max)
		value = max;

	return value;
}

// Loops the value t, so that it is never larger than length and never smaller than 0.
float Mathf::Repeat(float t, float length)
{
	return Clamp(t - std::floorf(t / length) * length, 0.0f, length);
}

float Mathf::Lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}

// PingPongs the value t, so that it is never larger than length and never smaller than 0.
float Mathf::PingPong(float t, float length)
{
	t = Repeat(t, length * 2.0f);
	return length - std::abs(t - length);
}