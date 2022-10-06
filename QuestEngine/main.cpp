#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Window.h"
#include "Shape.h"
#include "Camera.h"
int main()
{
	Window* window = new Window(1000, 1000, new char[] {"Opengl Window"});

	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"uniform vec2 shapePosition;\n"
		"uniform vec2 cameraPosition;\n"
		"uniform float cameraVerticalSize;\n"
		"uniform float cameraHorizontalSize;\n"		
		"void main()\n"
		"{\n"
			/*Transform vertice from localSpace to worldSpace */
			"vec2 verticeInWorldSpace = vec2(shapePosition.x + aPos.x, shapePosition.y + aPos.y);\n" 
			/*Transform vertice from worldSpace to cameraSpace (viewSpace or eyeSpace) */
			"vec2 verticeInCameraSpace = verticeInWorldSpace - cameraPosition;\n"
			/*Transform vertice from cameraSpace to NDCSpace*/
			"vec2 verticeInNDCSpace;\n"
			"verticeInNDCSpace.x = verticeInCameraSpace.x*2.0/cameraHorizontalSize;\n"
			"verticeInNDCSpace.y = verticeInCameraSpace.y*2.0/cameraVerticalSize;\n"
			"gl_Position = vec4(verticeInNDCSpace.x, verticeInNDCSpace.y, aPos.z, 1.0);\n"
		"}\0";


	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FdfragColor;\n"
		"void main()\n"
		"{\n"
		"	FdfragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f)\n;"
		"}\0";
	Vector2D cameraPosition = Vector2D(0.0f, 0.0f);
	Vector2D shapePos = Vector2D(2.5f, 0.0f);
	Vector2D shapePos2 = Vector2D(-2.5f, 0.0f);

	Camera* camera = new Camera(cameraPosition, 10, true);
	Shader* shader = new Shader(vertexShaderSource, fragmentShaderSource);

	//Test all available Shape
	//Shape* shape = Shape::CreateTriangle(shader, GLDrawType::GLSTATIC_DRAW, shapePos, -0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.5f);
	//Shape* shape = Shape::CreateRectangle(shader, GLDrawType::GLSTATIC_DRAW, shapePos, 0.5f,0.5f);
	//Shape* shape = Shape::CreateRegularConvexPolygon(shader, GLDrawType::GLSTATIC_DRAW, shapePos, 5, 0.5f);
	Shape* shape = Shape::CreateCircle(shader, GLDrawType::GLSTATIC_DRAW, shapePos, 2);

	//Shape* shape = Shape::CreateGrid(shader, GLDrawType::GLSTATIC_DRAW, shapePos,10,10);

	// Game Engine Loop
	while (!glfwWindowShouldClose(window->GetWindow()))
	{
		glClearColor(0.6f, 0.6f, 0.6f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);	
		shape->SetPosition(shapePos);
		shape->Draw(camera,window);
		shape->SetPosition(shapePos2);
		shape->Draw(camera, window);

		glfwSwapBuffers(window->GetWindow());
		glfwPollEvents();
	}

	delete shape;
	shape = nullptr;

	delete shader;
	shader = nullptr;

	delete camera;
	camera = nullptr;

	delete window;
	window = nullptr;

	return 0;
}

