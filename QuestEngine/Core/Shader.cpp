#include "Shader.h"
#include <glad/glad.h>
#include <iostream>

Shader::Shader(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	int vertexShaderIndex = ConfigureVertexShader(vertexShaderSource);
	int fragementShaderIndex = ConfigureFragmentShader(fragmentShaderSource);
	ConfigureShaderProgram(vertexShaderIndex, fragementShaderIndex);
}

void Shader::UseShader()
{
	glUseProgram(m_shaderProgram);
}

int Shader::ConfigureVertexShader(const char* vertexShaderSource)
{
	//Vertex Shader Configuration
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	

	int  success = 0;
	char infoLog[512];

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Vertex Sahder Compilation Failed\n" << infoLog << std::endl;
	}

	return vertexShader;
}

int Shader::ConfigureFragmentShader(const char* fragmentShaderSource)
{
	//Fragement Shader Configuration
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	


	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	int  success = 0;
	char infoLog[512];

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Fragment Shader Compilation Failed\n" << infoLog << std::endl;
	}

	return fragmentShader;
}

void Shader::ConfigureShaderProgram(int vertexShaderIndex, int fragementShaderIndex)
{
	//Shader Program Configuration

	m_shaderProgram = glCreateProgram();

	glAttachShader(m_shaderProgram, vertexShaderIndex);
	glAttachShader(m_shaderProgram, fragementShaderIndex);

	glLinkProgram(m_shaderProgram);

	int  success = 0;
	char infoLog[512];

	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
		std::cout << "Shader Program link Compilation Failed\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShaderIndex);
	glDeleteShader(fragementShaderIndex);
}

void Shader::SetUniformVector4D(std::string name, float v0, float v1, float v2, float v3)
{
	UseShader();
	int location = glGetUniformLocation(m_shaderProgram, name.c_str());
	glUniform4f(location, v0, v1, v2, v3);
}

void Shader::SetUniformVector2D(std::string name, Vector2D value)
{
	int location = glGetUniformLocation(m_shaderProgram, name.c_str());
	glUniform2f(location, value.m_x, value.m_y);
}

void Shader::SetUniformFloat(std::string name, float value)
{
	int location = glGetUniformLocation(m_shaderProgram, name.c_str());
	glUniform1f(location, value);
}