#ifndef _SHAPE_H_
#define _SHAPE_H_
#include <glad/glad.h>
#include "Shader.h"
#include "../Math/Vector2D.h"
#include "Camera.h"
#include "Window.h"
#include "Transform.h"
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
	Shape(Shader* shader, GLDrawType glDrawType, Transform tranform, int verticesCount, Vector2D* vertices, int indicesCount, unsigned int* indices, ShapeType shapeType = ShapeType::TRIANGLE, bool enableWireframe = false);
	~Shape();
	static Shape* CreateTriangle(Shader* shader, GLDrawType glDrawType, Transform tranform,
		float x1, float y1,
		float x2, float y2,
		float x3, float y3, bool enableWireframe = false);

	static Shape* CreateRectangle(Shader* shader, GLDrawType glDrawType, Transform tranform,
		float width, float height, bool enableWireframe = false);

	static Shape* CreateRegularConvexPolygon(Shader* shader, GLDrawType glDrawType, Transform tranform, int sideCount, float radius, bool enableWireframe = false);
	static Shape* CreateCircle(Shader* shader, GLDrawType glDrawType, Transform tranform, float radius, bool enableWireframe = false);
	static Shape* CreateGrid(Shader* shader, GLDrawType glDrawType, Transform tranform, int widthTileCount, int heightTileCount, bool enableWireframe = false);
	void Draw(Camera* camera, Window* window);
	void SetPosition(const Vector2D& position);
	void SetScale(const Vector2D& Scale);
	void SetAngle(const float angle);

	Vector2D GetPosition()const;
	void SetVertices(unsigned int index, const Vector2D& vec);
private :
	GLuint m_vbo;
	GLuint m_ebo;
	GLDrawType m_glDrawType;
	ShapeType m_shapeType;
	Shader* m_shader;
	Vector2D* m_vertices = nullptr;
	Transform m_transform;
	unsigned int* m_indices = nullptr;
	int m_verticesCount;
	int m_indicesCount;
	bool m_enableWireframe = false;
	int m_shapeTypeCount = 1;
	void ConfigureVBO(int verticesSizeof, Vector2D* vertices);
	void ConfigureEBO(int indicesSizeof, unsigned int indices[]);

	
};

#endif // !_SHAPE_H_


