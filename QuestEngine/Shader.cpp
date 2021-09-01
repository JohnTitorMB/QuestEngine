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