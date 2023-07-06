#include "Vector2DShape.h"
#include "FileSystem.h"
Vector2DShape::Vector2DShape(Vector2D vec, Vector2D pos, float r, float g, float b, float a)
{
	const char* vsFilename = "Assets/DefaultVertexShader.vert";
	std::string vertexShaderSourceString = FileSystem::get_file_contents(vsFilename);
	const char* vertexShaderSource = vertexShaderSourceString.c_str();

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