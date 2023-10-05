#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Window.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "../Utilities/MeshUtilities.h"
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

	Vector3D cameraPosition = Vector3D(0.0f, 0.0f,0.0f);
	Vector3D cameraAngles = Vector3D(0.0f, 0.0f,0.0f);
	
	Camera* camera = new Camera(cameraPosition, cameraAngles, 10,true, false);
	camera->SetFov(60);
	Shader* shader = new Shader(vertexShaderSource, fragmentShaderSource);
	Mesh* mesh = MeshUtilities::CreateCube(1.0f);

	Transform transform = Transform(Vector3D(0, 0, 5.0f), Vector3D(1.0f, 1.0f, 1.0f), Vector3D(0.0f, 0.0f, 0.0f));
	Texture* m_texture = new Texture("Assets/Texture.png");
	MeshRenderer* meshRenderer = new MeshRenderer(mesh, transform, shader, m_texture);
	float time = 0;
	Vector3D cameraRelativePosition = Vector3D(0.0f,0.0f,-5.0f);
	float cameraAngle = 0;
	float speed = 0.5f;

	glEnable(GL_DEPTH_TEST);

	// Game Engine Loop
	while (!glfwWindowShouldClose(window->GetWindow()))
	{
		glClearColor(0.6f, 0.6f, 0.6f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
		transform.m_position = Vector3D(0, 0, 5.0f);
		transform.m_angles = Vector3D(0, -cameraAngle, 0.0f);
		meshRenderer->SetTransform(transform);
		meshRenderer->Draw(camera, window);

		Vector3D cameraPosition = transform.m_position + cameraRelativePosition * Matrix4x4::RotateY(cameraAngle);
		camera->SetPosition(cameraPosition);
		camera->SetAngle(Vector3D(0, cameraAngle,0));
		cameraAngle += speed;

		transform.m_position = Vector3D(0, 0, 8.0f);
		transform.m_angles = Vector3D(cameraAngle, 0.0f, 0.0f);

		meshRenderer->SetTransform(transform);
		meshRenderer->Draw(camera, window);
		glfwSwapBuffers(window->GetWindow());
		glfwPollEvents();
	}

	delete mesh;
	mesh = nullptr;

	delete meshRenderer;
	meshRenderer = nullptr;

	delete m_texture;
	m_texture = nullptr;

	delete camera;
	camera = nullptr;

	delete window;
	window = nullptr;

	return 0;
}

