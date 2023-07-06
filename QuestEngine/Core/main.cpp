#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Window.h"
#include "Shape.h"
#include "Camera.h"
#include "../Utilities/TransformationUtilities.h"
#include "../Utilities/Vector2DShape.h"
#include "../Utilities/FileSystem.h"
int main()
{
	Window* window = new Window(1920, 1080, new char[] {"Opengl Window"});
	
	const char* vsFilename = "Assets/DefaultVertexShader.vert";	
	std::string vertexShaderSourceString = FileSystem::get_file_contents(vsFilename);
	const char* vertexShaderSource = vertexShaderSourceString.c_str();
	
	const char* fsFilename = "Assets/DefaultFragmentShader.frag";
	std::string fragmentShaderSourceString = FileSystem::get_file_contents(fsFilename);
	const char* fragmentShaderSource = fragmentShaderSourceString.c_str();

	Vector2D cameraPosition = Vector2D(0.0f, 0.0f);
	
	Camera* camera = new Camera(cameraPosition,0, 10, false);
	Shader* shader = new Shader(vertexShaderSource, fragmentShaderSource);
	float time = 0;

	//Identity
	Vector2D shapePos = Vector2D(0.0f, 1.0f);
	Vector2D initialVector = Vector2D(-1.0, 0.0f);
	Vector2D identityValue = TransformationUtilities::Identity(initialVector);
	Vector2DShape* vector2DShapeIdentity = new Vector2DShape(identityValue, shapePos, 0.0f, 1.0f, 0.0f, 1.0f);

	//Translation
	shapePos = Vector2D(0.0f, 0.0f);
	Vector2D initialVectorTranslation = Vector2D(2.5f, 2.5f);
	Vector2D translateValue = Vector2D(1.0, 0.0f);
	Vector2D tranlationValue = TransformationUtilities::Translation(initialVectorTranslation, translateValue);
	Vector2DShape* vector2DShape = new Vector2DShape(initialVectorTranslation,shapePos,0.0f,0.0f,1.0f,1.0f);
	Vector2DShape* vector2DShapeTranslation = new Vector2DShape(tranlationValue, shapePos, 0.0f, 1.0f, 0.0f, 1.0f);

	//Scale
	shapePos = Vector2D(0.0f, -1.0f);
	Vector2D initialVectorScale = Vector2D(-1.0f, -1.0f);
	float scale = 2.0f;
	Vector2D ScaleValue = TransformationUtilities::Scale(initialVectorScale, scale);
	Vector2DShape* vector2DShape1 = new Vector2DShape(initialVectorScale, shapePos, 1.0f, .0f, 1.0f, 1.0f);
	Vector2DShape* vector2DShapeScale = new Vector2DShape(ScaleValue, Vector2D(shapePos.m_x+0.2f, shapePos.m_y), 0.0f, 1.0f, 0.0f, 1.0f);

	//Homothety
	shapePos = Vector2D(-1.0f, -1.0f);
	Vector2D initialHomothety = Vector2D(-1.0f, -1.0f);
	Vector2D HomothetyCenter = Vector2D(-1.0f, 0.0f);
	float scaleHomothety = 2.5f;
	Vector2D HomothetieValue = TransformationUtilities::Homothety(initialHomothety, HomothetyCenter, scaleHomothety);
	Vector2DShape* vector2DShape2 = new Vector2DShape(initialHomothety, shapePos, 1.0f, .0f, 1.0f, 1.0f);
	Vector2DShape* vector2DHomothetyCenter = new Vector2DShape(HomothetyCenter, shapePos, 0.0f, .0f, 1.0f, 1.0f);
	Vector2DShape* vector2DShapeHomothetie = new Vector2DShape(HomothetieValue, shapePos, 0.0f, 1.0f, 0.0f, 1.0f);

	//PointReflection
	shapePos = Vector2D(-3.0f, 1.0f);
	Vector2D initialPointReflection = Vector2D(0.0f, 1.0f);
	Vector2D PointReflectionCenter = Vector2D(-1.0f, 1.0f);
	Vector2D PointReflectionValue = TransformationUtilities::PointReflection(initialPointReflection, PointReflectionCenter);
	Vector2DShape* vector2DShape3 = new Vector2DShape(initialPointReflection, shapePos, 1.0f, .0f, 1.0f, 1.0f);
	Vector2DShape* vector2DShapePointReflectionCenter = new Vector2DShape(PointReflectionCenter, shapePos, 0.0f, .0f, 1.0f, 1.0f);
	Vector2DShape* vector2DShapePointReflection = new Vector2DShape(PointReflectionValue, shapePos, 0.0f, 1.0f, 0.0f, 1.0f);

	//AxialSymetry
	shapePos = Vector2D(3.0f, -1.0f);
	Vector2D initialAxialSymetry = Vector2D(0.5f, 1.0f);
	Vector2D Axe = Vector2D(1.0f, 1.0f);
	Vector2D AxialSymetryValue = TransformationUtilities::AxialSymetry(initialAxialSymetry, Axe);
	Vector2DShape* vector2DShape4 = new Vector2DShape(initialAxialSymetry, shapePos, 1.0f, .0f, 1.0f, 1.0f);
	Vector2DShape* vector2DShapeAxialSymetryAxe = new Vector2DShape(Axe, shapePos, 0.0f, .0f, 1.0f, 1.0f);
	Vector2DShape* vector2DShapeAxialSymetry = new Vector2DShape(AxialSymetryValue, shapePos, 0.0f, 1.0f, 0.0f, 1.0f);

	//AxialSymetryCenter
	shapePos = Vector2D(5.0f, -1.0f);
	Vector2D initialValue = Vector2D(0.5f, 1.0f);
	Vector2D center = Vector2D(2.0f, 0.0f);
	Axe = Vector2D(1.0f, 1.0f);
	Vector2D AxialSymetryCenterValue = TransformationUtilities::AxialSymetry(initialValue, Axe,center);
	Vector2DShape* vector2DShape5 = new Vector2DShape(initialAxialSymetry, shapePos, 1.0f, .0f, 1.0f, 1.0f);
	Vector2DShape* vector2DShapeAxialSymetryCenterAxe = new Vector2DShape(Axe, shapePos, 0.0f, .0f, 1.0f, 1.0f);
	vector2DShapeAxialSymetryCenterAxe->SetPosition(center + shapePos);
	Vector2DShape* vector2DShapeAxialSymetryCenterValue = new Vector2DShape(center, shapePos, 0.0f, 1.0f, 1.0f, 1.0f);
	Vector2DShape* vector2DShapeAxialSymetryCenter = new Vector2DShape(AxialSymetryCenterValue, shapePos, 0.0f, 1.0f, 0.0f, 1.0f);

	//Rotation
	shapePos = Vector2D(-5.0f, -2.0f);
	Vector2D initialRotation = Vector2D(-1.0f, 1.0f);
	float angle = 10.0f;
	Vector2D RotationValue = TransformationUtilities::Rotation(initialRotation, angle);
	Vector2DShape* vector2DShape6 = new Vector2DShape(initialRotation, shapePos, 1.0f, .0f, 1.0f, 1.0f);
	Vector2DShape* vector2DShapeRotation = new Vector2DShape(RotationValue, shapePos, 0.0f, 1.0f, 0.0f, 1.0f);

	//RotationCenter
	shapePos = Vector2D(0.0f, 3.0f);
	Vector2D initialRotationC = Vector2D(-1.0f, 1.0f);
	center = Vector2D(-1.0f, 0.0f);
	angle = 10.0f;
	Vector2D RotationCValue = TransformationUtilities::Rotation(initialRotationC,center, angle);
	Vector2DShape* vector2DShape7 = new Vector2DShape(initialRotation, shapePos, 1.0f, .0f, 1.0f, 1.0f);
	Vector2DShape* vector2DShapeRotationCenter = new Vector2DShape(center, shapePos, 0.0f, 0.0f, 1.0f, 1.0f);
	Vector2DShape* vector2DShapeRotationC = new Vector2DShape(RotationCValue, shapePos, 0.0f, 1.0f, 0.0f, 1.0f);
	Vector2DShape* vector2DShapeRotationAditional= new Vector2DShape(RotationCValue - center, center + shapePos, 1.0f, 0.0f, 0.0f, 1.0f);

	Vector2D SquarePos = Vector2D(0, 0);
	Vector2D SquareScale = Vector2D(1.5f, 1.5f);
	float SquareRotationAngle = 45.0f;
	Transform tr = Transform(SquarePos, SquareScale, SquareRotationAngle);
	Shape* circle = Shape::CreateRegularConvexPolygon(shader, GLDrawType::GLSTATIC_DRAW, tr, 4, 2.5f);
	// Game Engine Loop
	while (!glfwWindowShouldClose(window->GetWindow()))
	{
		glClearColor(0.6f, 0.6f, 0.6f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);	
		
		vector2DShapeIdentity->Draw(camera, window);

		float scale = TransformationUtilities::PingPong(time, 1.0f);
		tranlationValue = TransformationUtilities::Translation(initialVectorTranslation, translateValue * scale);
		vector2DShapeTranslation->SetValue(tranlationValue);
		vector2DShape->Draw(camera, window);
		vector2DShapeTranslation->Draw(camera, window);

		scale = TransformationUtilities::PingPong(time, 2.0f);
		ScaleValue = TransformationUtilities::Scale(initialVectorScale, scale);
		vector2DShapeScale->SetValue(ScaleValue);
		vector2DShapeScale->Draw(camera, window);
		vector2DShape1->Draw(camera, window);


		scale = TransformationUtilities::PingPong(time, scaleHomothety);
		HomothetieValue = TransformationUtilities::Homothety(initialHomothety, HomothetyCenter, scale);
		vector2DShapeHomothetie->SetValue(HomothetieValue);
		vector2DShape2->Draw(camera, window);
		vector2DShapeHomothetie->Draw(camera, window);
		vector2DHomothetyCenter->Draw(camera, window);

		vector2DShape3->Draw(camera, window);
		vector2DShapePointReflectionCenter->Draw(camera, window);
		vector2DShapePointReflection->Draw(camera, window);

		vector2DShape4->Draw(camera, window);
		vector2DShapeAxialSymetryAxe->Draw(camera, window);
		vector2DShapeAxialSymetry->Draw(camera, window);

		vector2DShape5->Draw(camera, window);
		vector2DShapeAxialSymetryCenterAxe->Draw(camera, window);
		vector2DShapeAxialSymetryCenterValue->Draw(camera, window);
		vector2DShapeAxialSymetryCenter->Draw(camera, window);
		
		float finalAngle = TransformationUtilities::PingPong(time*50, 360.0f);
		RotationValue = TransformationUtilities::Rotation(initialRotation, finalAngle);
		vector2DShapeRotation->SetValue(RotationValue);
		vector2DShape6->Draw(camera, window);
		vector2DShapeRotation->Draw(camera, window);

		RotationCValue = TransformationUtilities::Rotation(initialRotationC,center, finalAngle);
		vector2DShapeRotationC->SetValue(RotationCValue);
		vector2DShapeRotationAditional->SetValue(RotationCValue - center);

		vector2DShape7->Draw(camera, window);
		vector2DShapeRotationCenter->Draw(camera, window);
		vector2DShapeRotationC->Draw(camera, window);
		vector2DShapeRotationAditional->Draw(camera, window);

		float squareScale = TransformationUtilities::PingPong(time, 1.5f);
		circle->SetScale(Vector2D(squareScale, squareScale));

		float squareAngle = TransformationUtilities::Repeat(time*100, 360.0f);
		circle->SetAngle(squareAngle);
		circle->Draw(camera, window);
		time += 0.01f;
		glfwSwapBuffers(window->GetWindow());
		glfwPollEvents();
	}

	delete vector2DShape;
	vector2DShape = nullptr;

	delete vector2DShapeTranslation;
	vector2DShapeTranslation = nullptr;

	delete vector2DShapeIdentity;
	vector2DShapeIdentity = nullptr;


	delete vector2DShape1;
	vector2DShape1 = nullptr;

	delete vector2DShapeScale;
	vector2DShapeScale = nullptr;

	delete camera;
	camera = nullptr;

	delete window;
	window = nullptr;

	return 0;
}

