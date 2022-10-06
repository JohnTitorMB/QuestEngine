#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Window.h"
#include "Shape.h"
int main()
{
	Window* window = new Window(1000, 1000, new char[] {"Opengl Window"});

	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"uniform vec2 shapePosition;\n"
		"void main()\n"
		"{\n"
			/*Transform vertice in localSpace to worldSpace */
			"vec2 localToWorldSpace = vec2(shapePosition.x + aPos.x, shapePosition.y + aPos.y);\n" 
			"gl_Position = vec4(localToWorldSpace.x, localToWorldSpace.y, aPos.z, 1.0);\n"
		"}\0";


	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FdfragColor;\n"
		"void main()\n"
		"{\n"
		"	FdfragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f)\n;"
		"}\0";

	Shader* shader = new Shader(vertexShaderSource, fragmentShaderSource);

	Vector2D shapePos = Vector2D(0.5f, 0.5f);
	Vector2D shapePos2 = Vector2D(-0.5f, 0.5f);

	//Test all available Shape
	//Shape* shape = Shape::CreateTriangle(shader, GLDrawType::GLSTATIC_DRAW, shapePos, -0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.5f);
	//Shape* shape = Shape::CreateRectangle(shader, GLDrawType::GLSTATIC_DRAW, shapePos, 0.5f,0.5f);
	//Shape* shape = Shape::CreateRegularConvexPolygon(shader, GLDrawType::GLSTATIC_DRAW, shapePos, 5, 0.5f);
	Shape* shape = Shape::CreateCircle(shader, GLDrawType::GLSTATIC_DRAW, shapePos, 0.4f);

	//Shape* shape = Shape::CreateGrid(shader, GLDrawType::GLSTATIC_DRAW, shapePos,10,10);

	// Game Engine Loop
	while (!glfwWindowShouldClose(window->GetWindow()))
	{
		glClearColor(0.6f, 0.6f, 0.6f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);	
		shape->SetPosition(shapePos);
		shape->Draw();
		shape->SetPosition(shapePos2);
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

