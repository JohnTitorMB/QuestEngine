#ifndef  _CAMERA_H_
#define _CAMERA_H_
#include "../Math/Vector3D.h"
#include "../Math/Vector2D.h"
#include "../Math/Matrix4x4.h"

class Camera
{
private :
	Vector3D m_position;
	Vector3D m_angles;
	float m_size;
	bool m_isHorizontal;
	bool m_isPerspective;
	float m_near = 0.3f;
	float m_far = 1000.0f;

public:
	Camera(Vector3D position, Vector3D angles, float size, bool isPerspective = true, bool isHorizontal = true);
	void SetPosition(const Vector3D& position);
	void SetAngle(const Vector3D& angles);
	void SetSize(float size);
	void SetSizeType(bool isHorizontal);
	void SetNear(float near);
	void SetFar(float far);
	void SetFov(float fov);
	Vector2D GetVerticalAndHorizontalSize(float width, float height)const;
	Vector3D GetPosition()const;
	Vector3D GetAngles()const;
	float GetNear()const;
	float GetFar()const;
	float GetSize()const;
	bool GetSizeType()const;
	Matrix4x4 ViewMatrix()const;
	Matrix4x4 ProjectionMatrix(float windowWidth, float windowHeight)const;
};

#endif // ! _CAMERA_H_



