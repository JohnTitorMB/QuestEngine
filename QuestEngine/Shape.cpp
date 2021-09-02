#include "Shape.h"
#include <iostream>
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>

#define  CIRCLESIDE 30


Shape::Shape(Shader* shader, GLDrawType glDrawType, int verticesSizeof, float vertices[], int indicesSizeof, unsigned int indices[], ShapeType shapeType, bool enableWireframe)
{
	m_shader = shader;
	m_glDrawType = glDrawType;
	m_shapeType = shapeType;
	m_enableWireframe = enableWireframe;
	ConfigureVBO(verticesSizeof,vertices);
	ConfigureEBO(indicesSizeof, indices);

	std::cout << "Vertex Sahder Compilation Failed\n" << sizeof(unsigned int) << std::endl;
	m_shapeTypeCount = (int)(indicesSizeof / sizeof(int));
}

Shape* Shape::CreateTriangle(Shader* shader, GLDrawType glDrawType,
	float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3, bool enableWireframe)
{
	float vertices[9] = { x1, y1, z1, x2, y2, z2, x3, y3, z3 };
	unsigned int indices[3] = { 0,1,2 };


	Shape* triangle = new Shape(shader, glDrawType, sizeof(vertices), vertices, sizeof(indices), indices, ShapeType::TRIANGLE,enableWireframe);
	return triangle;
}

Shape* Shape::CreateRectangle(Shader* shader, GLDrawType glDrawType,
	float width, float height, bool enableWireframe)
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


	Shape* rectangle = new Shape(shader, glDrawType, sizeof(vertices), vertices, sizeof(indices), indices, ShapeType::TRIANGLE, enableWireframe);
	return rectangle;
}

Shape* Shape::CreateRegularConvexPolygon(Shader* shader, GLDrawType glDrawType, int sideCount, float radius, bool enableWireframe)
{
	int verticesCount = sideCount * 3;
	int indicesCount = (sideCount - 2) * 3;
	float* vertices = new float[verticesCount];
	unsigned int* indices = new unsigned int[indicesCount];

	bool isPair = (sideCount % 2) == 0;

	float angle = 0;
	if (isPair)
		angle = M_PI / 2.0f + 2.0f * M_PI / sideCount / 2.0f;
	else
		angle = M_PI / 2.0f;

	//InitialiseVertices
	for (int i = 0; i < sideCount; i++)
	{
		float x = cos(angle) * radius;
		float y = sin(angle) * radius;

		angle -= 2.0f * M_PI / sideCount;

		// Exemple square : 1 = xyz  2 = xyz 3 = xyz 4 = xyz
		vertices[i * 3] = x;
		vertices[i * 3 + 1] = y;
		vertices[i * 3 + 2] = 0; // Z axis
	}
	//InitialiseTriangleIndex
	for (int triangleIndex = 0; triangleIndex < (sideCount - 2); triangleIndex++)
	{
		// Exemple square : triangle 1 = 3 sommet  trianglme 2 = 3 sommet également

		indices[triangleIndex * 3] = 0;
		indices[triangleIndex * 3 + 1] = triangleIndex + 1;
		indices[triangleIndex * 3 + 2] = triangleIndex + 2;		
	}

	Shape* polygon = new Shape(shader, glDrawType, verticesCount * sizeof(float), vertices, indicesCount * sizeof(int), indices, ShapeType::TRIANGLE, enableWireframe);
	return polygon;
}

Shape* Shape::CreateCircle(Shader* shader, GLDrawType glDrawType, float radius, bool enableWireframe)
{
	return CreateRegularConvexPolygon(shader, glDrawType, CIRCLESIDE, radius, enableWireframe);
}

Shape* Shape::CreateGrid(Shader* shader, GLDrawType glDrawType, int widthTileCount, int heightTileCount, bool enableWireframe)
{
	int verticesCount = ((widthTileCount + 1) + (heightTileCount + 1)) * 2 * 3;
	float* vertices = new float[verticesCount];

	int indicesCount = ((widthTileCount + 1) + (heightTileCount + 1)) * 2;
	unsigned int* indices = new unsigned int[indicesCount];

	float tileXSize = 2.0f / widthTileCount;
	for (int x = 0; x < (widthTileCount + 1); x++)
	{
		int vertexIndex = x * 2;
		vertices[vertexIndex * 3] = -1.0f + x * tileXSize;
		vertices[vertexIndex * 3+1] = -1.0f;
		vertices[vertexIndex * 3 + 2] = 0.0f;

		vertices[(vertexIndex + 1) * 3] = -1.0f + x * tileXSize;
		vertices[(vertexIndex + 1) * 3 + 1] = 1.0f;
		vertices[(vertexIndex + 1) * 3 + 2] = 0.0f;


		indices[vertexIndex] = vertexIndex;
		indices[vertexIndex+1] = vertexIndex+1;
	}

	float tileYSize = 2.0f / heightTileCount;
	for (int y = 0; y < (heightTileCount + 1); y++)
	{
		int vertexIndex = (widthTileCount + 1.0f) * 2.0f + y * 2.0f;
		vertices[vertexIndex * 3] = -1.0f;
		vertices[vertexIndex * 3 + 1] = -1.0f + y * tileYSize;
		vertices[vertexIndex * 3 + 2] = 0.0f;

		vertices[(vertexIndex + 1) * 3] = 1.0f;
		vertices[(vertexIndex + 1) * 3 + 1] = -1.0f + y * tileYSize ;
		vertices[(vertexIndex + 1) * 3 + 2] = 0.0f;

		indices[vertexIndex] = vertexIndex;
		indices[vertexIndex + 1] = vertexIndex + 1;
	}

	Shape* grid = new Shape(shader, glDrawType, verticesCount * sizeof(float), vertices, indicesCount * sizeof(int), indices,ShapeType::Line, enableWireframe);
	return grid;
}

void Shape::Draw()
{
	if(m_enableWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	m_shader->UseShader();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	//glDrawArrays(GL_TRIANGLES, 0, triangleCount);
	glDrawElements((int)m_shapeType, m_shapeTypeCount, GL_UNSIGNED_INT,0);

	if(m_enableWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void Shape::ConfigureVBO(int verticesSizeof, float vertices[])
{
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, verticesSizeof, vertices, (int)m_glDrawType);
}

void Shape::ConfigureEBO(int indicesSizeof, unsigned int indices[])
{
	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSizeof, indices, (int)m_glDrawType);
}