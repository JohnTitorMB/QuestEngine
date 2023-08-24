#ifndef _MESH_UTILITIES_H_
#define _MESH_UTILITIES_H_
#include "../Core/Mesh.h"
class MeshUtilities
{
public:
	static Mesh* CreateCube(float size);
	static Mesh* CreateRectangle(float width, float height);
	static Mesh* CreateRegularConvexPolygon(int sideCount, float radius);
	static Mesh* CreateCircle(float radius);
	static Mesh* CreateGrid(int widthTileCount, int heightTileCount);
};

#endif // !_MESH_UTILITIES_H_
