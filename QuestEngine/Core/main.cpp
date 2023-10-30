#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Window.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "../Utilities/MeshUtilities.h"
#include "../Utilities/FileSystem.h"
# define M_PI           3.14159265358979323846f  /* pi */

int main()
{
	Window* window = new Window(1920, 1080, new char[] {"Opengl Window"});

	const char* vsFilename = "Assets/DefaultVertexShader.vert";	
	std::string vertexShaderSourceString = FileSystem::get_file_contents(vsFilename);
	const char* vertexShaderSource = vertexShaderSourceString.c_str();
	
	const char* fsFilename = "Assets/DefaultFragmentShader.frag";
	std::string fragmentShaderSourceString = FileSystem::get_file_contents(fsFilename);
	const char* fragmentShaderSource = fragmentShaderSourceString.c_str();

	Vector3D cameraPosition = Vector3D(2.5f, 2.0f,-5.0f);
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

	//Create Cube
	Mesh* cubeMesh = MeshUtilities::CreateCube(1.0f);
	Transform cubeTransform = Transform(Vector3D(0.0f, 0.0f, 0.0f), Vector3D(1.0f, 1.0f, 1.0f), Vector3D(0.0f, 0.0f, 0.0f));
	MeshRenderer* cubeRenderer = new MeshRenderer(cubeMesh, cubeTransform, shader, m_texture);
	meshs.push_back(cubeMesh);
	transforms.push_back(cubeTransform);
	meshRenderers.push_back(cubeRenderer);
	
	//Create Sphere
	Mesh* sphereMesh = MeshUtilities::CreateUVSphere(0.5f, 32, 16);
	Transform sphereTransform = Transform(Vector3D(2.0f, 0.0f, 0.0f), Vector3D(1.0f, 1.0f, 1.0f), Vector3D(0.0f, 0.0f, 0.0f));
	MeshRenderer* sphereRenderer = new MeshRenderer(sphereMesh, sphereTransform, shader, m_texture);
	meshs.push_back(sphereMesh);
	transforms.push_back(sphereTransform);
	meshRenderers.push_back(sphereRenderer);

	//Create Cylinder
	Mesh* cylinderMesh = MeshUtilities::CreateCylinder(2.0f / (M_PI * 2.0f), 32, 1.0f);
	Transform cylinderTransform = Transform(Vector3D(4.0f, 0.0f, 0.0f), Vector3D(1.0f, 1.0f, 1.0f), Vector3D(0.0f, 0.0f, 0.0f));
	MeshRenderer* cylinderRenderer = new MeshRenderer(cylinderMesh, cylinderTransform, shader, m_texture);
	meshs.push_back(cylinderMesh);
	transforms.push_back(cylinderTransform);
	meshRenderers.push_back(cylinderRenderer);

	//Create Cone
	Mesh* coneMesh = MeshUtilities::CreateCone(2.0f / (M_PI * 2.0f), 0.1f, 32, 1.0f);
	Transform coneTransform = Transform(Vector3D(6.0f, 0.0f, 0.0f), Vector3D(1.0f, 1.0f, 1.0f), Vector3D(0.0f, 0.0f, 0.0f));
	MeshRenderer* coneRenderer = new MeshRenderer(coneMesh, coneTransform, shader, m_texture);
	meshs.push_back(coneMesh);
	transforms.push_back(coneTransform);
	meshRenderers.push_back(coneRenderer);
	
	//Create Plane
	Mesh* planeMesh = MeshUtilities::CreatePlane(1000);
	Transform planeTransform = Transform(Vector3D(0, -0.5f, 0.0f), Vector3D(1.0f, 1.0f, 1.0f), Vector3D(0.0f, 0.0f, 0.0f));
	MeshRenderer* planeRenderer = new MeshRenderer(planeMesh, planeTransform, shader, m_texture, Vector2D(125, 125), Vector2D((1.0 / 8.0) / 2.0f, (1.0 / 8.0) / 2.0f));
	meshs.push_back(planeMesh);
	transforms.push_back(planeTransform);
	meshRenderers.push_back(planeRenderer);

	//Create Custom Cube UV
	Texture* customCubeTexture = new Texture("Assets/CubeTextureFace.png");
	Mesh* cubeCustomMesh = MeshUtilities::CreateCustomCubeUV(1.0f, MeshUtilities::CubeUVInfo());
	Transform customCubeTransform = Transform(Vector3D(0.0f, 0.0f, 2.0f), Vector3D(1.0f, 1.0f, 1.0f), Vector3D(0.0f, 0.0f, 0.0f));
	MeshRenderer* cubeCustomRenderer = new MeshRenderer(cubeCustomMesh, customCubeTransform, shader, customCubeTexture);
	meshs.push_back(cubeCustomMesh);
	transforms.push_back(customCubeTransform);
	meshRenderers.push_back(cubeCustomRenderer);
	textures.push_back(customCubeTexture);

	//Create Sphere Custom Texture
	Texture* customSphereTexture = new Texture("Assets/SphereTexture.png");
	Transform customSphereTransform = Transform(Vector3D(2.0f, 0.0f, 2.0f), Vector3D(1.0f, 1.0f, 1.0f), Vector3D(0.0f, 0.0f, 0.0f));
	MeshRenderer* sphereCustomRenderer = new MeshRenderer(sphereMesh, customSphereTransform, shader, customSphereTexture);
	transforms.push_back(customSphereTransform);
	meshRenderers.push_back(sphereCustomRenderer);
	textures.push_back(customSphereTexture);

	//Create Custom Cylinder UV
	Texture* customCylinderTexture = new Texture("Assets/CylinderTexture16x8.png");
	float circleUVRadius = 0.16015625f;
	Mesh* cylinderCustomMesh = MeshUtilities::CreateCustomUVCylinder(2.0f / (M_PI * 2.0f), 32, 1.0f, Vector2D(0.5f, 0.25f), Vector2D(1.0f, 0.5f),
		Vector2D(0.25f, 0.75f), Vector2D(0.75f, 0.75f), circleUVRadius);
	Transform customCylinderTransform = Transform(Vector3D(4.0f, 0.0f, 2.0f), Vector3D(1.0f, 1.0f, 1.0f), Vector3D(0.0f, 0.0f, 0.0f));
	MeshRenderer* customCylinderRenderer = new MeshRenderer(cylinderCustomMesh, customCylinderTransform, shader, customCylinderTexture);
	meshs.push_back(cylinderCustomMesh);
	transforms.push_back(customCylinderTransform);
	meshRenderers.push_back(customCylinderRenderer);
	textures.push_back(customCylinderTexture);

	//Create Custom Cone UV
	Mesh* coneCustomMesh = MeshUtilities::CreateCustomConeUV(2.0f / (M_PI * 2.0f), 0.1f, 32, 1.0f, Vector2D(0.5f, 0.25f), Vector2D(1.0f, 0.5f),
		Vector2D(0.25f, 0.75f), Vector2D(0.75f, 0.75f), circleUVRadius, circleUVRadius);
	Transform customConeTransform = Transform(Vector3D(6.0f, 0.0f, 2.0f), Vector3D(1.0f, 1.0f, 1.0f), Vector3D(0.0f, 0.0f, 0.0f));
	MeshRenderer* customConeRenderer = new MeshRenderer(coneCustomMesh, customConeTransform, shader, customCylinderTexture);
	meshs.push_back(coneCustomMesh);
	transforms.push_back(customConeTransform);
	meshRenderers.push_back(customConeRenderer);
	
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

