#ifndef _MESH_UTILITIES_H_
#define _MESH_UTILITIES_H_
#include "../Core/Assets/Mesh.h"
class MeshUtilities
{

public:
	struct CubeUVInfo
	{
	public:
		Vector2D LeftFacePos = Vector2D(0.125f, 0.375f);
		Vector2D RightFacePos = Vector2D(0.625f, 0.375f);
		Vector2D BackFacePos = Vector2D(0.875f, 0.375f);
		Vector2D FrontFacePos = Vector2D(0.375f, 0.375f);
		Vector2D BottomFacePos = Vector2D(0.375f, 0.125f);
		Vector2D TopFacePos = Vector2D(0.375f, 0.625f);
		float uvFaceSize = 0.25f;
	};

	static Mesh* CreateCube(const std::string& assetName, float size);
	static Mesh* CreatePlane(const std::string& assetName, float size);
	static Mesh* CreateQuad(const std::string& assetName, float size);
	static Mesh* CreateUVSphere(const std::string& assetName, float radius, int segments, int rings);
	static Mesh* CreateCylinder(const std::string& assetName, float radius, int segments, float height);
	static Mesh* CreateCone(const std::string& assetName, float radius, float radius2, int segments, float height);

	static Mesh* CreateCustomCubeUV(const std::string& assetName, float size, CubeUVInfo cubeUVInfo);
	static Mesh* CreateCustomUVCylinder(const std::string& assetName, float radius, int segments, float height, Vector2D bodyUVPosition,
		Vector2D bodyUVSize, Vector2D circle1UVPosition, Vector2D circle2UVPosition,
		float circleUVRadius);
	static Mesh* CreateCustomConeUV(const std::string& assetName, float radius1, float radius2, int segments, float height, Vector2D bodyUVPosition,
		Vector2D bodyUVSize, Vector2D circle1UVPosition, Vector2D circle2UVPosition, float circle1UVRadius, float circle2UVRadius);

	static Mesh* CreateRectangle(const std::string& assetName, float width, float height);
	static Mesh* CreateRegularConvexPolygon(const std::string& assetName, int sideCount, float radius);
	static Mesh* CreateCircle(const std::string& assetName, float radius);
	static Mesh* CreateGrid(const std::string& assetName, int widthTileCount, int heightTileCount);

	static Mesh* CreateConvexPolygonFanMethod(const std::string& assetName, const std::vector<Vector2D>& polygon);
	static Mesh* CreatePolygonWithEarMethod(const std::string& assetName, const std::vector<Vector2D>& polygon);
};

#endif // !_MESH_UTILITIES_H_
