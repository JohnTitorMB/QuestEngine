#include "Camera.h"
#include <iostream>
# define M_PI           3.14159265358979323846  /* pi */

CameraComponent::CameraComponent(Vector3D position, Quaternion rotation, float size, bool isPerspective, bool isHorizontal)
{
	m_size = size;
	m_isHorizontal = isHorizontal;	
	m_isPerspective = isPerspective;
}

CameraComponent::CameraComponent(const CameraComponent& other) : SceneComponent(other)
{
	m_size = other.m_size;
	m_isHorizontal = other.m_isHorizontal;
	m_isPerspective = other.m_isPerspective;
	m_near = other.m_near;
	m_far = other.m_far;
}

Vector2D CameraComponent::GetVerticalAndHorizontalSize(float width, float height)const
{
	if (m_isHorizontal)
		return Vector2D(m_size, m_size * height / width);
	else
		return Vector2D(m_size * width / height, m_size);
}

void CameraComponent::SetProjectionMode(EProjectionMode _projectionMode)
{
	if (_projectionMode == EProjectionMode::PERSPECTIVE)
		m_isPerspective = true;
	else
		m_isPerspective = false;
}


void CameraComponent::SetSizeType(bool isHorizontal)
{
	m_isHorizontal = isHorizontal;
}

void CameraComponent::SetSize(float size)
{
	m_size = size;
}
void CameraComponent::SetNear(float near)
{
	m_near = near;
}

void CameraComponent::SetFar(float far)
{
	m_far = far;
}

void CameraComponent::SetFov(float fov)
{
	float fovInRadian = fov * M_PI / 180.0f;

	m_size = 2.0f * m_near * tan(fovInRadian / 2.0f);
}

float CameraComponent::GetSize()const
{
	return m_size;
}

bool CameraComponent::GetSizeType()const
{
	return m_isHorizontal;
}

float CameraComponent::GetNear()const
{
	return m_near;
}


float CameraComponent::GetFar()const
{
	return m_far;
}

Matrix4x4 CameraComponent::ViewMatrix()const
{
	return Matrix4x4::ScaleXYZ(Vector3D(1.0f, 1.0f,-1.0f)) * Matrix4x4::Rotate(GetWorldRotation()).Inverse() * Matrix4x4::Translate(GetWorldPosition()).Inverse();
}

Matrix4x4 CameraComponent::ProjectionMatrix(float windowWidth, float windowHeight)const
{
	Vector2D size = GetVerticalAndHorizontalSize(windowWidth, windowHeight);

	if (m_isPerspective)
		return Matrix4x4::Perspective(-size.m_x / 2.0f, size.m_x / 2.0f, -size.m_y / 2.0f, size.m_y / 2.0f, m_near, m_far);
	else
		return Matrix4x4::Orthographic(-size.m_x / 2.0f, size.m_x / 2.0f, -size.m_y / 2.0f, size.m_y / 2.0f, m_near, m_far);
}

Component* CameraComponent::Clone()
{
	CameraComponent* cameraComponent = new CameraComponent(*this);
	clonnedObject = cameraComponent;
	clonnedObject->baseObject = this;
	return cameraComponent;
}

void CameraComponent::AssignPointerAndReference()
{
	SceneComponent::AssignPointerAndReference();
}
