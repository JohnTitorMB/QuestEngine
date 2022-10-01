#ifndef _SHAPE_H_
#define _SHAPE_H_
#include <glad/glad.h>
#include "Shader.h"
#include "Vector2D.h"

enum class GLDrawType
{
	GLSTREAM_DRAW = 0x88E0,
	GLSTATIC_DRAW = 0x88E4,
	GLDYNAMIC_DRAW = 0x88E8
};

enum class ShapeType
{
	TRIANGLE = 0x0004,
	Line = 0x0001
};

class Shape
{
public :
	Shape(Shader* shader, GLDrawType glDrawType, int verticesCount, Vector2D* vertices, int indicesCount, unsigned int* indices, ShapeType shapeType = ShapeType::TRIANGLE, bool enableWireframe = false);
	~Shape();
	static Shape* CreateTriangle(Shader* shader, GLDrawType glDrawType,
		float x1, float y1,
		float x2, float y2,
		float x3, float y3, bool enableWireframe = false);

	static Shape* CreateRectangle(Shader* shader, GLDrawType glDrawType,
		float width, float height, bool enableWireframe = false);

	static Shape* CreateRegularConvexPolygon(Shader* shader, GLDrawType glDrawType, int sideCount, float radius, bool enableWireframe = false);
	static Shape* CreateCircle(Shader* shader, GLDrawType glDrawType, float radius, bool enableWireframe = false);
	static Shape* CreateGrid(Shader* shader, GLDrawType glDrawType, int widthTileCount, int heightTileCount, bool enableWireframe = false);
	void Draw();

private :
	GLuint m_vbo;
	GLuint m_ebo;
	GLDrawType m_glDrawType;
	ShapeType m_shapeType;
	Shader* m_shader;
	Vector2D* m_vertices = nullptr;
	unsigned int* m_indices = nullptr;
	int m_verticesCount;
	int m_indicesCount;
	bool m_enableWireframe = false;
	int m_shapeTypeCount = 1;
	void ConfigureVBO(int verticesSizeof, Vector2D* vertices);
	void ConfigureEBO(int indicesSizeof, unsigned int indices[]);

	
};

#endif // !_SHAPE_H_



