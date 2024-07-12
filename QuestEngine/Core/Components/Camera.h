#ifndef  _CAMERA_H_
#define _CAMERA_H_
#include "../../Math/Vector3D.h"
#include "../../Math/Vector2D.h"
#include "../../Math/Matrix4x4.h"
#include "../Component.h"
#include "SceneComponent.h"

class CameraComponent : public SceneComponent
{
public :
	
	enum class EProjectionMode
	{
		PERSPECTIVE,
		ORTHOGRAPHIC,
	};
	CameraComponent(Vector3D position, Quaternion rotation, float size, bool isPerspective = true, bool isHorizontal = true);
	CameraComponent() {};
	CameraComponent(const CameraComponent& other);
private :
	float m_size = 5.0f;
	bool m_isHorizontal = false;
	bool m_isPerspective = true;
	float m_near = 0.3f;
	float m_far = 1000.0f;
	
	bool m_useDepthZeroToOneProjection = false;
	bool m_useReversedZProjection = false;
	bool m_useInfiniteProjection = false;
	float m_infiniteEpisilon = 1e-6;

protected:
	
public:
	void SetProjectionMode(EProjectionMode _projectionMode);	
	void SetSize(float size);
	void SetSizeType(bool isHorizontal);
	void SetNear(float near);
	void SetFar(float far);
	void SetFov(float fov);
	
	void SetUseDepthZeroToOneProjection(bool value);
	void SetUseReversedZProjection(bool value);
	void SetUseInfiniteProjection(bool value);
	void SetInifiniteProjectionEpsilon(float value);

	Vector2D GetVerticalAndHorizontalSize(float width, float height)const;
	float GetNear()const;
	float GetFar()const;
	float GetSize()const;
	bool GetSizeType()const;
	bool GetUseDepthZeroToOneProjection();
	bool GetUseReversedZProjection();
	bool GetUseInfiniteProjection();
	float GetInifiniteProjectionEpsilon();

	Matrix4x4 ViewMatrix()const;
	Matrix4x4 ProjectionMatrix(float windowWidth, float windowHeight)const;

	Component* Clone()override;
	void AssignPointerAndReference()override;
};

#endif // ! _CAMERA_H_



