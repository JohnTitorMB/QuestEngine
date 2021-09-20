#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Window.h"
#include "Shape.h"
int main()
{
	Window* window = new Window(1000, 1000, new char[] {"Opengl Window"});

	float vertices[] = { -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f };
	 

	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";


	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FdfragColor;\n"
		"void main()\n"
		"{\n"
		"	FdfragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f)\n;"
		"}\0";

	Shader* shader = new Shader(vertexShaderSource, fragmentShaderSource);

	//Test all available Shape
	Shape* shape = Shape::CreateTriangle(shader, GLDrawType::GLSTATIC_DRAW, -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f);
	//Shape* shape = new Shape(shader, GLDrawType::GLSTREAM_DRAW, sizeof(vertices), vertices);
	//Shape* shape = Shape::CreateRectangle(shader, GLDrawType::GLSTATIC_DRAW,0.8f,0.5f);
	//Shape* shape = Shape::CreateRegularConvexPolygon(shader, GLDrawType::GLSTATIC_DRAW, 5, 0.8f);
	//Shape* shape = Shape::CreateCircle(shader, GLDrawType::GLSTATIC_DRAW, 0.8f);

	//Shape* shape = Shape::CreateGrid(shader, GLDrawType::GLSTATIC_DRAW,10,10);

	// Game Engine Loop
	while (!glfwWindowShouldClose(window->GetWindow()))
	{
		glClearColor(0.6f, 0.6f, 0.6f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);			
		shape->Draw();
		glfwSwapBuffers(window->GetWindow());
		glfwPollEvents();
	}

	delete shape;
	shape = nullptr;

	delete shader;
	shader = nullptr;

	delete window;
	window = nullptr;

	return 0;
}

