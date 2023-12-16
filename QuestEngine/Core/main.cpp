#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Window.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "../Utilities/MeshUtilities.h"
#include "../Utilities/FileSystem.h"
# define M_PI           3.14159265358979323846f  /* pi */

void CreateCube(Vector3D position, Mesh* mesh, Shader* shader, Texture*texture, std::vector<Transform>& transforms, std::vector<MeshRenderer*>& meshRenderers)
{
	Transform cubeTransform = Transform(position, Vector3D(1.0f, 1.0f, 1.0f), Quaternion::Identity());
	MeshRenderer* cubeRenderer = new MeshRenderer(mesh, cubeTransform, shader, texture);
	transforms.push_back(cubeTransform);
	meshRenderers.push_back(cubeRenderer);
}

void SetRotation(int index, Quaternion rotation, std::vector<MeshRenderer*> meshRenderers)
{
	Transform tr = meshRenderers[index]->GetTransform();
	tr.SetRotation(rotation);
	meshRenderers[index]->SetTransform(tr);
}

void SetPosition(int index, Vector3D position, std::vector<MeshRenderer*> meshRenderers)
{
	Transform tr = meshRenderers[index]->GetTransform();
	tr.SetPosition(position);
	meshRenderers[index]->SetTransform(tr);
}

Vector3D GetPosition(int index, std::vector<MeshRenderer*> meshRenderers)
{
	Transform tr = meshRenderers[index]->GetTransform();
	return tr.GetPosition();
}

Quaternion GetRotation(int index, std::vector<MeshRenderer*> meshRenderers)
{
	Transform tr = meshRenderers[index]->GetTransform();
	return tr.GetRotation();
}

int main()
{
	Window* window = new Window(1920, 1080, new char[] {"Opengl Window"});

	const char* vsFilename = "Assets/DefaultVertexShader.vert";	
	std::string vertexShaderSourceString = FileSystem::get_file_contents(vsFilename);
	const char* vertexShaderSource = vertexShaderSourceString.c_str();
	
	const char* fsFilename = "Assets/DefaultFragmentShader.frag";
	std::string fragmentShaderSourceString = FileSystem::get_file_contents(fsFilename);
	const char* fragmentShaderSource = fragmentShaderSourceString.c_str();

	Vector3D cameraPosition = Vector3D(0.0f, 2.0f,-5.0f);
	Vector3D cameraAngles = Vector3D(0.0f, 0.0f,0.0f);	
	
	Camera* camera = new Camera(cameraPosition, cameraAngles, 10,true, false);
	camera->SetFov(60);
	Shader* shader = new Shader(vertexShaderSource, fragmentShaderSource);

	std::vector<Mesh*> meshs;
	std::vector<Transform> transforms;
	std::vector<MeshRenderer*> meshRenderers;
	std::vector<Texture*> textures;
	Texture* m_texture = new Texture("Assets/Texture.png");

	textures.push_back(m_texture);	
	
	//Create Plane
	Mesh* planeMesh = MeshUtilities::CreatePlane(1000);
	Transform planeTransform = Transform(Vector3D(0, -1.0f, 0.0f), Vector3D(1.0f, 1.0f, 1.0f), Quaternion::Identity());
	MeshRenderer* planeRenderer = new MeshRenderer(planeMesh, planeTransform, shader, m_texture, Vector2D(125, 125), Vector2D((1.0 / 8.0) / 2.0f, (1.0 / 8.0) / 2.0f));
	meshs.push_back(planeMesh);
	transforms.push_back(planeTransform);
	meshRenderers.push_back(planeRenderer);

	//Create Cube
	Texture* cubeTexture = new Texture("Assets/CubeTextureFace.png");
	textures.push_back(cubeTexture);

	Mesh* cubeMesh = MeshUtilities::CreateCustomCubeUV(1.0f, MeshUtilities::CubeUVInfo());
	meshs.push_back(cubeMesh);

	//Quaternion Test
	for (int i = 0; i < 7; i++)
	{
		Vector3D position = Vector3D(-6.0f + i * 2.0f, 0.0f, 6.0f);
		CreateCube(position, cubeMesh, shader, cubeTexture, transforms, meshRenderers);
	}

	//Quaternion Lerp Test
	for (int i = 0; i < 10; i++)
	{
		Vector3D position = Vector3D(-9.0f + i * 2.0f, 2.0f, 6.0f);
		CreateCube(position, cubeMesh, shader, cubeTexture, transforms, meshRenderers);
	}

	//Quaternion Slerp Test
	for (int i = 0; i < 10; i++)
	{
		Vector3D position = Vector3D(-9.0f + i * 2.0f, 4.0f, 6.0f);
		CreateCube(position, cubeMesh, shader, cubeTexture, transforms, meshRenderers);
	}

	//Vector3 lerp and slerp Test
	for (int i = 0; i < 3; i++)
	{
		Vector3D position = Vector3D(-5.0f + i * 5.0f, 6.0f, 6.0f);
		CreateCube(position, cubeMesh, shader, cubeTexture, transforms, meshRenderers);
	}
	


	float time = 0;
	Vector3D cameraRelativePosition = Vector3D(0.0f,0.0f,-5.0f);
	float cameraAngle = 0;
	float speed = 0.5f;

	
	//IdentityTest
	SetRotation(1, Quaternion::Identity(), meshRenderers);

	//EulerAngleTest
	SetRotation(2, Quaternion::FromEulerAngle(Vector3D(0,45,90)), meshRenderers);

	//AxisAngleTest
	SetRotation(3, Quaternion::AxisAngle(Vector3D(0,1,0), 90), meshRenderers);

	//GoToRotationTest
	Quaternion rotation = Quaternion::AxisAngle(Vector3D(0, 1, 0), 45);
	rotation.GoToRotation(Vector3D::Up, Vector3D::Right);
	SetRotation(4, rotation, meshRenderers);

	//LookRotationTest
	rotation = Quaternion::AxisAngle(Vector3D(0, 1, 0), 45);
	SetRotation(5, Quaternion::LookRotation(Vector3D::Up * -1, Vector3D::Right), meshRenderers);

	//RotateWorldTest
	rotation = Quaternion::AxisAngle(Vector3D(0, 1, 0), 45);
	Quaternion rotationOther = Quaternion::AxisAngle(Vector3D(1, 0, 0), 90);
	rotation.Rotate(rotationOther, true);
	SetRotation(6, rotation, meshRenderers);

	//RotateSelfTest
	rotation = Quaternion::AxisAngle(Vector3D(0, 1, 0), 45);
	rotationOther = Quaternion::AxisAngle(Vector3D(1, 0, 0), 90);
	rotation.Rotate(rotationOther, false);
	SetRotation(7, rotation, meshRenderers);

	Vector3D startEulerAngle = Vector3D(0, 0, 0);
	Vector3D startEulerAngle2 = Vector3D(90, 0, 0);
	Vector3D endEulerAngle = Vector3D(90, 0, 0);
	Vector3D endEulerAngle1 = Vector3D(-270, 0, 0);
	Vector3D endEulerAngle2 = Vector3D(-90, 0, 0);
	Vector3D endEulerAngle3 = Vector3D(270, 0, 0);
	Vector3D endEulerAngle4 = Vector3D(0, 90, 0);
	Quaternion quatA = Quaternion::FromEulerAngle(startEulerAngle);
	Quaternion quatB = Quaternion::FromEulerAngle(endEulerAngle, Quaternion::EulerAngleMode::XYZ); 
	Quaternion quatC = Quaternion::FromEulerAngle(endEulerAngle1, Quaternion::EulerAngleMode::XYZ); 
	Quaternion quatD = Quaternion::FromEulerAngle(endEulerAngle2, Quaternion::EulerAngleMode::XYZ); 
	Quaternion quatE = Quaternion::FromEulerAngle(endEulerAngle3, Quaternion::EulerAngleMode::XYZ); 
	
	Quaternion quatF = Quaternion::FromEulerAngle(startEulerAngle2, Quaternion::EulerAngleMode::XYZ); 
	Quaternion quatG = Quaternion::FromEulerAngle(endEulerAngle4, Quaternion::EulerAngleMode::XYZ); 

	Vector3D startPosition = Vector3D(-5.0f, 6.0f, 6.0f);
	Vector3D endPosition = Vector3D(0.0f, 6.0f, 6.0f);

	Vector3D slerpLocalPosition = Vector3D(2.5f, 6.5f, 6.0f);
	Vector3D slerpLocalPosition2 = Vector3D(5.0f, 6.5f, 6.0f);

	Vector3D slerpStartDirection = Vector3D(-1.0f, 1.0f, 0.0f).Normalized();
	Vector3D slerpEndDirection = Vector3D(1.0f, 1.0f, 0.0f).Normalized();

	float t = 0;
	glEnable(GL_DEPTH_TEST);
	// Game Engine Loop
	while (!glfwWindowShouldClose(window->GetWindow()))
	{
		glClearColor(0.6f, 0.6f, 0.6f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
		float lerpInterpolationValue = Mathf::PingPong(t, 1.0f);

		//Quaternion Linear Interpolation normalized shortest path
		Quaternion rotation = Quaternion::LerpNormalized(quatA, quatB, lerpInterpolationValue, false);
		SetRotation(8, rotation, meshRenderers);

		rotation = Quaternion::LerpNormalized(quatA, quatC, lerpInterpolationValue, false);
		SetRotation(9, rotation, meshRenderers);

		rotation = Quaternion::LerpNormalized(quatA, quatD, lerpInterpolationValue, false);
		SetRotation(10, rotation, meshRenderers);

		rotation = Quaternion::LerpNormalized(quatA, quatE, lerpInterpolationValue, false);
		SetRotation(11, rotation, meshRenderers);

		rotation = Quaternion::LerpNormalized(quatF, quatG, lerpInterpolationValue, false);
		SetRotation(12, rotation, meshRenderers);

		//Quaternion Linear Interpolation normalized longest path
		rotation = Quaternion::LerpNormalized(quatA, quatB, lerpInterpolationValue, true);
		SetRotation(13, rotation, meshRenderers);

		rotation = Quaternion::LerpNormalized(quatA, quatC, lerpInterpolationValue, true);
		SetRotation(14, rotation, meshRenderers);

		rotation = Quaternion::LerpNormalized(quatA, quatD, lerpInterpolationValue, true);
		SetRotation(15, rotation, meshRenderers);

		rotation = Quaternion::LerpNormalized(quatA, quatE, lerpInterpolationValue, true);
		SetRotation(16, rotation, meshRenderers);

		rotation = Quaternion::LerpNormalized(quatF, quatG, lerpInterpolationValue, true);
		SetRotation(17, rotation, meshRenderers);

		//Quaternion Spherical Interpolation normalized shortest path
		rotation = Quaternion::SlerpNormalized(quatA, quatB, lerpInterpolationValue, false);
		SetRotation(18, rotation, meshRenderers);

		rotation = Quaternion::SlerpNormalized(quatA, quatC, lerpInterpolationValue, false);
		SetRotation(19, rotation, meshRenderers);

		rotation = Quaternion::SlerpNormalized(quatA, quatD, lerpInterpolationValue, false);
		SetRotation(20, rotation, meshRenderers);

		rotation = Quaternion::SlerpNormalized(quatA, quatE, lerpInterpolationValue, false);
		SetRotation(21, rotation, meshRenderers);

		rotation = Quaternion::SlerpNormalized(quatF, quatG, lerpInterpolationValue, false);
		SetRotation(22, rotation, meshRenderers);

		//Quaternion Spherical Interpolation normalized longest path
		rotation = Quaternion::SlerpNormalized(quatA, quatB, lerpInterpolationValue, true);
		SetRotation(23, rotation, meshRenderers);

		rotation = Quaternion::SlerpNormalized(quatA, quatC, lerpInterpolationValue, true);
		SetRotation(24, rotation, meshRenderers);

		rotation = Quaternion::SlerpNormalized(quatA, quatD, lerpInterpolationValue, true);
		SetRotation(25, rotation, meshRenderers);

		rotation = Quaternion::SlerpNormalized(quatA, quatE, lerpInterpolationValue, true);
		SetRotation(26, rotation, meshRenderers);

		rotation = Quaternion::SlerpNormalized(quatF, quatG, lerpInterpolationValue, true);
		SetRotation(27, rotation, meshRenderers);

		// Vector3 Linear Interpolation
		Vector3D position = Vector3D::Lerp(startPosition, endPosition, lerpInterpolationValue);
		SetPosition(28, position, meshRenderers);
		
		// Vector3 Spherical Interpolation shortest path
		position = slerpLocalPosition + Vector3D::Slerp(slerpStartDirection, slerpEndDirection, lerpInterpolationValue, false);
		SetPosition(29, position, meshRenderers);

		// Vector3 Spherical Interpolation longest path
		position = slerpLocalPosition2 + Vector3D::Slerp(slerpStartDirection, slerpEndDirection, lerpInterpolationValue, true);
		SetPosition(30, position, meshRenderers);

		t += 0.01f;
		for (int i = 0; i < meshRenderers.size(); i++)
			meshRenderers[i]->Draw(camera, window);


		glfwSwapBuffers(window->GetWindow());
		glfwPollEvents();
	}

	for (int i = 0; i < meshs.size(); i++)
	{
		delete meshs[i];
		meshs[i] = nullptr;
	}

	for (int i = 0; i < meshRenderers.size(); i++)
	{
		delete meshRenderers[i];
		meshRenderers[i] = nullptr;
	}

	for (int i = 0; i < textures.size(); i++)
	{
		delete textures[i];
		textures[i] = nullptr;
	}

	meshs.clear();
	meshRenderers.clear();
	transforms.clear();
	textures.clear();

	delete camera;
	camera = nullptr;

	delete window;
	window = nullptr;

	return 0;
}

