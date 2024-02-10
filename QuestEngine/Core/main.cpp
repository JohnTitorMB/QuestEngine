#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Window.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "../Utilities/MeshUtilities.h"
#include "../Utilities/FileSystem.h"
#include "../Core/Light.h"
#include "../Core/DirectionalLight.h"
#include "../Core/PointLight.h"
#include "../Core/SpotLight.h"
#include "../Math/Mathf.h"
#include "LightingSettings.h"

# define M_PI           3.14159265358979323846f  /* pi */


int CreateCube(Vector3D position, Quaternion rotation, Vector3D scale, Mesh* mesh, Shader* shader, Material* material, Texture* texture, std::vector<Transform>& transforms, std::vector<MeshRenderer*>& meshRenderers)
{
	Transform cubeTransform = Transform(position, scale, rotation);
	MeshRenderer* cubeRenderer = new MeshRenderer(mesh,cubeTransform, shader, material, texture);
	transforms.push_back(cubeTransform);
	meshRenderers.push_back(cubeRenderer);

	return meshRenderers.size()-1;
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
	//Object
	std::vector<Mesh*> meshs;
	std::vector<Transform> transforms;
	std::vector<MeshRenderer*> meshRenderers;
	std::vector<Texture*> textures;
	std::vector<Material*> materials;
	std::vector<Light*> lights;

	//Initialise Opengl Window
	Window* window = new Window(1920, 1080, new char[] {"Opengl Window"});
	glEnable(GL_DEPTH_TEST);

	//Initialise Shader
	const char* vsFilename = "Assets/BlinnPhongShader.vert";	
	std::string vertexShaderSourceString = FileSystem::get_file_contents(vsFilename);
	const char* vertexShaderSource = vertexShaderSourceString.c_str();
	
	const char* fsFilename = "Assets/BlinnPhongShader.frag";
	std::string fragmentShaderSourceString = FileSystem::get_file_contents(fsFilename);
	const char* fragmentShaderSource = fragmentShaderSourceString.c_str();
	Shader* shader = new Shader(vertexShaderSource, fragmentShaderSource);

	//IntialiseCamera
	Vector3D cameraPosition = Vector3D(0.0f, 0.0f,-10.8f);
	Vector3D cameraAngles = Vector3D(0.0f, 0.0f,0.0f);	
	
	Camera* camera = new Camera(cameraPosition, cameraAngles, 10,true, false);
	camera->SetFov(60);

	//Cube Texture
	Texture* cubeTexture = new Texture("Assets/CubeTextureFace.png");
	textures.push_back(cubeTexture);

	Texture* cubeSpecularTexture = new Texture("Assets/CubeTextureSpecular.png");
	textures.push_back(cubeSpecularTexture);
	
	// Set Scene Configuration
	Mesh* cubeMesh = MeshUtilities::CreateCustomCubeUV(1.0f, MeshUtilities::CubeUVInfo());
	meshs.push_back(cubeMesh);

	Material* material2 = new Material(cubeTexture, cubeTexture, cubeSpecularTexture, Color(0, 1, 0, 1), Color(0, 1, 0, 1), Color(1, 1, 0, 1));
	materials.push_back(material2);
	Vector3D position2 = Vector3D(0.0f, -3.0f, 0.0f);
	Quaternion rotation2 = Quaternion::AxisAngle(Vector3D::Right,180);
	Vector3D scale2 = Vector3D(6.0f, 0.01f, 6.0f);
	CreateCube(position2, rotation2, scale2, cubeMesh, shader, material2, cubeTexture, transforms, meshRenderers);

	Material* material3 = new Material(cubeTexture, cubeTexture, cubeSpecularTexture, Color(0, 0, 1, 1), Color(0, 0, 1, 1), Color(1, 1, 1, 1));
	materials.push_back(material3);
	Vector3D position3 = Vector3D(-3.0f, 0.0f, 0.0f);
	Quaternion rotation3 = Quaternion::AxisAngle(Vector3D::Up, 180);
	Vector3D scale3 = Vector3D(0.01f, 6.0f, 6.0f);
	CreateCube(position3, rotation3, scale3, cubeMesh, shader, material3, cubeTexture, transforms, meshRenderers);

	Material* material4 = new Material(cubeTexture, cubeTexture, cubeSpecularTexture, Color(0, 1, 1, 1), Color(0, 1, 1, 1), Color(1, 1, 1, 1));
	materials.push_back(material4);
	Vector3D position4 = Vector3D(3.0f, 0.0f, 0.0f);
	Quaternion rotation4 = Quaternion::AxisAngle(Vector3D::Up, 180);
	Vector3D scale4 = Vector3D(0.01f, 6.0f, 6.0f);
	CreateCube(position4, rotation4, scale4, cubeMesh, shader, material4, cubeTexture, transforms, meshRenderers);

	Material* material5 = new Material(cubeTexture, cubeTexture, cubeSpecularTexture, Color(1, 0, 1, 1), Color(1, 0, 1, 1), Color(1, 1, 1, 1));
	materials.push_back(material5);
	Vector3D position5 = Vector3D(0.0f, 0.0f, 3.0f);
	Quaternion rotation5 = Quaternion::Identity();
	Vector3D scale5 = Vector3D(6.0f, 6.0f, 0.01f);
	CreateCube(position5, rotation5, scale5, cubeMesh, shader, material5, cubeTexture, transforms, meshRenderers);

	Material* material = new Material(cubeTexture, cubeTexture, cubeSpecularTexture, Color(1, 0, 0, 1), Color(1, 0, 0, 1), Color(1, 1, 1, 1));
	materials.push_back(material);
	Vector3D position6 = Vector3D(0.0f, 3.0f, 0.0f);
	Quaternion rotation6 = Quaternion::AxisAngle(Vector3D::Right, 180);
	Vector3D scale6 = Vector3D(6.0f, 0.01f, 6.0f);
	CreateCube(position6, rotation6, scale6, cubeMesh, shader, material, cubeTexture, transforms, meshRenderers);


	//Lightning Setting
	LightingSettings::m_globalAmbiantColor = Color(0.5, 0.5, 0.5, 1.0f);
	
	bool enableDirectionalLight = false;
	bool enablePointLight = false;
	bool enableSpotLight = false;
	
	//Initialise Directional Light
	int directionVisualLightIndex = -1;
	DirectionalLight* directionalLight = nullptr;
	if (enableDirectionalLight)
	{
		Texture* LightVisualTexture = new Texture("Assets/DirectionalLightTexture.png");
		textures.push_back(LightVisualTexture);

		Material* lightVisualMaterial = new Material(LightVisualTexture, LightVisualTexture, LightVisualTexture, Color(10, 10, 10, 1), Color(0, 0, 0, 1), Color(0, 0, 0, 1));
		materials.push_back(lightVisualMaterial);

		directionalLight = new DirectionalLight();
		directionalLight->m_direction = Vector3D(0, 0, 1).Normalized();
		directionalLight->m_ambiantColor = Color(0, 0, 0, 1.0f);
		directionalLight->m_specularColor = Color(0.3f, 0.3f, 0.3f, 1.0f);
		directionalLight->m_intensity = 3.0f;
		lights.push_back(directionalLight);

		Vector3D position = Vector3D(0.0f, 0, 1.0f);
		Quaternion rotation = Quaternion::Identity();
		Vector3D scale = Vector3D(1.0f, 1.0f, 1.0f);
		directionVisualLightIndex = CreateCube(position, rotation, scale, cubeMesh, shader, lightVisualMaterial, cubeTexture, transforms, meshRenderers);
	}

	//Initialise Point Light
	PointLight* pointLight = nullptr;
	if (enablePointLight)
	{
		Texture* LightVisualTexture = new Texture("Assets/PointLightTexture.png");
		textures.push_back(LightVisualTexture);

		Material* lightVisualMaterial = new Material(LightVisualTexture, LightVisualTexture, LightVisualTexture, Color(10, 10, 10, 1), Color(0, 0, 0, 1), Color(0, 0, 0, 1));
		materials.push_back(lightVisualMaterial);


		pointLight = new PointLight();
		pointLight->m_position = Vector3D(0, 0, 0);
		pointLight->m_specularColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
		pointLight->m_linearValue = 0.09f;
		pointLight->m_intensity = 2.0f;
		pointLight->m_quadraticValue = 0.032f;
		lights.push_back(pointLight);

		materials.push_back(material);
		Vector3D position = Vector3D(0.0f, 0, 0.0f);
		Quaternion rotation = Quaternion::Identity();
		Vector3D scale = Vector3D(1.0f, 1.0f, 1.0f);
		CreateCube(position, rotation, scale, cubeMesh, shader, lightVisualMaterial, cubeTexture, transforms, meshRenderers);
	}

	//Initialise Spot Light
	int spotLightVisualIndex0 = -1;
	int spotLightVisualIndex1 = -1;
	int spotLightVisualIndex2 = -1;
	SpotLight* spotLight = nullptr;
	SpotLight* spotLight2 = nullptr;
	SpotLight* spotLight3 = nullptr;
	if (enableSpotLight)
	{
		Texture* LightVisualTexture = new Texture("Assets/SpotLightTexture.png");
		textures.push_back(LightVisualTexture);

		Material* lightVisualMaterial = new Material(LightVisualTexture, LightVisualTexture, LightVisualTexture, Color(10, 10, 10, 1), Color(0, 0, 0, 1), Color(0, 0, 0, 1));
		materials.push_back(lightVisualMaterial);

		spotLight = new SpotLight();
		spotLight->m_position = Vector3D(3.0f, 0, 0);
		spotLight->m_direction = Vector3D(0, -1, 0);
		spotLight->m_linearValue = 0.09f;
		spotLight->m_ambiantColor = Color(0.15f, 0.15f, 0.15f, 1.0f);
		spotLight->m_spotAngle = 10;
		spotLight->m_spotSmoothValue = 0.2f;
		spotLight->m_quadraticValue = 0.032f;
		lights.push_back(spotLight);

		spotLight2 = new SpotLight();
		spotLight2->m_position = Vector3D(0.0f, 0, 0);
		spotLight2->m_direction = Vector3D(0, -1, 0);
		spotLight2->m_linearValue = 0.09f;
		spotLight2->m_ambiantColor = Color(0.15f, 0.15f, 0.15f, 1.0f);
		spotLight2->m_spotAngle = 10;
		spotLight2->m_spotSmoothValue = 0.5f;
		spotLight2->m_quadraticValue = 0.032f;
		lights.push_back(spotLight2);

		spotLight3 = new SpotLight();
		spotLight3->m_position = Vector3D(-1.5f, 0, 0);
		spotLight3->m_direction = Vector3D(0, -1, 0);
		spotLight3->m_linearValue = 0.09f;
		spotLight3->m_ambiantColor = Color(0.15f, 0.15f, 0.15f, 1.0f);
		spotLight3->m_spotAngle = 10;
		spotLight3->m_spotSmoothValue = 1.0f;
		spotLight3->m_quadraticValue = 0.032f;
		lights.push_back(spotLight3);

		Vector3D position7 = Vector3D(3.0f, 0.0f, 0.0f);
		Quaternion rotation7 = Quaternion::Identity();
		Vector3D scale7 = Vector3D(1.0f, 1.0f, 1.0f);
		spotLightVisualIndex0 = CreateCube(position7, rotation7, scale7, cubeMesh, shader, lightVisualMaterial, cubeTexture, transforms, meshRenderers);
		spotLightVisualIndex1 = CreateCube(Vector3D(0.0f, 0.0f, 0.0f), rotation7, scale7, cubeMesh, shader, lightVisualMaterial, cubeTexture, transforms, meshRenderers);
		spotLightVisualIndex2 = CreateCube(Vector3D(-1.5f, 0.0f, 0.0f), rotation7, scale7, cubeMesh, shader, lightVisualMaterial, cubeTexture, transforms, meshRenderers);

	}


	// Game Engine Loop
	float cameraAngle = 0.0f;
	float stepLight = 1.0f;
	while (!glfwWindowShouldClose(window->GetWindow()))
	{
		glClearColor(0.3f, 0.3f, 0.3f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

		for (int i = 0; i < meshRenderers.size(); i++)
			meshRenderers[i]->Draw(camera,lights, window);

		if (enableDirectionalLight)
		{
			directionalLight->m_direction = Quaternion::AxisAngle(Vector3D(0, 1, 0).Normalized(), 1.0f) * directionalLight->m_direction;
			SetPosition(directionVisualLightIndex, directionalLight->m_direction, meshRenderers);
		}

		if (enablePointLight)
			pointLight->m_intensity = Mathf::PingPong(stepLight, 3.0f);

		if (enableSpotLight)
		{
			spotLight->m_position = Vector3D(3.0f, 0.0f, -3.0f + Mathf::PingPong(stepLight, 6.0f));
			spotLight2->m_position = Vector3D(0.0f, 0.0f, -3.0f + Mathf::PingPong(stepLight, 6.0f));
			spotLight3->m_position = Vector3D(-1.5f, 0.0f, -3.0f + Mathf::PingPong(stepLight, 6.0f));
			SetPosition(spotLightVisualIndex0, spotLight->m_position, meshRenderers);
			SetPosition(spotLightVisualIndex1, spotLight2->m_position, meshRenderers);
			SetPosition(spotLightVisualIndex2, spotLight3->m_position, meshRenderers);
		}

		stepLight += 0.01f;

		glfwSwapBuffers(window->GetWindow());
		glfwPollEvents();
	}

	delete window;
	window = nullptr;

	delete camera;
	camera = nullptr;

	delete shader;
	shader = nullptr;

	for (int i = 0; i < meshs.size(); i++)
	{
		delete meshs[i];
		meshs[i] = nullptr;
	}
	meshs.clear();

	for (int i = 0; i < meshRenderers.size(); i++)
	{
		delete meshRenderers[i];
		meshRenderers[i] = nullptr;
	}
	meshRenderers.clear();

	for (int i = 0; i < textures.size(); i++)
	{
		delete textures[i];
		textures[i] = nullptr;
	}
	textures.clear();

	for (int i = 0 ; i < lights.size(); i++)
	{
		delete lights[i];
		lights[i] = nullptr;
	}
	lights.clear();

	return 0;
}

