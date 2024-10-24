#include "Shader.h"
#include <glad/glad.h>
#include <iostream>
#include "../../Utilities/FileSystem.h"
Shader::Shader(std::string vertexShaderFilePath, std::string fragmentShaderFilePath)
{
	std::string vertexShaderSource = FileSystem::get_file_contents(vertexShaderFilePath);
	std::string fragmentShaderSource = FileSystem::get_file_contents(fragmentShaderFilePath);

	int vertexShaderIndex = ConfigureVertexShader(vertexShaderSource.c_str());
	int fragementShaderIndex = ConfigureFragmentShader(fragmentShaderSource.c_str());
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

	GLint numUniforms = 0;
	glGetProgramiv(m_shaderProgram, GL_ACTIVE_UNIFORMS, &numUniforms);
}

void Shader::SetUniformVector4D(std::string name, float v0, float v1, float v2, float v3)
{
	UseShader();
	int location = glGetUniformLocation(m_shaderProgram, name.c_str());
	glUniform4f(location, v0, v1, v2, v3);
}

void Shader::SetUniformVector4D(std::string name, Vector4D value)
{
	int location = glGetUniformLocation(m_shaderProgram, name.c_str());
	glUniform4f(location, value.m_x, value.m_y, value.m_z, value.m_w);
}


void Shader::SetUniformColor(std::string name, Color value)
{
	UseShader();
	int location = glGetUniformLocation(m_shaderProgram, name.c_str());
	glUniform4f(location, value.m_r, value.m_g, value.m_b, value.m_a);
}

void Shader::SetUniformVector2D(std::string name, Vector2D value)
{
	int location = glGetUniformLocation(m_shaderProgram, name.c_str());
	glUniform2f(location, value.m_x, value.m_y);
}

void Shader::SetUniformVector3D(std::string name, Vector3D value)
{
	int location = glGetUniformLocation(m_shaderProgram, name.c_str());
	glUniform3f(location, value.m_x, value.m_y, value.m_z);
}

void Shader::SetUniformFloat(std::string name, float value)
{
	int location = glGetUniformLocation(m_shaderProgram, name.c_str());
	glUniform1f(location, value);
}

void Shader::SetUniformInt(std::string name, int value)
{
	int location = glGetUniformLocation(m_shaderProgram, name.c_str());
	glUniform1i(location, value);
}

void Shader::SetUniformFloatArray(std::string name, std::vector<float> value)
{
	int location = glGetUniformLocation(m_shaderProgram, name.c_str());
	glUniform1fv(location, value.size(), value.data());
}

void Shader::SetUniformMatrix3x3(std::string name, Matrix3x3 value)
{
	int location = glGetUniformLocation(m_shaderProgram, name.c_str());

	glUniformMatrix3fv(location, 1, GL_FALSE, value.GetAsArray().data());
}

void Shader::SetUniformMatrix4x4(std::string name, Matrix4x4 value)
{
	int location = glGetUniformLocation(m_shaderProgram, name.c_str());

	glUniformMatrix4fv(location, 1, GL_FALSE, value.GetAsArray().data());
}
