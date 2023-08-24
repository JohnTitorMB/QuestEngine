#ifndef _MESH_H_
#define _MESH_H_
#include <vector>
#include "../Math/Vector3D.h"
#include <glad/glad.h>
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

class Mesh
{
public:

	GLDrawType m_glDrawType = GLDrawType::GLDYNAMIC_DRAW;
	ShapeType m_shapeType = ShapeType::TRIANGLE;
	Mesh();
	void SetVertices(std::vector<Vector3D> vertices);
	void SetIndices(std::vector<unsigned int> indices);
	std::vector<Vector3D> GetVertices()const;
	std::vector<unsigned int> GetIndices()const;
	void UseMesh();
private:
	std::vector<Vector3D> m_vertices;
	std::vector<unsigned int> m_indices;
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ebo;
	void GenerateVBO();
	void GenerateEBO();
	void GenerateVAO();
	void SetupVertexAttribs(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
	void ConfigureVBO();
	void ConfigureEBO();

};

#endif // !_MESH_H_


