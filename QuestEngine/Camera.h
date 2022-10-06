#ifndef  _CAMERA_H_
#define _CAMERA_H_
#include "Vector2D.h"

class Camera
{
private :
	Vector2D m_position;
	float m_size;
	bool m_isHorizontal;


public:
	Camera(Vector2D position, float size, bool isHorizontal = true);
	void SetPosition(const Vector2D& position);
	void SetSize(float size);
	void SetSizeType(bool isHorizontal);
	Vector2D GetVerticalAndHorizontalSize(float width, float height);

	Vector2D GetPosition()const;
	float GetSize()const;
	bool GetSizeType()const;
};

#endif // ! _CAMERA_H_



