#ifndef _SCENEGRAPHTESTCOMPONENT_H_
#define _SCENEGRAPHTESTCOMPONENT_H_
#include "../Core/Component.h"
#include "../Core/Components/DirectionalLight.h"

class SceneGraphTestComponent : public Component
{
private:
	SceneComponent* m_sceneComponent;
	SceneComponent* m_sceneComponent2;
	SceneComponent* m_sceneComponentChild;
	SceneComponent* m_sceneComponentChild2;
	bool m_isAttachToParent1 = true;
	Quaternion childRotation;

protected:
	void Update() override;
	void Start() override;

public:
	SceneGraphTestComponent();
	SceneGraphTestComponent(const SceneGraphTestComponent& other);
	void SetSceneComponent(SceneComponent* sceneComponent);
	void SetSceneComponent2(SceneComponent* sceneComponent2);
	void SetSceneComponentChild(SceneComponent* sceneComponentChild);
	void SetSceneComponentChild2(SceneComponent* sceneComponentChild2);
	Component* Clone()override;
	void AssignPointerAndReference()override;

	float time = 0;
	float time2 = 0;
};

#endif

