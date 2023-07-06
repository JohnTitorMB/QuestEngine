#ifndef  _CAMERA_H_
#define _CAMERA_H_
#include "../Math/Vector2D.h"
#include "../Math/Matrix3x3.h"

class Camera
{
private :
	Vector2D m_position;
	float m_size;
	bool m_isHorizontal;
	float m_angle = 0.0f;


public:
	Camera(Vector2D position, float angle, float size, bool isHorizontal = true);
	void SetPosition(const Vector2D& position);
	void SetAngle(const float& angle);
	void SetSize(float size);
	void SetSizeType(bool isHorizontal);
	Vector2D GetVerticalAndHorizontalSize(float width, float height)const;

	Vector2D GetPosition()const;
	float GetSize()const;
	bool GetSizeType()const;
	Matrix3x3 ViewMatrix()const;
	Matrix3x3 ToNDCSpaceMatrix(float windowWidth, float windowHeight)const;
};

#endif // ! _CAMERA_H_



