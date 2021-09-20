#ifndef _WINDOW_H_
#define _WINDOW_H_
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class Window
{
public :
	Window(int width, int height, char* title);
	~Window();
	Window(int width, int height, char* title, int glMajorVersion, int glMinorVersion);
	GLFWwindow* GetWindow() const;
private:
	int m_glMajorVersion = 3;
	int m_glMinorVersion = 2;

	int m_width = 1280;
	int m_height = 720;
	char* m_title;

	GLFWwindow* m_window;
	int InitialiseGLFW();
	int InitialiseGLAD();
	int CreateWindow();
};

#endif // !_WINDOW_H_


