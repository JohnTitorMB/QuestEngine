#include "Shader.h"
#include <glad/glad.h>
#include <iostream>
#include "../../Utilities/FileSystem.h"
#include "../ColorManagement/ColorConversion.h"
#include "../ColorManagement/ColorManagement.h"
#include <fstream>
#include <sstream>
#include <regex>
#include <unordered_set>


Shader::Shader(std::string vertexShaderFilePath, std::string fragmentShaderFilePath)
{
	std::string vertexShaderSource = PreProsessShader(vertexShaderFilePath);
	std::string fragmentShaderSource = PreProsessShader(fragmentShaderFilePath);

	int vertexShaderIndex = ConfigureVertexShader(vertexShaderSource.c_str());
	int fragementShaderIndex = ConfigureFragmentShader(fragmentShaderSource.c_str());
	ConfigureShaderProgram(vertexShaderIndex, fragementShaderIndex);
}

std::string Shader::PreProsessShader(std::string shaderFilePath)
{
	// Si déjà chargé, retour immédiat
	auto it = m_loadedGLSLFiles.find(shaderFilePath);
	if (it != m_loadedGLSLFiles.end()) {
		return it->second;
	}

	// Protection contre les cycles
	static std::unordered_set<std::string> filesBeingProcessed;
	if (filesBeingProcessed.count(shaderFilePath)) {
		throw std::runtime_error("Cycle detected in shader includes with file: " + shaderFilePath);
	}
	filesBeingProcessed.insert(shaderFilePath);

	std::string contents = FileSystem::get_file_contents(shaderFilePath);
	std::stringstream processed;

	std::regex includeRegex("^\\s*#include\\s+\"([^\"]+)\"\\s*$");
	std::stringstream contentStream(contents);
	std::string line;

	// Trouver le répertoire du fichier courant
	std::string directory;
	size_t lastSlash = shaderFilePath.find_last_of("/\\");
	if (lastSlash != std::string::npos) {
		directory = shaderFilePath.substr(0, lastSlash + 1);
	}

	while (std::getline(contentStream, line)) {
		std::smatch matches;
		if (std::regex_match(line, matches, includeRegex)) {
			std::string includePath = matches[1].str();
			std::string fullIncludePath = directory + includePath;
			processed << PreProsessShader(fullIncludePath);
		}
		else {
			processed << line << '\n';
		}
	}

	std::string finalShaderCode = processed.str();

	// Cache dans m_loadedGLSLFiles
	m_loadedGLSLFiles[shaderFilePath] = finalShaderCode;

	filesBeingProcessed.erase(shaderFilePath);

	return finalShaderCode;
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


void Shader::SetUniformColor(std::string name, ColorRGB value)
{
	LinearColorRGB linear = value.ToLinear();
	CustomColorRGB c = ColorConversion::ConvertRGBColor(linear, ColorManagement::Instance().GetGPUWorkingSpace());
	UseShader();
	int location = glGetUniformLocation(m_shaderProgram, name.c_str());
	glUniform4f(location, c.m_r, c.m_g, c.m_b, value.m_alpha);
}

void Shader::SetUniformColorY(std::string name, ColorRGBY value)
{
	LinearColorRGB linear = value.ToLinear();
	CustomColorRGB c = ColorConversion::ConvertRGBColor(linear, ColorManagement::Instance().GetGPUWorkingSpace());

	Vector4D encoded = Vector4D(-1.0f + 2.0f * c.m_r,
		-1.0f + 2.0f * c.m_g,
		-1.0f + 2.0f * c.m_b,
		linear.m_alpha);

	UseShader();
	int location = glGetUniformLocation(m_shaderProgram, name.c_str());
	glUniform4f(location, encoded.m_x, encoded.m_y, encoded.m_z, encoded.m_w);
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
