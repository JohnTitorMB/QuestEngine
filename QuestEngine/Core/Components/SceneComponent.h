#ifndef _SCENECOMPONENT_H_
#define _SCENECOMPONENT_H_
#include <set>
#include "../../Math/Vector3D.h"
#include "../../Math/Quaternion.h"
#include "Transform.h"
#include "../Component.h"

class SceneComponent : public Component
{
private:
	Vector3D m_relativePosition;
	Quaternion m_relativeRotation;
	Vector3D m_relativeScale;
	
	SceneComponent* m_parent;
	std::set<SceneComponent*> m_childs;
	Transform m_transform;
	void MoveChildWorldPosition(Vector3D delta);
	void RefreshChildWorldPosition();
	void RefreshChildWorldRotation();
	void RefreshChildWorldScale();

public:
	SceneComponent();
	SceneComponent(const SceneComponent& sceneComponent);
	void SetParent(SceneComponent* sceneComponent);
	bool IsChildExist(SceneComponent* childSceneComponent);
	const Transform& GetTransform()const;

	void SetRelativePosition(const Vector3D& relativePosition);
	void SetRelativeRotation(const Quaternion& relativeRotation);
	void SetRelativeScale(const Vector3D& relativeScale);

	void SetWorldPosition(const Vector3D& worldPosition);
	void SetWorldRotation(const Quaternion& worldRotation);
	void SetWorldScale(const Vector3D& worldScale);
	Component* Clone()override;
	void AssignPointerAndReference()override;

	Vector3D GetWorldPosition()const;
	Quaternion GetWorldRotation()const;
	Vector3D GetWorldScale()const;

	Vector3D GetRelativePosition()const;
	Quaternion GetRelativeRotation()const;
	Vector3D GetRelativeScale()const;

	Vector3D GetForwardVector()const;
	Vector3D GetRightVector()const;
	Vector3D GetUpVector()const;
};

#endif // !_SCENECOMPONENT_H_


