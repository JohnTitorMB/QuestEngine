#ifndef _SHAPE_H_
#define _SHAPE_H_
#include <glad/glad.h>
#include "Shader.h"

enum GLDrawType
{
	GLSTREAM_DRAW = 0x88E0,
	GLSTATIC_DRAW = 0x88E4,
	GLDYNAMIC_DRAW = 0x88E8
};
class Shape
{
public :
	Shape(Shader* shader, GLDrawType glDrawType, int verticesSizeof, float vertices[], int indicesSizeof, unsigned int indices[]);
	static Shape* CreateTriangle(Shader* shader, GLDrawType glDrawType,
		float x1, float y1, float z1,
		float x2, float y2, float z2,
		float x3, float y3, float z3);

	static Shape* CreateRectangle(Shader* shader, GLDrawType glDrawType,
		float width, float height);

	
	void Draw();

private :
	GLuint m_vbo;
	GLuint m_ebo;
	GLDrawType m_glDrawType;
	Shader* m_shader;
	int triangleCount = 1;
	void ConfigureVBO(int verticesSizeof,float vertices[]);
	void ConfigureEBO(int indicesSizeof, unsigned int indices[]);
};

#endif // !_SHAPE_H_



