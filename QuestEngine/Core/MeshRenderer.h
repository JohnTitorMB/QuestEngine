#ifndef _MESH_RENDERER_H_
#define _MESH_RENDERER_H_
#include "Camera.h"
#include "Light.h"
#include "Window.h"
#include "Transform.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
enum class PolygonMode
{
	Fill = GL_FILL,
	Line = GL_LINE,
	Point = GL_POINT,
};

class MeshRenderer
{
private:

	PolygonMode m_polygonMode = PolygonMode::Fill;
	Shader* m_shader;
	Transform m_transform;
	Mesh* m_mesh;
	Texture* m_texture;
	bool m_drawPartialMesh = false;
	int m_partialMeshElementCount = 0;
	int m_partialMeshStartIndex = 0;
	Vector2D m_textureTilling = Vector2D(1, 1);
	Vector2D m_textureOffset = Vector2D(0, 0);
	Material* m_material;
public:
	MeshRenderer(Mesh* mesh, Transform transform, Shader* shader, Material* material, Texture* texture, Vector2D textureTilling = Vector2D(1,1), Vector2D textureOffset = Vector2D(0, 0));
	void Draw(Camera* camera, std::vector<Light*>lights, Window* window)const;
	void SetDrawPartialMesh(bool drawPartialMesh);
	void SetPartialMeshElementCount(int partialMeshElementCount);
	void SetPartialMeshStartIndex(int partialMeshStartIndex);
	void SetMesh(Mesh*mesh);
	void SetShader(Shader* shader);
	void SetPolygonMode(PolygonMode polygonMode);
	void SetTransform(Transform transform);

	bool GetDrawPartialMesh()const;
	int GetPartialMeshElementCount()const;
	int GetPartialMeshStartIndex()const;
	const Mesh* GetMesh()const;
	const Shader* GetShader()const;
	PolygonMode GetPolygonMode()const;
	Transform GetTransform()const;
};

#endif // !_MESH_RENDERER_H_
