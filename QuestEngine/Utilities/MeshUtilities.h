#ifndef _MESH_UTILITIES_H_
#define _MESH_UTILITIES_H_
#include "../Core/Mesh.h"
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

	static Mesh* CreateCube(float size);
	static Mesh* CreatePlane(float size);
	static Mesh* CreateUVSphere(float radius, int segments, int rings);
	static Mesh* CreateCylinder(float radius, int segments, float height);
	static Mesh* CreateCone(float radius, float radius2, int segments, float height);

	static Mesh* CreateCustomCubeUV(float size, CubeUVInfo cubeUVInfo);
	static Mesh* CreateCustomUVCylinder(float radius, int segments, float height, Vector2D bodyUVPosition,
		Vector2D bodyUVSize, Vector2D circle1UVPosition, Vector2D circle2UVPosition,
		float circleUVRadius);
	static Mesh* CreateCustomConeUV(float radius1, float radius2, int segments, float height, Vector2D bodyUVPosition,
		Vector2D bodyUVSize, Vector2D circle1UVPosition, Vector2D circle2UVPosition, float circle1UVRadius, float circle2UVRadius);

	static Mesh* CreateRectangle(float width, float height);
	static Mesh* CreateRegularConvexPolygon(int sideCount, float radius);
	static Mesh* CreateCircle(float radius);
	static Mesh* CreateGrid(int widthTileCount, int heightTileCount);
};

#endif // !_MESH_UTILITIES_H_
