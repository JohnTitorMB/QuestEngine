#include "Window.h"
#include <iostream>
#include "World.h"
#include "../Game/CameraController.h"

Window::Window(int width, int height, char* title)
{
	m_width = width;
	m_height = height;
	m_title = title;
	InitialiseGLFW();
	CreateWindow();
	InitialiseGLAD();

	glfwSetWindowUserPointer(m_window, this);
	glfwSetFramebufferSizeCallback(m_window, SetTheFrameBufferSize); // works fine

	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	InitialiseGLFWCallback();
}

void Window::InitialiseGLFWCallback()
{
	///TODO : use mods and handle special characters (-""...) using glfwSetCharCallback, check keyboard language, azerty, qwerty
	glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			Window* win = (Window*)glfwGetWindowUserPointer(window);

			InputSystem* inputSystem = InputSystem::Instance();

			if (action == GLFW_PRESS || action == GLFW_RELEASE)
				inputSystem->RegisterInput(win, ConvertGLFWInputKeyCode(key), ConvertGLFWInputAction(action));
		});

	glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int mouseButton, int action, int mods)
		{
			Window* win = (Window*)glfwGetWindowUserPointer(window);

			InputSystem* inputSystem = InputSystem::Instance();

			if (action == GLFW_PRESS || action == GLFW_RELEASE)
				inputSystem->RegisterInput(win, ConvertGLFWInputMouseCode(mouseButton), ConvertGLFWInputAction(action));
		});

	glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset)
		{
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			InputSystem* inputSystem = InputSystem::Instance();
			if (inputSystem->GetInputState(InputCode::E_MOUSE_SCROLL_X) != (float)xoffset)
				inputSystem->RegisterInput(win, InputCode::E_MOUSE_SCROLL_X, xoffset);

			if (inputSystem->GetInputState(InputCode::E_MOUSE_SCROLL_Y) != (float)yoffset)
				inputSystem->RegisterInput(win, InputCode::E_MOUSE_SCROLL_Y, yoffset);
		});

	glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos)
		{
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			InputSystem* inputSystem = InputSystem::Instance();

			if (inputSystem->GetInputState(InputCode::E_MOUSE_POS_X) != (float)xpos)
				inputSystem->RegisterInput(win, InputCode::E_MOUSE_POS_X, xpos);

			if (inputSystem->GetInputState(InputCode::E_MOUSE_POS_Y) != (float)ypos)
				inputSystem->RegisterInput(win, InputCode::E_MOUSE_POS_Y, ypos);
		});
}

void Window::LockMouseInput(bool lock)
{
	if (lock)
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

InputCode Window::ConvertGLFWInputKeyCode(int keyCode)
{	
	return (InputCode)keyCode;
}

InputCode Window::ConvertGLFWInputMouseCode(int mouseInput)
{
	if (mouseInput == 0)
		return InputCode::E_MOUSE_BUTTON_1;
	else if (mouseInput == 1)
		return InputCode::E_MOUSE_BUTTON_2;
	else if (mouseInput == 2)
		return InputCode::E_MOUSE_BUTTON_3;
	else if (mouseInput == 3)
		return InputCode::E_MOUSE_BUTTON_4;
	else if (mouseInput == 4)
		return InputCode::E_MOUSE_BUTTON_5;
	else if (mouseInput == 5)
		return InputCode::E_MOUSE_BUTTON_6;
	else if (mouseInput == 6)
		return InputCode::E_MOUSE_BUTTON_7;
	else if (mouseInput == 7)
		return InputCode::E_MOUSE_BUTTON_8;

	return InputCode::E_UNKNOWN;
}

float Window::ConvertGLFWInputAction(int action)
{
	if (action == GLFW_PRESS)
		return 1.0f;
	else if (action == GLFW_RELEASE)
		return 0.0f;
	else if(action == GLFW_REPEAT)
		return 0.0f;
	else
		return 0.0f;
}

void Window::SetTheFrameBufferSize(GLFWwindow* window, int width, int height)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->RefreshWidthAndHeight();
	glViewport(0, 0, width, height);
}

Window::Window(int width, int height, char* title, int glMajorVersion, int glMinorVersion)
{
	
	m_glMajorVersion = glMajorVersion;
	m_glMinorVersion = glMinorVersion;

	InitialiseGLFW();
	CreateWindow();
	InitialiseGLAD();
}

Window::~Window()
{
	delete m_title;
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

	glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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
	int width = m_width;
	int height = m_height;
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

float Window::GetWidth()const
{
	return m_width;
}

float Window::GetHeight()const
{
	return m_height;
}

void Window::RefreshWidthAndHeight()
{
	glfwGetWindowSize(m_window, &m_width, &m_height);
}