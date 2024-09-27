#ifndef _WINDOW_H_
#define _WINDOW_H_
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Inputs/InputSystem.h"

class Window
{
	
public :
	Window(int width, int height, char* title);
	~Window();
	Window(int width, int height, char* title, int glMajorVersion, int glMinorVersion);
	GLFWwindow* GetWindow() const;
	int GetWidth()const;
	int GetHeight()const;
	void RefreshWidthAndHeight();
	void SetMSAASample(unsigned int msaaSample);
	static void SetTheFrameBufferSize(GLFWwindow* window, int width, int height);
	static InputCode ConvertGLFWInputKeyCode(int keyCode);

	static InputCode ConvertGLFWInputMouseCode(int mouseInput);
	static float ConvertGLFWInputAction(int action);

	void LockMouseInput(bool lock);
	float lastPositionX = 0.0f;
	float lastPositionY = 0.0f;
	void SetVsync(bool value);
	bool GetVsync()const;
private:
	
	int m_glMajorVersion = 3;
	int m_glMinorVersion = 2;
	int m_msaaSample = 0;
	int m_width = 1280;
	int m_height = 720;
	char* m_title;
	bool m_vsync = true;


	GLFWwindow* m_window;
	int InitialiseGLFW();
	int InitialiseGLAD();
	int CreateWindow(GLFWwindow* sharedContext = nullptr);

	void InitialiseGLFWCallback();
};

#endif // !_WINDOW_H_


