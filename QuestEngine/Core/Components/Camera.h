#ifndef  _CAMERA_H_
#define _CAMERA_H_
#include "../../Math/Vector3D.h"
#include "../../Math/Vector2D.h"
#include "../../Math/Matrix4x4.h"
#include "../Component.h"

class CameraComponent : public Component
{
public :
	enum class EProjectionMode
	{
		PERSPECTIVE,
		ORTHOGRAPHIC,
	};
	CameraComponent(Vector3D position, Quaternion rotation, float size, bool isPerspective = true, bool isHorizontal = true);
	CameraComponent() {};
private :
	Vector3D m_position;
	Quaternion m_rotation;
	float m_size = 5.0f;
	bool m_isHorizontal = false;
	bool m_isPerspective = true;
	float m_near = 0.3f;
	float m_far = 1000.0f;
	
protected:
	
public:
	void SetProjectionMode(EProjectionMode _projectionMode);
	
	void SetPosition(const Vector3D& position);
	void SetRotation(const Quaternion& rotation);
	void SetSize(float size);
	void SetSizeType(bool isHorizontal);
	void SetNear(float near);
	void SetFar(float far);
	void SetFov(float fov);
	Vector2D GetVerticalAndHorizontalSize(float width, float height)const;
	Vector3D GetPosition()const;
	Quaternion GetRotation()const;
	float GetNear()const;
	float GetFar()const;
	float GetSize()const;
	bool GetSizeType()const;
	Matrix4x4 ViewMatrix()const;
	Matrix4x4 ProjectionMatrix(float windowWidth, float windowHeight)const;
};

#endif // ! _CAMERA_H_



