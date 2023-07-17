#include "Shape.h"
#include <iostream>
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>

#define  CIRCLESIDE 30


Shape::Shape(Shader* shader, GLDrawType glDrawType, Transform tranform, int verticesCount, Vector2D* vertices, int indicesCount, unsigned int* indices, ShapeType shapeType, bool enableWireframe)
{
	m_shader = shader;
	m_glDrawType = glDrawType;
	m_shapeType = shapeType;
	m_enableWireframe = enableWireframe;
	m_transform = tranform;
	
	ConfigureVAO();

	GenereVBO();
	ConfigureVBO(sizeof(Vector2D) * verticesCount,vertices);
	SetupVertexAttribs();

	GenereEBO();
	ConfigureEBO(sizeof(int) * indicesCount, indices);

	m_shapeTypeCount = indicesCount;

	m_vertices = vertices;
	m_indices = indices;

	m_verticesCount = verticesCount;
	m_indicesCount = indicesCount;
}

Shape::~Shape()
{
	if (m_vertices)
		delete [] m_vertices;

	if (m_indices)
		delete [] m_indices;
}
Shape* Shape::CreateTriangle(Shader* shader, GLDrawType glDrawType, Transform tranform,
	float x1, float y1,
	float x2, float y2,
	float x3, float y3, bool enableWireframe)
{
	Vector2D* vertices = new Vector2D[] {Vector2D (x1, y1), Vector2D(x2, y2), Vector2D(x3, y3) };
	unsigned int* indices = new unsigned int[]{ 0,1,2 };


	Shape* triangle = new Shape(shader, glDrawType, tranform, 9, vertices, 3, indices, ShapeType::TRIANGLE,enableWireframe);
	return triangle;
}

Shape* Shape::CreateRectangle(Shader* shader, GLDrawType glDrawType, Transform tranform,
	float width, float height, bool enableWireframe)
{
	Vector2D* vertices = new Vector2D[]
	{ Vector2D(- width / 2.0f, -height / 2.0f), //bottom-Left
	  Vector2D(width / 2.0f, -height / 2.0f), //bottom-right
	  Vector2D(width / 2.0f, height / 2.0f), // top-right
	  Vector2D(-width / 2.0f, height / 2.0f) // top-left
	};

	unsigned int* indices = new unsigned int[]
	{
		0,2,1,
		0,3,2
	};


	Shape* rectangle = new Shape(shader, glDrawType, tranform, 4, vertices, 6, indices, ShapeType::TRIANGLE, enableWireframe);
	return rectangle;
}

Shape* Shape::CreateRegularConvexPolygon(Shader* shader, GLDrawType glDrawType, Transform tranform, int sideCount, float radius, bool enableWireframe)
{
	int verticesCount = sideCount;
	int indicesCount = (sideCount - 2) * 3;
	Vector2D* vertices = new Vector2D[verticesCount];
	unsigned int* indices = new unsigned int[indicesCount];

	bool isPair = (sideCount % 2) == 0;
	
	float angle = 0;
	if (isPair)
		angle = M_PI / 2.0f + 2.0f * M_PI / sideCount / 2.0f;
	else
		angle = M_PI / 2.0f;

	//Initialise Vertices
	for (int i = 0; i < sideCount; i++)
	{
		float x = cos(angle) * radius;
		float y = sin(angle) * radius;

		angle -= 2.0f * M_PI / sideCount;

		// Exemple square : 1 = xyz  2 = xyz 3 = xyz 4 = xyz
		vertices[i].m_x = x;
		vertices[i].m_y = y;
	}
	//Initialise Triangle Index
	for (int triangleIndex = 0; triangleIndex < (sideCount - 2); triangleIndex++)
	{
		// Exemple square : triangle 1 = 3 sommet  trianglme 2 = 3 sommet également

		indices[triangleIndex * 3] = 0;
		indices[triangleIndex * 3 + 1] = triangleIndex + 1;
		indices[triangleIndex * 3 + 2] = triangleIndex + 2;		
	}

	Shape* polygon = new Shape(shader, glDrawType, tranform, verticesCount, vertices, indicesCount, indices, ShapeType::TRIANGLE, enableWireframe);
	return polygon;
}

Shape* Shape::CreateCircle(Shader* shader, GLDrawType glDrawType, Transform tranform, float radius, bool enableWireframe)
{
	return CreateRegularConvexPolygon(shader, glDrawType, tranform, CIRCLESIDE, radius, enableWireframe);
}

Shape* Shape::CreateGrid(Shader* shader, GLDrawType glDrawType, Transform tranform, int widthTileCount, int heightTileCount, bool enableWireframe)
{
	int verticesCount = ((widthTileCount + 1) + (heightTileCount + 1)) * 2;
	Vector2D* vertices = new Vector2D[verticesCount];

	int indicesCount = ((widthTileCount + 1) + (heightTileCount + 1)) * 2;
	unsigned int* indices = new unsigned int[indicesCount];

	//Configure vertical vertices and indices
	float tileXSize = 2.0f / widthTileCount;
	for (int x = 0; x < (widthTileCount + 1); x++)
	{
		int vertexIndex = x * 2;
		vertices[vertexIndex].m_x = -1.0f + x * tileXSize;
		vertices[vertexIndex].m_y = -1.0f;

		vertices[(vertexIndex + 1)].m_x = -1.0f + x * tileXSize;
		vertices[(vertexIndex + 1)].m_y = 1.0f;


		indices[vertexIndex] = vertexIndex;
		indices[vertexIndex+1] = vertexIndex+1;
	}

	//Configure horizontal vertices and indices
	float tileYSize = 2.0f / heightTileCount;
	for (int y = 0; y < (heightTileCount + 1); y++)
	{
		int vertexIndex = (widthTileCount + 1.0f) * 2.0f + y * 2.0f;
		vertices[vertexIndex].m_x = -1.0f;
		vertices[vertexIndex].m_y = -1.0f + y * tileYSize;

		vertices[(vertexIndex + 1)].m_x = 1.0f;
		vertices[(vertexIndex + 1)].m_y = -1.0f + y * tileYSize ;

		indices[vertexIndex] = vertexIndex;
		indices[vertexIndex + 1] = vertexIndex + 1;
	}

	Shape* grid = new Shape(shader, glDrawType, tranform, verticesCount, vertices, indicesCount, indices,ShapeType::Line, enableWireframe);
	return grid;
}

void Shape::Draw(Camera* camera, Window* window)
{
	if(m_enableWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	m_shader->UseShader();

	m_shader->SetUniformMatrix3x3("model", m_transform.TransformMatrix());
	m_shader->SetUniformMatrix3x3("view", camera->ViewMatrix());
	m_shader->SetUniformMatrix3x3("toNDCSpace", camera->ToNDCSpaceMatrix(window->GetWidth(), window->GetHeight()));

	glBindVertexArray(m_vao);
	glDrawElements((int)m_shapeType, m_shapeTypeCount, GL_UNSIGNED_INT,0);

	if(m_enableWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void Shape::ConfigureVAO()
{
	glGenVertexArrays(1, &m_vao);
}

void Shape::SetupVertexAttribs()
{
	glBindVertexArray(m_vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Shape::GenereVBO()
{
	glGenBuffers(1, &m_vbo);
}

void Shape::GenereEBO()
{
	glGenBuffers(1, &m_ebo);
}
void Shape::ConfigureVBO(int verticesSizeof, Vector2D* vertices)
{
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, verticesSizeof, vertices, (int)m_glDrawType);
}

void Shape::ConfigureEBO(int indicesSizeof, unsigned int indices[])
{
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSizeof, indices, (int)m_glDrawType);
}

void Shape::SetPosition(const Vector2D& position)
{
	m_transform.m_position = position;
}

void Shape::SetScale(const Vector2D& Scale)
{
	m_transform.m_scale = Scale;
}

void Shape::SetAngle(const float angle)
{
	m_transform.m_angle = angle;
}


Vector2D Shape::GetPosition()const
{
	return m_transform.m_position;
}

void Shape::SetVertices(unsigned int index, const Vector2D& vec)
{
	if (index >= m_verticesCount)
		return;
	
	m_vertices[index] = vec;

	ConfigureVBO(sizeof(Vector2D) * m_verticesCount, m_vertices);
}