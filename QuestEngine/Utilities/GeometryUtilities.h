#ifndef _GEOMTRY_UTILITIES_H_
#define _GEOMTRY_UTILITIES_H_
#include "../Math/Vector2D.h"
#include <vector>
class GeometryUtilities
{

public:
	static std::vector<unsigned int> TriangulateConvexPolygonFanMethod(const std::vector<Vector2D>& polygon);
	static std::vector<unsigned int> TriangulatePolygonEarMethod(const std::vector<Vector2D>& polygon);
	static bool PointIsInTriangle2D(const Vector2D& point, const Vector2D& v0, const Vector2D& v1, const Vector2D& v2);
	static Vector3D ComputeBarycentricCoordinate(const Vector2D& point, const Vector2D& v0, const Vector2D& v1, const Vector2D& v2);
	static float PinadaEdgeFunction(const Vector2D& point, const Vector2D& v0, const Vector2D& v1);
	static Vector3D GetAverageNormalWithPoints(const std::vector<Vector3D>& points);
	static Vector3D GetCenterWithPoints(const std::vector<Vector3D>& points);
	static std::vector<Vector2D> Progect3DPointsToAveragePlan(const std::vector<Vector3D>& points);
	static Vector3D ProjectPointOnPlane(const Vector3D& point, const Vector3D& planeOrign, const Vector3D& planeNormal);
	GeometryUtilities() = delete;
};

#endif // !_GEOMTRY_UTILITIES_H_

