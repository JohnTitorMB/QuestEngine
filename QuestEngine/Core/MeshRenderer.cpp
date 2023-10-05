#include "MeshRenderer.h"
#include <iostream>
MeshRenderer::MeshRenderer(Mesh* mesh, Transform transform, Shader* shader, Texture* texture)
{
	m_mesh = mesh;
	m_transform = transform;
	m_shader = shader;
	m_texture = texture;
}

void MeshRenderer::Draw(Camera* camera, Window* window)const
{
	glPolygonMode(GL_FRONT_AND_BACK, (int)m_polygonMode);

	m_shader->UseShader();

	m_shader->SetUniformMatrix4x4("model", m_transform.TransformMatrix());
	m_shader->SetUniformMatrix4x4("view", camera->ViewMatrix());
	m_shader->SetUniformMatrix4x4("projection", camera->ProjectionMatrix(window->GetWidth(), window->GetHeight()));
	
	m_mesh->UseMesh();
	m_texture->UseTexture();
	

	if(m_drawPartialMesh)
		glDrawElements((int)m_mesh->m_shapeType, m_partialMeshElementCount, GL_UNSIGNED_INT, (void*)(m_partialMeshStartIndex * sizeof(unsigned int)));
	else
	{
		glDrawElements((int)m_mesh->m_shapeType, m_mesh->GetIndices().size(), GL_UNSIGNED_INT, 0);
	}
}

void MeshRenderer::SetDrawPartialMesh(bool drawPartialMesh)
{
	m_drawPartialMesh = drawPartialMesh;
}
void MeshRenderer::SetPartialMeshElementCount(int partialMeshElementCount)
{
	m_partialMeshElementCount = partialMeshElementCount;
}

void MeshRenderer::SetPartialMeshStartIndex(int partialMeshStartIndex)
{
	m_partialMeshStartIndex = partialMeshStartIndex;
}

void MeshRenderer::SetMesh(Mesh* mesh)
{
	m_mesh = mesh;
}

void MeshRenderer::SetShader(Shader* shader)
{
	m_shader = shader;
}

void MeshRenderer::SetPolygonMode(PolygonMode polygonMode)
{
	m_polygonMode = polygonMode;
}

void MeshRenderer::SetTransform(Transform transform)
{
	m_transform = transform;
}


bool MeshRenderer::GetDrawPartialMesh()const
{
	return m_drawPartialMesh;
}

int MeshRenderer::GetPartialMeshElementCount()const
{
	return m_partialMeshElementCount;
}

int MeshRenderer::GetPartialMeshStartIndex()const
{
	return m_partialMeshStartIndex;
}

const Mesh* MeshRenderer::GetMesh()const
{
	return m_mesh;
}

const Shader* MeshRenderer::GetShader()const
{
	return m_shader;
}

PolygonMode MeshRenderer::GetPolygonMode()const
{
	return m_polygonMode;
}

Transform MeshRenderer::GetTransform()const 
{
	return m_transform;
}

