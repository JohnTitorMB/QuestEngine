#ifndef _SHADER_H_
#define _SHADER_H_
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Vector2D.h"
#include <string>
class Shader
{
public :
	Shader(const char* vertexShaderSource, const char* fragmentShaderSource);
	void UseShader();
	void SetUniformVector2D(std::string name, Vector2D value);
	void SetUniformFloat(std::string name, float value);

private:
	int m_shaderProgram;

	int ConfigureVertexShader(const char* vertexShaderSource);
	int ConfigureFragmentShader(const char* fragmentShaderSource);
	void ConfigureShaderProgram(int vertexShaderIndex, int fragementShaderIndex);
	
};

#endif // _SHADER_H_


