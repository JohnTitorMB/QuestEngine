#ifndef _MESH_RENDERER_H_
#define _MESH_RENDERER_H_
#include "Camera.h"
#include "Light.h"
#include "../Window.h"
#include "Transform.h"
#include "../Assets/Shader.h"
#include "../Assets/Mesh.h"
#include "../Assets/Material.h"
#include "../Component.h"
#include <set>

enum class PolygonMode
{
	Fill = GL_FILL,
	Line = GL_LINE,
	Point = GL_POINT,
};

class MeshRendererComponent : public Component
{
private:

	PolygonMode m_polygonMode = PolygonMode::Fill;
	Shader* m_shader = nullptr;
	TransformComponent* m_transformComponent = nullptr;
	Mesh* m_mesh = nullptr;
	bool m_drawPartialMesh = false;
	int m_partialMeshElementCount = 0;
	int m_partialMeshStartIndex = 0;
	Material* m_material = nullptr;
public:
	MeshRendererComponent() = default;

	void Draw(CameraComponent* camera, std::set<LightComponent*>lights, Window* window)const;
	void SetDrawPartialMesh(bool drawPartialMesh);
	void SetPartialMeshElementCount(int partialMeshElementCount);
	void SetPartialMeshStartIndex(int partialMeshStartIndex);
	void SetMesh(Mesh*mesh);
	void SetShader(Shader* shader);
	void SetMaterial(Material* material);
	void SetPolygonMode(PolygonMode polygonMode);
	void SetTransformComponent(TransformComponent* transformComponent);

	bool GetDrawPartialMesh()const;
	int GetPartialMeshElementCount()const;
	int GetPartialMeshStartIndex()const;
	const Mesh* GetMesh()const;
	const Shader* GetShader()const;
	const Material* GetMaterial(Material* material)const;

	PolygonMode GetPolygonMode()const;
	TransformComponent* GetTransformComponent()const;

};

#endif // !_MESH_RENDERER_H_
