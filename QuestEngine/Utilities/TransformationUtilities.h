#ifndef _TRANSFORMATIONUTILITIES_H_
#define _TRANSFORMATIONUTILITIES_H_
#include "../Math/Vector2D.h"
class TransformationUtilities

{
public:
	static Vector2D Identity(const Vector2D& a);
	static Vector2D Translation(const Vector2D& a, const Vector2D& t);
	static Vector2D Scale(const Vector2D& a, float s);

	static Vector2D Homothety(const Vector2D& a, const Vector2D& b, float s);
	static Vector2D PointReflection(const Vector2D& a, const Vector2D& center);
	
	static Vector2D AxialSymetry(const Vector2D& a, const Vector2D& axe);
	static Vector2D AxialSymetry(const Vector2D& a, const Vector2D& axe, const Vector2D& center);

	static Vector2D Rotation(const Vector2D& a, const float angle);
	static Vector2D Rotation(const Vector2D& a,const Vector2D& center, const float angle);

	static float PingPong(float t, float length);
	static float Repeat(float t, float length);

private:
	TransformationUtilities() {};
	static float DotProduct(const Vector2D& a, const Vector2D& b);
};
#endif // !_TRANSFORMATIONUTILITIES_H_

