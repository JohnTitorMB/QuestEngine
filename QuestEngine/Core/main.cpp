#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Window.h"
#include "World.h"
class ManagerTest;
class ObjectTest;

int main()
{
	//Initialise Opengl Window
	Window* window = new Window(1920, 1080, new char[] {"Opengl Window"});
	glEnable(GL_DEPTH_TEST);

	//Init World
	World* world = World::Instance();
	world->InitWorld();

	// Game Engine Loop
	float cameraAngle = 0.0f;
	float stepLight = 1.0f;
	while (!glfwWindowShouldClose(window->GetWindow()))
	{
		glClearColor(0.3f, 0.3f, 0.3f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

		world->Update();
		world->Display(window);

		glfwSwapBuffers(window->GetWindow());
		glfwPollEvents();
	}

	delete window;
	window = nullptr;

	return 0;
}

