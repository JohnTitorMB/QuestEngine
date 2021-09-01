#include "Window.h"
#include <iostream>

Window::Window(int width, int height, char* title)
{
	m_width = width;
	m_height = height;
	m_title = title;

	InitialiseGLFW();
	CreateWindow();
	InitialiseGLAD();
}

Window::Window(int width, int height, char* title, int glMajorVersion, int glMinorVersion)
{
	m_width = width;
	m_height = height;
	m_title = title;
	m_glMajorVersion = glMajorVersion;
	m_glMinorVersion = glMinorVersion;

	InitialiseGLFW();
	CreateWindow();
	InitialiseGLAD();
}

int Window::InitialiseGLFW()
{
	int result = glfwInit();
	if (result == GLFW_TRUE)
	{
		std::cout << "GLFW is initialised" << std::endl;
	}	
	else
	{
		std::cout << "GLFW is not initialised" << std::endl;
		return -1;
	}

	glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	return 1;
}

int Window::InitialiseGLAD()
{
	int result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (!result)
	{
		std::cout << "Glad load gl function FAILED" << std::endl;
		glfwTerminate();
		return -1;
	}

	return 1;
}

int Window::CreateWindow()
{
	int width = 1280;
	int height = 720;
	const char* title = "First Opengl Window";
	m_window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!m_window)
	{
		std::cout << "GLFW Window creation FAILED !" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(m_window);

	return 1;
}

GLFWwindow* Window::GetWindow() const
{
	return m_window;
}