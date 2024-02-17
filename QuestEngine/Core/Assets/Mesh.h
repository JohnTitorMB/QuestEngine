#ifndef _MESH_H_
#define _MESH_H_
#include <vector>
#include "../../Math/Vector3D.h"
#include "../../Math/Vector2D.h"
#include <glad/glad.h>

#include "Assets.h"

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

struct VertexAttribute
{
public:
	Vector3D m_position;
	Vector2D m_uv; 
	Vector3D m_normal; 
};
class Mesh : public Assets
{
public:

	GLDrawType m_glDrawType = GLDrawType::GLDYNAMIC_DRAW;
	ShapeType m_shapeType = ShapeType::TRIANGLE;
	Mesh(bool useOneVbo = true);

	~Mesh();
	void SetVertices(std::vector<Vector3D> vertices);
	void SetUvs(std::vector<Vector2D> uvs);
	void SetNormals(std::vector<Vector3D> normals);
	void SetIndices(std::vector<unsigned int> indices);
	std::vector<Vector3D> GetVertices()const;
	std::vector<Vector2D> GetUvs()const;
	std::vector<Vector3D> GetNormals()const;
	std::vector<unsigned int> GetIndices()const;
	void UseMesh();
	void ComputeNormals();
private:
	std::vector<Vector3D> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Vector2D> m_uvs;
	std::vector<Vector3D> m_normals;
	GLuint m_vao;
	std::vector<GLuint> m_vbos;
	GLuint m_ebo;
	bool m_useOneVbo = true;
	GLuint GenerateVBO();
	void GenerateVBOs(int vboCount);
	void GenerateEBO();
	void GenerateVAO();
	void SetupVertexAttribs(GLuint index, int vboIndex, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
	void ConfigureVerticesBuffer();
	void ConfigureUvsBuffer();
	void ConfigureNormalsBuffer();
	void ConfigureVertexAttributesBuffer();
	void ConfigureEBO();
	std::vector<VertexAttribute> CombineVertexBuffer();

};

#endif // !_MESH_H_


