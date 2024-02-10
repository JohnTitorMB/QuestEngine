#ifndef _SHADER_H_
#define _SHADER_H_
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Math/Vector2D.h"
#include "../Math/Matrix3x3.h"
#include "../Math/Matrix4x4.h"
#include "Color.h"
#include <string>
class Shader
{
public :
	Shader(const char* vertexShaderSource, const char* fragmentShaderSource);
	void UseShader();
	void SetUniformVector2D(std::string name, Vector2D value);
	void SetUniformVector3D(std::string name, Vector3D value);
	void SetUniformVector4D(std::string name, float v0, float v1, float v2, float v3 );
	void SetUniformVector4D(std::string name, Vector4D value);
	void SetUniformFloat(std::string name, float value);
	void SetUniformMatrix3x3(std::string name, Matrix3x3 value);
	void SetUniformMatrix4x4(std::string name, Matrix4x4 value);
	void SetUniformColor(std::string name, Color value);
	void SetUniformInt(std::string name, int value);

private:
	int m_shaderProgram;

	int ConfigureVertexShader(const char* vertexShaderSource);
	int ConfigureFragmentShader(const char* fragmentShaderSource);
	void ConfigureShaderProgram(int vertexShaderIndex, int fragementShaderIndex);
	
};

#endif // _SHADER_H_


