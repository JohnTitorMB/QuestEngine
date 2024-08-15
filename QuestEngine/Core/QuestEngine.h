#ifndef _QUESTENGINE_H_
#define _QUESTENGINE_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Window.h"
#include "World.h"
#include "Inputs/InputSystem.h"
#include <chrono>
#include <thread>
#include "OBJLoader.h"
#include "TimeManager.h"
class QuestEngine
{
private :
    Window* m_window = nullptr;
    QuestEngine();
    ~QuestEngine();

    friend int main(); 

    QuestEngine(const QuestEngine&) = delete;
    QuestEngine& operator=(const QuestEngine&) = delete;

    void Init();
    void Update();
};

#endif _QUESTENGINE_H_