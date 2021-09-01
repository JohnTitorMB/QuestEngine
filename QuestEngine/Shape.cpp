#include "Shape.h"
#include <iostream>
Shape::Shape(Shader* shader, GLDrawType glDrawType, int verticesSizeof, float vertices[], int indicesSizeof, unsigned int indices[])
{
	m_shader = shader;
	m_glDrawType = glDrawType;
	ConfigureVBO(verticesSizeof,vertices);
	ConfigureEBO(indicesSizeof, indices);

	std::cout << "Vertex Sahder Compilation Failed\n" << sizeof(unsigned int) << std::endl;

	triangleCount = (int)(indicesSizeof / sizeof(int));
}

Shape* Shape::CreateTriangle(Shader* shader, GLDrawType glDrawType,
	float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3)
{
	float vertices[9] = { x1, y1, z1, x2, y2, z2, x3, y3, z3 };
	unsigned int indices[3] = { 0,1,2 };


	Shape* triangle = new Shape(shader, glDrawType, sizeof(vertices), vertices, sizeof(indices), indices);
	return triangle;
}

Shape* Shape::CreateRectangle(Shader* shader, GLDrawType glDrawType,
	float width, float height)
{
	float vertices[12] =
	{ -width / 2.0f, -height / 2.0f, 0.0f, //bottom-Left
	  width / 2.0f, -height / 2.0f, 0.0f, //bottom-right
	  width / 2.0f, height / 2.0f, 0.0f, // top-right
	  -width / 2.0f, height / 2.0f, 0.0f // top-left
	};

	unsigned int indices[6] =
	{
		0,2,1,
		0,3,2
	};


	Shape* rectangle = new Shape(shader, glDrawType, sizeof(vertices), vertices, sizeof(indices), indices);
	return rectangle;
}


void Shape::Draw()
{

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	m_shader->UseShader();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	//glDrawArrays(GL_TRIANGLES, 0, triangleCount);
	glDrawElements(GL_TRIANGLES, triangleCount, GL_UNSIGNED_INT,0);

}

void Shape::ConfigureVBO(int verticesSizeof, float vertices[])
{
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, verticesSizeof, vertices, m_glDrawType);
}

void Shape::ConfigureEBO(int indicesSizeof, unsigned int indices[])
{
	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSizeof, indices, m_glDrawType);
}