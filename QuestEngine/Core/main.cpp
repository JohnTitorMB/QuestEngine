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
/*
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <iostream>*/


int main()
{
	Vector2D size = Vector2D(2.0f * 1920 / 1080, 2.0f);
	float near = 30;
	float far = 100;
	Matrix4x4::PerspectiveReverseZ(-size.m_x / 2.0f, size.m_x / 2.0f, -size.m_y / 2.0f, size.m_y / 2.0f, near, far);

	Window* window = new Window(1920, 1080, new char[] {"Opengl Window"});


	//Init World
	World* world = World::Instance();
	world->InitWorld();

	InputSystem* inputSystem = InputSystem::Instance();

	// Game Engine Loop
	float cameraAngle = 0.0f;
	float stepLight = 1.0f;

	bool vsync = true;
	window->SetVsync(vsync);

	while (!glfwWindowShouldClose(window->GetWindow()))
	{
		glfwPollEvents();
		inputSystem->ProcessInput(window);

		glStencilMask(0xFF);
		glDepthMask(true);
		glClearColor(0.1f, 0.1f, 0.1f,1.0f);
		glClearDepth(1.0f);
		glClearStencil(0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


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

