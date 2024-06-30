#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Window.h"
#include "World.h"
#include "Inputs/InputSystem.h"
#include <chrono>
#include <thread>
#include "OBJLoader.h"
/*
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <iostream>*/


int main()
{
	Window* window = new Window(1920, 1080, new char[] {"Opengl Window"});
	glEnable(GL_DEPTH_TEST);

	//Init World
	World* world = World::Instance();
	world->InitWorld();

	InputSystem* inputSystem = InputSystem::Instance();

	// Game Engine Loop
	float cameraAngle = 0.0f;
	float stepLight = 1.0f;

	float previousTime = glfwGetTime();

	while (!glfwWindowShouldClose(window->GetWindow()))
	{
		float currentTime = glfwGetTime();
		world->deltaTime = currentTime - previousTime;

		previousTime = currentTime;

		glfwPollEvents();
		inputSystem->ProcessInput(window);
		
		glClearColor(0.1f, 0.1f, 0.1f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

		world->Update();
		
		world->Display(window);

		glfwSwapBuffers(window->GetWindow());
	}

	delete window;
	window = nullptr;


//	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
//	_CrtDumpMemoryLeaks();

	return 0;
}

