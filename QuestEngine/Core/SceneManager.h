#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_
#include "Scene.h"

class SceneManager
{
	std::vector<Scene*> m_scenes;
	static SceneManager* m_instance;
public:
	SceneManager();
	~SceneManager();
	Scene& CreateScene();
	void LoadScene(int index);
	static SceneManager* Instance();
};

#endif // !_SCENEMANAGER_H_

