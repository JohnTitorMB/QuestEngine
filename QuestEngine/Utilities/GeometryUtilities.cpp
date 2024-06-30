#include "GeometryUtilities.h"
#include <list>
#include <set>
#include <iostream>

/// <summary>
/// Triangulate a convexe polygon with fan method
/// </summary>
/// <param name="polygon"> polygon where each point is ordered clockwise or counter-clockwise</param>
/// <returns>return indices triangle</returns>
std::vector<unsigned int> GeometryUtilities::TriangulateConvexPolygonFanMethod(const std::vector<Vector2D>& polygon)
{
	std::vector<unsigned int> indices;
	if (polygon.size() < 3)
	{
		//TODO : Make assert error here
		return indices;
	}

	for (size_t i = 2; i < polygon.size(); i++)
	{
		indices.push_back(i);
		indices.push_back(i-1);
		indices.push_back(0);
	}
	return indices;
}

/// <summary>
/// Triangulate a polygon with fan ear method
/// </summary>
/// <param name="polygon"> polygon where each point is ordered in counter-clockwise</param>
/// <returns>return indices triangle</returns>
std::vector<unsigned int> GeometryUtilities::TriangulatePolygonEarMethod(const std::vector<Vector2D>& polygon)
{
	std::vector<unsigned int> indices;
	if (polygon.size() < 3)
	{
		//TODO : Make assert error here
		return indices;
	}
	else if (polygon.size() == 3)
	{
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		return indices;
	}

	std::list<unsigned int> indicesList;
	for (int i = 0; i < polygon.size(); i++)
		indicesList.push_back(i);

	for (auto it = indicesList.begin(); it != indicesList.end(); ++it)
	{
		int currentIndice = *it;
		int prevIndice;
		int nextIndice;

		auto lastIt = std::prev(indicesList.end(), 1);
		if (it == indicesList.begin())
			prevIndice = *lastIt;
		else
			prevIndice = *std::prev(it, 1);

		if (it == lastIt)
			nextIndice = *indicesList.begin();
		else
			nextIndice = *std::next(it, 1);

		Vector2D prevPoint = polygon[prevIndice];
		Vector2D currentPoint = polygon[currentIndice];
		Vector2D nextPoint = polygon[nextIndice];

		Vector2D prevDirection = (prevPoint - currentPoint).Normalized();
		Vector2D nextDirection = (nextPoint - currentPoint).Normalized();

		float angle = Vector2D::GetAngle(prevDirection, nextDirection);

		if (angle <= 180.0f) // Find first condition for Ear Point
		{
			bool containPoint = false;
			for (int i = 0; i < polygon.size(); i++)
			{
				if (i != currentIndice && i != prevIndice && i != nextIndice)
				{
					if (PointIsInTriangle2D(polygon[i], prevPoint, currentPoint, nextPoint))
					{
						containPoint = true;
						break;
					}
				}
			}

			if (!containPoint) // Find second condition for Ear Point
			{
				indices.push_back(prevIndice);
				indices.push_back(currentIndice);
				indices.push_back(nextIndice);
				indicesList.erase(it);
				it = indicesList.begin();
			}
		}
	}

	return indices;
}

bool GeometryUtilities::PointIsInTriangle2D(const Vector2D& point, const Vector2D& v0, const Vector2D& v1, const Vector2D& v2)
{
	Vector3D barCoord = ComputeBarycentricCoordinate(point, v0, v1, v2);

	if ((barCoord.m_x > 0 && barCoord.m_y > 0 && barCoord.m_z > 0) ||
		(barCoord.m_x < 0 && barCoord.m_y < 0 && barCoord.m_z < 0))
		return true;

	return false;
}

Vector3D GeometryUtilities::ComputeBarycentricCoordinate(const Vector2D& point, const Vector2D& v0, const Vector2D& v1, const Vector2D& v2)
{
	float e0 = PinadaEdgeFunction(point, v0, v1);
	float e1 = PinadaEdgeFunction(point, v1, v2);
	float e2 = PinadaEdgeFunction(point, v2, v0);
	
	return Vector3D(e0, e1, e2);
}

float GeometryUtilities::PinadaEdgeFunction(const Vector2D& point, const Vector2D& v0, const Vector2D& v1)
{
	Vector2D diffP = point - v0;
	Vector2D diffV = v1 - v0;
	return diffV.m_x * diffP.m_y - diffV.m_y * diffP.m_x;
}

/// <summary>
///  Get the normal of 3D points linked point by point in order
/// </summary>
/// <param name="points"></param>
/// <returns></returns>
Vector3D GeometryUtilities::GetAverageNormalWithPoints(const std::vector<Vector3D>& points)
{
	Vector3D averageNormal = Vector3D::Zero;

	if (points.size() > 0)
	{
		for (int i = 0; i < points.size(); i++)
		{
			int j = i + 1;
			int k = i - 1;
			Vector3D currentPoint = points[i];
			Vector3D nextPoint = j != points.size() ? points[j] : points[0];
			Vector3D prevPoint = k != -1 ? points[k] : points[points.size()-1];

			Vector3D prevDirection = (prevPoint - currentPoint).Normalized();
			Vector3D nextDirection = (nextPoint - currentPoint).Normalized();

			Vector3D normal = Vector3D::CrossProduct(prevDirection, nextDirection).Normalized();
			averageNormal += normal;
		}

		averageNormal /= points.size();
	}
	return averageNormal;
}


Vector3D GeometryUtilities::GetCenterWithPoints(const std::vector<Vector3D>& points)
{
	Vector3D center = Vector3D::Zero;

	if (points.size() > 0)
	{
		for (Vector3D point : points)
			center += point;

		center /= points.size();
	}

	return center;
}

std::vector<Vector2D> GeometryUtilities::Progect3DPointsToAveragePlan(const std::vector<Vector3D>& points)
{
	std::vector<Vector2D> points2D;

	Vector3D averageNormal = GetAverageNormalWithPoints(points);
	Vector3D averageCenter = GetCenterWithPoints(points);

	Vector3D orthogonal = Vector3D::Orthogonal(averageNormal);
	Vector3D cross = Vector3D::CrossProduct(averageNormal, orthogonal);

	for (Vector3D point : points)
	{
		Vector3D projectedPoint = ProjectPointOnPlane(point, averageCenter, averageNormal);

		float x = Vector3D::DotProduct(projectedPoint, orthogonal);
		float y = Vector3D::DotProduct(projectedPoint, cross);

		points2D.push_back(Vector2D(x, y));
	}

	return points2D;
}

Vector3D GeometryUtilities::ProjectPointOnPlane(const Vector3D& point, const Vector3D& planeOrign, const Vector3D& planeNormal)
{
	Vector3D diffplaneOrignAndPoint = (point - planeOrign);
	Vector3D projectedPoint = point - planeNormal * Vector3D::DotProduct(diffplaneOrignAndPoint, planeNormal);

	return projectedPoint;
}
