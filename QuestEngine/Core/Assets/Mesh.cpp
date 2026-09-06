#include "Mesh.h"
#include <iostream>
#include "../Graphics.h"

Mesh::Mesh(bool useOneVbo)
{
	GenerateVAO();
	m_useOneVbo = useOneVbo;
	GenerateVBOs(m_useOneVbo == true ? 1 : 4);
	GenerateEBO();
	
	if (m_useOneVbo)
		ConfigureVertexAttributesBuffer();
	else
	{
		ConfigureVerticesBuffer();
		ConfigureUvsBuffer();
		ConfigureNormalsBuffer();
		ConfigureTangentsBuffer();
	}

	Graphics::GetInstance()->SetupVAO(m_sharedVao);
}

Mesh::~Mesh()
{
	m_vertices.clear();
	m_uvs.clear();
	m_indices.clear();
	m_normals.clear();

	Graphics::GetInstance()->DeleteSharedVAO(m_sharedVao);

	for (GLuint vbo : m_vbos)
	{
		glDeleteBuffers(1, &vbo);
	}

	glDeleteBuffers(1, &m_ebo);
}

void Mesh::SetVertices(std::vector<Vector3D> vertices)
{
	m_vertices = vertices;
	if (m_useOneVbo)
		ConfigureVertexAttributesBuffer();
	else
		ConfigureVerticesBuffer();

	Graphics::GetInstance()->SetupVAO(m_sharedVao);
}

void Mesh::SetUvs(std::vector<Vector2D> uvs)
{
	m_uvs = uvs;
	if (m_useOneVbo)
		ConfigureVertexAttributesBuffer();
	else
		ConfigureUvsBuffer();

	Graphics::GetInstance()->SetupVAO(m_sharedVao);
}

void Mesh::SetNormals(std::vector<Vector3D> normals)
{
	m_normals = normals;
	if (m_useOneVbo)
		ConfigureVertexAttributesBuffer();
	else
		ConfigureNormalsBuffer();

	Graphics::GetInstance()->SetupVAO(m_sharedVao);
}

void Mesh::SetTangents(std::vector<Vector4D> tangents)
{
	m_tangents = tangents;
	if(m_useOneVbo)
		ConfigureVertexAttributesBuffer();
	else
		ConfigureTangentsBuffer();

	Graphics::GetInstance()->SetupVAO(m_sharedVao);
}

void Mesh::SetIndices(std::vector<unsigned int> indices)
{
	m_indices = indices;
	ConfigureEBO();
}


std::vector<Vector3D> Mesh::GetVertices()const
{
	return m_vertices;
}

std::vector<Vector2D> Mesh::GetUvs()const
{
	return m_uvs;
}

std::vector<Vector3D> Mesh::GetNormals()const
{
	return m_normals;
}

std::vector<Vector4D> Mesh::GetTangents() const
{
	return m_tangents;
}

std::vector<unsigned int> Mesh::GetIndices()const
{
	return m_indices;
}

GLuint Mesh::GenerateVBO()
{
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	return vbo;
}

void Mesh::GenerateVBOs(int vboCount)
{
	for (int i = 0; i < vboCount; i++)
	{
		m_vbos.push_back(GenerateVBO()); 
	}
}
void Mesh::GenerateEBO()
{
	glGenBuffers(1, &m_ebo);
}

void Mesh::GenerateVAO()
{
	Graphics::GetInstance()->GenSharedVAO(this, m_sharedVao);
}

std::vector<VertexAttribute> Mesh::CombineVertexBuffer()
{
	std::vector<VertexAttribute> vertexAttributes;
	for (int i = 0; i < m_vertices.size(); i++)
	{
		VertexAttribute vertexAttribute = VertexAttribute();
		vertexAttribute.m_position = m_vertices[i];
		if (i < m_uvs.size())
			vertexAttribute.m_uv = m_uvs[i];
		else
			vertexAttribute.m_uv = Vector2D(0, 0);

		if (i < m_normals.size())
			vertexAttribute.m_normal = m_normals[i];
		else
			vertexAttribute.m_normal = Vector3D(0, 0,0);

		if(i < m_tangents.size())
			vertexAttribute.m_tangent = m_tangents[i];
		else
			vertexAttribute.m_tangent = Vector4D(0, 0, 0, 0);

		vertexAttributes.push_back(vertexAttribute);
	}
	return vertexAttributes;
}

void Mesh::ConfigureVerticesBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3D) * m_vertices.size(), m_vertices.data(), (int)m_glDrawType);
}

void Mesh::ConfigureUvsBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2D) * m_uvs.size(), m_uvs.data(), (int)m_glDrawType);
}

void Mesh::ConfigureNormalsBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3D) * m_normals.size(), m_normals.data(), (int)m_glDrawType);
}

void Mesh::ConfigureTangentsBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector4D) * m_tangents.size(), m_tangents.data(), (int)m_glDrawType);
}

void Mesh::ConfigureVertexAttributesBuffer()
{
	std::vector<VertexAttribute> verticesAttributes = CombineVertexBuffer();
	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexAttribute) * verticesAttributes.size(), verticesAttributes.data(), (int)m_glDrawType);
}

void Mesh::ConfigureEBO()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m_indices.size(), m_indices.data(), (int)m_glDrawType);
}

void Mesh::UseMesh()
{
	Graphics::GetInstance()->BindVAO(m_sharedVao);
}

void Mesh::ComputeNormals()
{
	if (m_shapeType == ShapeType::TRIANGLE)
	{
		m_normals = std::vector<Vector3D>(m_vertices.size());
		for (int i = 0; i < m_indices.size(); i += 3)
		{
			int indice0 = i;
			int indice1 = i + 1;
			int indice2 = i + 2;

			if (indice2 < m_indices.size())
			{
				int vertexIndice0 = m_indices[indice0];
				int vertexIndice1 = m_indices[indice1];
				int vertexIndice2 = m_indices[indice2];

				Vector3D vertex0 = m_vertices[vertexIndice0];
				Vector3D vertex1 = m_vertices[vertexIndice1];
				Vector3D vertex2 = m_vertices[vertexIndice2];
				Vector3D v0v1 = (vertex1 - vertex0).Normalized();
				Vector3D v1v2 = (vertex2 - vertex1).Normalized();
				Vector3D normal = Vector3D::CrossProduct(v0v1, v1v2).Normalized() * -1.0f;

				if (normal.IsNaN())
					normal = Vector3D::Up;

				m_normals[vertexIndice0] += normal;
				m_normals[vertexIndice1] += normal;
				m_normals[vertexIndice2] += normal;
			}
		}

		for (size_t i = 0; i < m_normals.size(); ++i)
		{
			m_normals[i].Normalize();
		}

		SetNormals(m_normals);
	}
}

void Mesh::ComputeTangents()
{
	if (m_shapeType != ShapeType::TRIANGLE)
		return;

	if (m_vertices.empty() || m_uvs.empty() || m_normals.empty())
		return;

	// Accumulate tangent and bitangent contributions for each vertex.
	std::vector<Vector3D> tangents(m_vertices.size(), Vector3D(0.0f, 0.0f, 0.0f));
	std::vector<Vector3D> bitangents(m_vertices.size(), Vector3D(0.0f, 0.0f, 0.0f));

	for (int i = 0; i + 2 < m_indices.size(); i += 3)
	{
		int vertexIndice0 = m_indices[i];
		int vertexIndice1 = m_indices[i + 1];
		int vertexIndice2 = m_indices[i + 2];

		const Vector3D& vertex0 = m_vertices[vertexIndice0];
		const Vector3D& vertex1 = m_vertices[vertexIndice1];
		const Vector3D& vertex2 = m_vertices[vertexIndice2];

		const Vector2D& uv0 = m_uvs[vertexIndice0];
		const Vector2D& uv1 = m_uvs[vertexIndice1];
		const Vector2D& uv2 = m_uvs[vertexIndice2];

		Vector3D edge1 = vertex1 - vertex0;
		Vector3D edge2 = vertex2 - vertex0;

		Vector2D deltaUV1 = uv1 - uv0;
		Vector2D deltaUV2 = uv2 - uv0;

		float determinant =
			deltaUV1.m_x * deltaUV2.m_y -
			deltaUV2.m_x * deltaUV1.m_y;

		// Degenerate UV triangle: no valid tangent basis can be computed.
		if (std::abs(determinant) < 0.000001f)
			continue;

		float f = 1.0f / determinant;

		Vector3D tangent =
		{
			f * (deltaUV2.m_y * edge1.m_x - deltaUV1.m_y * edge2.m_x),
			f * (deltaUV2.m_y * edge1.m_y - deltaUV1.m_y * edge2.m_y),
			f * (deltaUV2.m_y * edge1.m_z - deltaUV1.m_y * edge2.m_z)
		};

		Vector3D bitangent =
		{
			f * (-deltaUV2.m_x * edge1.m_x + deltaUV1.m_x * edge2.m_x),
			f * (-deltaUV2.m_x * edge1.m_y + deltaUV1.m_x * edge2.m_y),
			f * (-deltaUV2.m_x * edge1.m_z + deltaUV1.m_x * edge2.m_z)
		};

		tangents[vertexIndice0] += tangent;
		tangents[vertexIndice1] += tangent;
		tangents[vertexIndice2] += tangent;

		bitangents[vertexIndice0] += bitangent;
		bitangents[vertexIndice1] += bitangent;
		bitangents[vertexIndice2] += bitangent;
	}

	m_tangents.resize(m_vertices.size());

	for (int i = 0; i < m_vertices.size(); ++i)
	{
		Vector3D normal = m_normals[i].Normalized();
		Vector3D tangent = tangents[i];

		// Gram-Schmidt: make tangent orthogonal to the normal.
		tangent = tangent -
			normal * Vector3D::DotProduct(normal, tangent);

		tangent = tangent.Normalized();

		float handedness =
			Vector3D::DotProduct(
				Vector3D::CrossProduct(normal, tangent),
				bitangents[i]
			) < 0.0f
			? -1.0f
			: 1.0f;

		m_tangents[i] =
			Vector4D(
				tangent.m_x,
				tangent.m_y,
				tangent.m_z,
				handedness
			);
	}

	SetTangents(m_tangents);
}
