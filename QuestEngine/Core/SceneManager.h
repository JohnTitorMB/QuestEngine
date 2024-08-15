#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_
#include "Scene.h"

class QuestEngine;
class SceneManager
{
	friend class QuestEngine;
	std::vector<Scene*> m_scenes;
	static SceneManager* m_instance;
	~SceneManager();

public:
	SceneManager();
	Scene& CreateScene();
	void LoadScene(int index);
	static SceneManager* Instance();
};

#endif // !_SCENEMANAGER_H_

