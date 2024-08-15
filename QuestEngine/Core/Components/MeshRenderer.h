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
#include "SceneComponent.h"
#include <set>

enum class PolygonMode
{
	Fill = GL_FILL,
	Line = GL_LINE,
	Point = GL_POINT,
};

enum class CullFace
{
	Back = GL_BACK,
	Front = GL_FRONT,
	BackAndFront = GL_FRONT_AND_BACK
};

enum class FrontFace
{
	ClockWise = GL_CW,
	CounterClockWise = GL_CCW
};

enum class DepthTestFunc
{
	Never = GL_NEVER,
	Less = GL_LESS,
	Equal = GL_EQUAL,
	Lequal = GL_LEQUAL,
	Greater = GL_GREATER,
	Notequal = GL_NOTEQUAL,
	Gequal = GL_GEQUAL,
	Always = GL_ALWAYS
};

enum class StencilTestFunc
{
	Never = GL_NEVER,
	Less = GL_LESS,
	Equal = GL_EQUAL,
	Lequal = GL_LEQUAL,
	Greater = GL_GREATER,
	Notequal = GL_NOTEQUAL,
	Gequal = GL_GEQUAL,
	Always = GL_ALWAYS
};

enum class StencilTestAction
{
	Keep = GL_KEEP,
	Zero = GL_ZERO,
	Replace = GL_REPLACE,
	Incr = GL_INCR,
	Incr_Wrap = GL_INCR_WRAP,
	Decr = GL_DECR,
	Decr_Wrap = GL_DECR_WRAP,
	Invert = GL_INVERT
};

class MeshRendererComponent : public SceneComponent
{
private:

	PolygonMode m_polygonMode = PolygonMode::Fill;
	Shader* m_shader = nullptr;
	Mesh* m_mesh = nullptr;
	bool m_drawPartialMesh = false;
	int m_partialMeshElementCount = 0;
	int m_partialMeshStartIndex = 0;
	Material* m_material = nullptr;
	
	//Depth Specification
	bool m_isDepthMaskEnable = true;
	bool m_isDepthTestEnable = true;
	DepthTestFunc m_depthTestFunc = DepthTestFunc::Less;


	//Stencil Specification
	bool m_isStencilTestEnable = false;
	int m_stencilBackMask = 0xFF;
	int m_stencilFrontMask = 0xFF;

	StencilTestFunc m_stencilTestBackFunc = StencilTestFunc::Always;
	int  m_stencilTestBackRef = 1;
	int  m_stencilTestBackMask = 0xFF;

	StencilTestFunc m_stencilTestFrontFunc = StencilTestFunc::Never;
	int  m_stencilTestFrontRef = 1;
	int  m_stencilTestFrontMask = 0xFF;

	StencilTestAction m_stencilFailBackAction = StencilTestAction::Keep;
	StencilTestAction m_depthFailBackAction = StencilTestAction::Keep;
	StencilTestAction m_depthPassBackAction = StencilTestAction::Keep;

	StencilTestAction m_stencilFailFrontAction = StencilTestAction::Keep;
	StencilTestAction m_depthFailFrontAction = StencilTestAction::Keep;
	StencilTestAction m_depthPassFrontAction = StencilTestAction::Keep;

	//Cull Face Specification
	bool m_isCullFaceEnable = true;
	CullFace m_cullFace = CullFace::Back;
	FrontFace m_frontFace = FrontFace::CounterClockWise;
	void SendMaterialToShader()const;
	int m_geometryRenderingPriority = 0;



public:


	bool m_useViewMatrixWithoutTranslation = false;
	MeshRendererComponent() = default;
	MeshRendererComponent& operator=(const MeshRendererComponent& other) = default;


	void Draw(CameraComponent* camera, std::set<LightComponent*>lights, Window* window)const;
	void SetDrawPartialMesh(bool drawPartialMesh);
	void SetPartialMeshElementCount(int partialMeshElementCount);
	void SetPartialMeshStartIndex(int partialMeshStartIndex);
	void SetMesh(Mesh*mesh);
	void SetShader(Shader* shader);
	void SetMaterial(Material* material);
	void SetPolygonMode(PolygonMode polygonMode);
	void SetGeometryRenderingPriority(int value);

	bool GetDrawPartialMesh()const;
	int GetPartialMeshElementCount()const;
	int GetPartialMeshStartIndex()const;
	int GetGeometryRenderingPriority()const;

	Mesh* GetMesh()const;
	Shader* GetShader()const;
	Material* GetMaterial()const;

	PolygonMode GetPolygonMode()const;

	//Depth Specification Setter
	void EnableDepthMask(bool value);
	void EnableDepthTest(bool value);
	void SetDepthTestFunc(DepthTestFunc depthTestFunc);
	
	//Stencil Specification Setter
	void EnableStencilTest(bool value);
	void SetStencilBackMask(int mask);
	void SetStencilFrontMask(int mask);
	void SetStencilTestBackFunc(StencilTestFunc stencilTestFunc); 
	void SetStencilTestBackRef(int stencilTestRef);
	void SetStencilTestBackMask(int stencilTestMask);

	void SetStencilTestFrontFunc(StencilTestFunc stencilTestFunc);
	void SetStencilTestFrontRef(int stencilTestRef);
	void SetStencilTestFrontMask(int stencilTestMask);
	
	void SetStencilFailBackAction(StencilTestAction stencilTestAction);
	void SetDepthFailBackAction(StencilTestAction stencilTestAction);
	void SetDepthPassBackAction(StencilTestAction stencilTestAction);

	void SetStencilFailFrontAction(StencilTestAction stencilTestAction);
	void SetDepthFailFrontAction(StencilTestAction stencilTestAction);
	void SetDepthPassFrontAction(StencilTestAction stencilTestAction);

	//CullFace Specification Setter
	void EnableCullFace(bool value);
	void SetCullFace(CullFace cullFace);
	void SetFrontFace(FrontFace frontFace);


	//Depth Specification Getter
	bool IsDepthMaskEnable();
	bool IsDepthTestEnable();
	DepthTestFunc GetDepthTestFunc();

	//Stencil Specification Getter
	int GetStencilBackMask();
	int GetStencilFrontMask();
	bool IsStencilTestEnable();
	StencilTestFunc GetStencilTestBackFunc();
	int GetStencilTestBackRef();
	int GetStencilTestBackMask();

	StencilTestFunc GetStencilTestFrontFunc();
	int GetStencilTestFrontRef();
	int GetStencilTestFrontMask();

	StencilTestAction GetStencilFailBackAction();
	StencilTestAction GetDepthFailBackAction();
	StencilTestAction GetDepthPassBackAction();

	StencilTestAction GetStencilFailFrontAction();
	StencilTestAction GetDepthFailFrontAction();
	StencilTestAction GetDepthPassFrontAction();


	//CullFace Specification Getter
	bool IsCullFaceEnable(bool value);
	CullFace GetCullFace(CullFace cullFace);
	FrontFace GetFrontFace(FrontFace frontFace);

	Component* Clone()override;
	void AssignPointerAndReference()override;
	
};

#endif // !_MESH_RENDERER_H_
