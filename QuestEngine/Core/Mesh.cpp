#include "Mesh.h"
#include <iostream>
Mesh::Mesh()
{
	GenerateVAO();
	GenerateVBO();
	GenerateEBO();
	SetupVertexAttribs(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void Mesh::SetVertices(std::vector<Vector3D> vertices)
{
	m_vertices = vertices;
	ConfigureVBO();
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

std::vector<unsigned int> Mesh::GetIndices()const
{
	return m_indices;
}

void Mesh::GenerateVBO()
{
	glGenBuffers(1, &m_vbo);
}

void Mesh::GenerateEBO()
{
	glGenBuffers(1, &m_ebo);
}

void Mesh::GenerateVAO()
{
	glGenVertexArrays(1, &m_vao);
}

void Mesh::SetupVertexAttribs(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
{
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	glEnableVertexAttribArray(index);
}

void Mesh::ConfigureVBO()
{
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3D) * m_vertices.size(), m_vertices.data(), (int)m_glDrawType);

	SetupVertexAttribs(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void Mesh::ConfigureEBO()
{
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m_indices.size(), m_indices.data(), (int)m_glDrawType);
}

void Mesh::UseMesh()
{
	glBindVertexArray(m_vao);
}