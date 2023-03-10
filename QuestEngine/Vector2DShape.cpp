#include "Vector2DShape.h"

Vector2DShape::Vector2DShape(Vector2D vec, Vector2D pos, float r, float g, float b, float a)
{
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"uniform vec2 shapePosition;\n"
		"uniform vec2 shapeScale;\n"
		"uniform float shapeAngle;\n"
		"uniform vec2 cameraPosition;\n"
		"uniform float cameraVerticalSize;\n"
		"uniform float cameraHorizontalSize;\n"
		"void main()\n"
		"{\n"
		/*Transform vertice from localSpace into worldSpace */
		"vec2 vertexInWorldSpace = vec2(shapePosition.x + aPos.x, shapePosition.y + aPos.y);\n"
		/*Transform vertice from worldSpace into cameraSpace (viewSpace or eyeSpace) */
		"vec2 vertexInCameraSpace = vertexInWorldSpace - cameraPosition;\n"
		/*Transform vertice from cameraSpace into NDCSpace*/
		"vec2 vertexInNDCSpace;\n"
		"vertexInNDCSpace.x = vertexInCameraSpace.x*2.0/cameraHorizontalSize;\n"
		"vertexInNDCSpace.y = vertexInCameraSpace.y*2.0/cameraVerticalSize;\n"
		"gl_Position = vec4(vertexInNDCSpace.x, vertexInNDCSpace.y, aPos.z, 1.0);\n"
		"}\0";


	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FdfragColor;\n"
		"uniform vec4 color; \n"
		"void main()\n"
		"{\n"
		"	FdfragColor = vec4(color.r, color.g, color.b, color.a)\n;"
		"}\0";

	Vector2D* vertices = new Vector2D[]{ Vector2D(0,0), vec };
	unsigned int* indices = new unsigned int[] {0, 1};
	m_shader = new Shader(vertexShaderSource, fragmentShaderSource);
	m_shader->SetUniformVector4D("color", r, g, b, a);
	Transform transform = Transform(pos, Vector2D(1.0f, 1.0f), 0.0f);
	m_shape = new Shape(m_shader, GLDrawType::GLDYNAMIC_DRAW, transform,2, vertices,2, indices,ShapeType::Line);
	SetValue(vec);
}

Vector2DShape::~Vector2DShape()
{
	delete m_shape;
	m_shape = nullptr;

	delete m_shader;
	m_shader = nullptr;
}
void Vector2DShape::SetValue(Vector2D vec)
{
	value = vec;
	m_shape->SetVertices(1, value);
}

void Vector2DShape::SetPosition(Vector2D position)
{
	m_shape->SetPosition(position);
}

void Vector2DShape::Draw(Camera* camera, Window* window)
{
	m_shape->Draw(camera, window);
}