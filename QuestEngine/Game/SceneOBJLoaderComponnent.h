#ifndef _SCENEOBJLOADERCOMPONENT_H_
#define _SCENEOBJLOADERCOMPONENT_H_
#include "../Core/Component.h"
#include "../Core/Events/EventCallback.h"
#include "../Core/Inputs/InputAction.h"
#include "../Core/Entity.h"
#include "../Core/Inputs/InputMap.h"

class SceneOBJLoaderComponnent : public Component
{
private:
	InputMap* m_inputMap;
	int currentOBJ = 0;
	std::set<Entity*> objs;
protected:
	void Update() override;
	void Start() override;
public:
	SceneOBJLoaderComponnent();
	SceneOBJLoaderComponnent(const SceneOBJLoaderComponnent& cameraController);
	~SceneOBJLoaderComponnent();
	Component* Clone()override;
	void AssignPointerAndReference()override;
	void NextObj(InputCallbackData data);
	void PrevObj(InputCallbackData data);
	void UnloadObjs();
};

#endif _SCENEOBJLOADERCOMPONENT_H_
