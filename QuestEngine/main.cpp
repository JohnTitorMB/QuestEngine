#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Fr : Squelette de l'initialisation d'une fenêtre Opengl avec GLFW et glad
// En : Skeleton of the initialization of Opengl window with GLFW and Glad
int main()
{
	// Fr : GLFW initialisation
	// En : GLFW initialization
	int result = glfwInit();
	if (result == GLFW_TRUE)
		std::cout << "GLFW is initialised" << std::endl;
	else
		std::cout << "GLFW is not initialised" << std::endl;

	// Fr/En : Opengl Context configuration
	glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Fr : Création de la fenêtre
	// En : Window creation
	int width = 1280;
	int height = 720;
	const char* title = "First Opengl Window";
	
	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window)
	{
		std::cout << "GLFW Window creation FAILED !" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Fr : Lié le contexte opengl à la fenêtre GLFW
	// En : Link opengl context to GLFW window
	glfwMakeContextCurrent(window);
		
	// Fr : Chargement des fonctions Opengl
	// En : Opengl function loading
	result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (!result)
	{
		std::cout << "Glad load gl function FAILED" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Fr : boucle de rendu
	// En : rendering loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(1.0f, 1.0f, 1.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);	
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}