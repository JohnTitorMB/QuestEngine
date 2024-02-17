#include "MeshUtilities.h"
# define M_PI           3.14159265358979323846  /* pi */
#include <iostream>
#include "../Core/AssetsManager.h"
#include "../Core/Assets/Mesh.h"

Mesh* MeshUtilities::CreateCube(const std::string& assetName,float size)
{
	Mesh* mesh = AssetsManager::CreateMesh(assetName, true);
	float halfSize = size / 2.0f;
	std::vector<Vector3D> vertices;

	vertices.push_back(Vector3D(-halfSize, -halfSize, -halfSize));
	vertices.push_back(Vector3D(halfSize, -halfSize, -halfSize));
	vertices.push_back(Vector3D(halfSize, halfSize, -halfSize));
	vertices.push_back(Vector3D(-halfSize, halfSize, -halfSize));
	
	vertices.push_back(Vector3D(halfSize, -halfSize, -halfSize));
	vertices.push_back(Vector3D(halfSize, -halfSize, halfSize));
	vertices.push_back(Vector3D(halfSize, halfSize, halfSize));
	vertices.push_back(Vector3D(halfSize, halfSize, -halfSize));

	vertices.push_back(Vector3D(halfSize, -halfSize, halfSize));
	vertices.push_back(Vector3D(-halfSize, -halfSize, halfSize));
	vertices.push_back(Vector3D(-halfSize, halfSize, halfSize));
	vertices.push_back(Vector3D(halfSize, halfSize, halfSize));

	vertices.push_back(Vector3D(-halfSize, -halfSize, halfSize));
	vertices.push_back(Vector3D(-halfSize, -halfSize, -halfSize));
	vertices.push_back(Vector3D(-halfSize, halfSize, -halfSize));
	vertices.push_back(Vector3D(-halfSize, halfSize, halfSize));

	vertices.push_back(Vector3D(-halfSize, halfSize, -halfSize));
	vertices.push_back(Vector3D(halfSize, halfSize, -halfSize));
	vertices.push_back(Vector3D(halfSize, halfSize, halfSize));
	vertices.push_back(Vector3D(-halfSize, halfSize, halfSize));

	vertices.push_back(Vector3D(-halfSize, -halfSize, -halfSize));
	vertices.push_back(Vector3D(-halfSize, -halfSize, halfSize));
	vertices.push_back(Vector3D(halfSize, -halfSize, halfSize));
	vertices.push_back(Vector3D(halfSize, -halfSize, -halfSize));
	
	std::vector<Vector2D> uvs;

	for (int i = 0; i < 6; i++)
	{
		uvs.push_back(Vector2D(0, 0));
		uvs.push_back(Vector2D(1, 0));
		uvs.push_back(Vector2D(1, 1));
		uvs.push_back(Vector2D(0, 1));
	}

	std::vector<unsigned int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);
	
	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(4);
	indices.push_back(6);
	indices.push_back(7);

	indices.push_back(8);
	indices.push_back(9);
	indices.push_back(10);
	indices.push_back(8);
	indices.push_back(10);
	indices.push_back(11);

	indices.push_back(12);
	indices.push_back(13);
	indices.push_back(14);
	indices.push_back(12);
	indices.push_back(14);
	indices.push_back(15);

	indices.push_back(16);
	indices.push_back(17);
	indices.push_back(18);
	indices.push_back(16);
	indices.push_back(18);
	indices.push_back(19);

	indices.push_back(20);
	indices.push_back(21);
	indices.push_back(22);
	indices.push_back(20);
	indices.push_back(22);
	indices.push_back(23);
	mesh->SetVertices(vertices);
	mesh->SetIndices(indices);
	mesh->SetUvs(uvs);

	mesh->ComputeNormals();
	return mesh;
}

Mesh* MeshUtilities::CreateUVSphere(const std::string& assetName, float radius, int segments, int rings)
{
	Mesh* mesh = AssetsManager::CreateMesh(assetName, true);

	std::vector<Vector3D> vertices;
	std::vector<Vector2D> uvs;
	std::vector<unsigned int> indices;

	for (int y = 0; y <= rings; y++)
	{
		for (int x = 0; x <= segments; x++)
		{
			float xSegment = (float)x / (float)segments;
			float yRing = (float)y / (float)rings;

			float xPos = cosf(xSegment * 2.0f * M_PI) * sinf(yRing * M_PI);
			float yPos = cosf(yRing * M_PI);
			float zPos = sinf(xSegment * 2.0f * M_PI) * sinf(yRing * M_PI);

			float u = xSegment;
			float v = yRing;

			uvs.push_back({ Vector2D(u,v) });
			vertices.push_back({ Vector3D(xPos, yPos, zPos) * radius });
		}
	}

	for (int y = 0; y < rings; y++)
	{
		for (int x = 0; x < segments; x++)
		{
			int current = y * (segments + 1) + x;
			int next = y * (segments + 1) + x + 1;

			indices.push_back(current);
			indices.push_back(next);
			indices.push_back(current + segments + 1);

			indices.push_back(current + segments + 1);
			indices.push_back(next);
			indices.push_back(next + segments + 1);
		}
	}

	mesh->SetVertices(vertices);
	mesh->SetUvs(uvs);
	mesh->SetIndices(indices);

	mesh->ComputeNormals();
	return mesh;
}

Mesh* MeshUtilities::CreatePlane(const std::string& assetName, float size)
{
	Mesh* mesh = AssetsManager::CreateMesh(assetName, true);
	float halSize = size / 2.0f;

	std::vector<Vector3D> vertices =
	{
		{ -halSize, 0, -halSize },
		{ halSize, 0, -halSize },
		{ halSize, 0, halSize },
		{ -halSize, 0, halSize }
	};

	std::vector<Vector2D> uvs =
	{
		{ 0, 0 },
		{ 1, 0 },
		{ 1, 1 },
		{ 0, 1 }
	};

	std::vector<unsigned int> indices =
	{
		0, 2, 1,
		0, 3, 2
	};

	mesh->SetVertices(vertices);
	mesh->SetUvs(uvs);
	mesh->SetIndices(indices);

	std::cout << "Genere plane Normal : " << std::endl;
	mesh->ComputeNormals();
	return mesh;
}

Mesh* MeshUtilities::CreateCylinder(const std::string& assetName, float radius, int segments, float height)
{
	Mesh* mesh = AssetsManager::CreateMesh(assetName, true);

	std::vector<Vector3D> vertices;
	std::vector<Vector2D> uvs;
	std::vector<unsigned int> indices;

	Vector2D circleUVPosition = Vector2D(0.5f, 0.5f);
	float circleUVRadius = 0.5f;

	// Generate Vertices for the top and bottom caps
	for (int i = 0; i < 2; i++)
	{
		float yPos = i == 0 ? -height / 2.0f : height / 2.0f;
		// Center vertex for the cap
		vertices.push_back(Vector3D(0.0f, yPos, 0.0f));
		uvs.push_back(circleUVPosition);

		// Vertices around the cap
		for (int j = 0; j <= segments; j++)
		{
			float angle = (float)j / (float)segments * 2.0f * M_PI;
			float xPos = cos(angle) * radius;
			float zPos = sin(angle) * radius;

			vertices.push_back(Vector3D(xPos, yPos, zPos));

			// Adjusted UVs to distribute radially
			float u = circleUVPosition.m_x + circleUVRadius * cos(angle);
			float v = circleUVPosition.m_y + circleUVRadius * sin(angle);

			uvs.push_back(Vector2D(u, v));
		}
	}

	float circleUVHeight = 1.0f;

	// Generate Vertices for the middle of the cylinder
	for (int i = 0; i <= segments; i++)
	{
		float angle = (float)i / (float)segments * 2.0f * M_PI;
		float xPos = cos(angle) * radius;
		float zPos = sin(angle) * radius;

		// Bottom vertex
		vertices.push_back(Vector3D(xPos, -height / 2.0f, zPos));
		// Adjusted UVs to distribute radially
		float u = (float)i / (float)segments;
		float v = 0.0f;

		uvs.push_back(Vector2D(u, v));

		// Top vertex
		vertices.push_back(Vector3D(xPos, height / 2.0f, zPos));
		// Adjusted UVs to distribute radially
		v = circleUVHeight;
		uvs.push_back(Vector2D(u, v));

	}

	// Generate indices for the top and bottom caps
	for (int i = 0; i < 2; i++)
	{
		int offset = i * (segments + 2);
		for (int j = 0; j < segments; j++)
		{
			indices.push_back(offset);
			indices.push_back(j + 1 + offset);
			indices.push_back(j + 2 + offset);
		}
	}

	// Generate indices for the middle of the cylinder
	int bodyStartIndex = 2 * (segments + 2);
	for (int i = 0; i < segments; i++)
	{
		int current = bodyStartIndex + i * 2;
		int next = current + 2;

		// First triangle
		indices.push_back(current);
		indices.push_back(current + 1);
		indices.push_back(next);

		// Second triangle
		indices.push_back(current + 1);
		indices.push_back(next + 1);
		indices.push_back(next);
	}

	mesh->SetVertices(vertices);
	mesh->SetUvs(uvs);
	mesh->SetIndices(indices);

	std::cout << "Genere cylindre Normal : " << std::endl;
	mesh->ComputeNormals();

	return mesh;
}

Mesh* MeshUtilities::CreateCone(const std::string& assetName, float radius1, float radius2, int segments, float height)
{
	Mesh* mesh = AssetsManager::CreateMesh(assetName, true);

	std::vector<Vector3D> vertices;
	std::vector<Vector2D> uvs;
	std::vector<unsigned int> indices;

	Vector2D circleUVPosition = Vector2D(0.5f, 0.5f);
	float circleUVRadius = 0.5f;

	// Generate Vertices for the top and bottom caps
	for (int i = 0; i < 2; i++)
	{
		float radius = i == 0 ? radius1 : radius2;  // Adjust the radius based on the cap
		float yPos = i == 0 ? -height / 2.0f : height / 2.0f;

		// Center vertex for the cap
		vertices.push_back(Vector3D(0.0f, yPos, 0.0f));
		uvs.push_back(circleUVPosition);

		// Vertices around the cap
		for (int j = 0; j <= segments; j++)
		{
			float angle = (float)j / (float)segments * 2.0f * M_PI;
			float xPos = cos(angle) * radius;
			float zPos = sin(angle) * radius;

			vertices.push_back(Vector3D(xPos, yPos, zPos));

			// Adjusted UVs to distribute radially
			float u = circleUVPosition.m_x + circleUVRadius * cos(angle);
			float v = circleUVPosition.m_y + circleUVRadius * sin(angle);
			uvs.push_back(Vector2D(u, v));
		}
	}

	float circleUVHeight = 1.0f;

	// Generate Vertices for the sides of the cone
	for (int i = 0; i <= segments; i++)
	{
		float angle = (float)i / (float)segments * 2.0f * M_PI;
		float xPosBottom = cos(angle) * radius1;  // Use the bottom radius
		float zPosBottom = sin(angle) * radius1;  // Use the bottom radius
		float xPosTop = cos(angle) * radius2;    // Use the top radius
		float zPosTop = sin(angle) * radius2;    // Use the top radius

		// Bottom vertex
		vertices.push_back(Vector3D(xPosBottom, -height / 2.0f, zPosBottom));
		float u = (float)i / (float)segments;
		float v = 0.0f;
		uvs.push_back(Vector2D(u, v));

		// Top vertex
		vertices.push_back(Vector3D(xPosTop, height / 2.0f, zPosTop));
		v = circleUVHeight;
		uvs.push_back(Vector2D(u, v));
	}

	// Generate indices for the top and bottom caps
	for (int i = 0; i < 2; i++)
	{
		int offset = i * (segments + 2);
		for (int j = 0; j < segments; j++)
		{
			indices.push_back(offset);
			indices.push_back(j + 1 + offset);
			indices.push_back(j + 2 + offset);
		}
	}

	// Generate indices for the sides of the cone
	int bodyStartIndex = 2 * (segments + 2);
	for (int i = 0; i < segments; i++)
	{
		int current = bodyStartIndex + i * 2;
		int next = current + 2;

		// First triangle
		indices.push_back(current);
		indices.push_back(current + 1);
		indices.push_back(next);

		// Second triangle
		indices.push_back(current + 1);
		indices.push_back(next + 1);
		indices.push_back(next);
	}

	mesh->SetVertices(vertices);
	mesh->SetUvs(uvs);
	mesh->SetIndices(indices);

	std::cout << "Genere cone Normal : " << std::endl;
	mesh->ComputeNormals();

	return mesh;
}

Mesh* MeshUtilities::CreateCustomCubeUV(const std::string& assetName, float size, CubeUVInfo cubeUVInfo)
{
	Mesh* mesh = CreateCube(assetName, size);
	std::vector<Vector2D> uvs;
	float halfFaceUvSize = cubeUVInfo.uvFaceSize / 2.0f;

	Vector2D facePosition = cubeUVInfo.FrontFacePos;
	uvs.push_back(facePosition + Vector2D(-halfFaceUvSize, -halfFaceUvSize));
	uvs.push_back(facePosition + Vector2D(halfFaceUvSize, -halfFaceUvSize));
	uvs.push_back(facePosition + Vector2D(halfFaceUvSize, halfFaceUvSize));
	uvs.push_back(facePosition + Vector2D(-halfFaceUvSize, halfFaceUvSize));

	facePosition = cubeUVInfo.RightFacePos;
	uvs.push_back(facePosition + Vector2D(-halfFaceUvSize, -halfFaceUvSize));
	uvs.push_back(facePosition + Vector2D(halfFaceUvSize, -halfFaceUvSize));
	uvs.push_back(facePosition + Vector2D(halfFaceUvSize, halfFaceUvSize));
	uvs.push_back(facePosition + Vector2D(-halfFaceUvSize, halfFaceUvSize));

	facePosition = cubeUVInfo.BackFacePos;
	uvs.push_back(facePosition + Vector2D(-halfFaceUvSize, -halfFaceUvSize));
	uvs.push_back(facePosition + Vector2D(halfFaceUvSize, -halfFaceUvSize));
	uvs.push_back(facePosition + Vector2D(halfFaceUvSize, halfFaceUvSize));
	uvs.push_back(facePosition + Vector2D(-halfFaceUvSize, halfFaceUvSize));

	facePosition = cubeUVInfo.LeftFacePos;
	uvs.push_back(facePosition + Vector2D(-halfFaceUvSize, -halfFaceUvSize));
	uvs.push_back(facePosition + Vector2D(halfFaceUvSize, -halfFaceUvSize));
	uvs.push_back(facePosition + Vector2D(halfFaceUvSize, halfFaceUvSize));
	uvs.push_back(facePosition + Vector2D(-halfFaceUvSize, halfFaceUvSize));

	facePosition = cubeUVInfo.TopFacePos;
	uvs.push_back(facePosition + Vector2D(-halfFaceUvSize, -halfFaceUvSize));
	uvs.push_back(facePosition + Vector2D(halfFaceUvSize, -halfFaceUvSize));
	uvs.push_back(facePosition + Vector2D(halfFaceUvSize, halfFaceUvSize));
	uvs.push_back(facePosition + Vector2D(-halfFaceUvSize, halfFaceUvSize));

	facePosition = cubeUVInfo.BottomFacePos;

	uvs.push_back(facePosition + Vector2D(-halfFaceUvSize, halfFaceUvSize));
	uvs.push_back(facePosition + Vector2D(-halfFaceUvSize, -halfFaceUvSize));
	uvs.push_back(facePosition + Vector2D(halfFaceUvSize, -halfFaceUvSize));
	uvs.push_back(facePosition + Vector2D(halfFaceUvSize, halfFaceUvSize));

	mesh->SetUvs(uvs);
	return mesh;
}

Mesh* MeshUtilities::CreateCustomUVCylinder(const std::string& assetName, float radius, int segments, float height, Vector2D bodyUVPosition,
	Vector2D bodyUVSize, Vector2D circle1UVPosition, Vector2D circle2UVPosition, float circleUVRadius)
{
	Mesh* mesh = CreateCylinder(assetName, radius, segments, height);
	std::vector<Vector2D> uvs;


	// Generate Vertices for the top and bottom caps
	for (int i = 0; i < 2; i++)
	{
		Vector2D uvPosition = i == 0 ? circle1UVPosition : circle2UVPosition;
		// Center vertex for the cap
		uvs.push_back(uvPosition);

		// Vertices around the cap
		for (int j = 0; j <= segments; j++)
		{
			float angle = (float)j / (float)segments * 2.0f * M_PI;
			// Adjusted UVs to distribute radially
			float u = uvPosition.m_x + circleUVRadius * cos(angle);
			float v = uvPosition.m_y + circleUVRadius * sin(angle);

			uvs.push_back(Vector2D(u, v));
		}
	}

	// Generate Vertices for the middle of the cylinder
	for (int i = 0; i <= segments; i++)
	{
		float angle = (float)i / (float)segments * 2.0f * M_PI;

		// Adjusted UVs to distribute radially
		float u = bodyUVPosition.m_x - bodyUVSize.m_x / 2.0f + ((float)i / (float)segments) * (bodyUVSize.m_x);
		float v = bodyUVPosition.m_y - bodyUVSize.m_y / 2.0f;

		uvs.push_back(Vector2D(u, v));

		// Adjusted UVs to distribute radially
		v = bodyUVPosition.m_y - bodyUVSize.m_y / 2.0f + bodyUVSize.m_y;
		uvs.push_back(Vector2D(u, v));
	}

	mesh->SetUvs(uvs);

	std::cout << "Genere CustomUVCylinder Normal : " << std::endl;
	mesh->ComputeNormals();

	return mesh;

}

Mesh* MeshUtilities::CreateCustomConeUV(const std::string& assetName, float radius1, float radius2, int segments, float height, Vector2D bodyUVPosition,
	Vector2D bodyUVSize, Vector2D circle1UVPosition, Vector2D circle2UVPosition, float circle1UVRadius, float circle2UVRadius)
{
	Mesh* mesh = CreateCone(assetName, radius1, radius2, segments, height);
	std::vector<Vector2D> uvs;


	// Generate Vertices for the top and bottom caps
	for (int i = 0; i < 2; i++)
	{
		Vector2D uvPosition = i == 0 ? circle1UVPosition : circle2UVPosition;
		float uvRadius = i == 0 ? circle1UVRadius : circle2UVRadius;
		// Center vertex for the cap
		uvs.push_back(uvPosition);

		// Vertices around the cap
		for (int j = 0; j <= segments; j++)
		{
			float angle = (float)j / (float)segments * 2.0f * M_PI;
			// Adjusted UVs to distribute radially
			float u = uvPosition.m_x + uvRadius * cos(angle);
			float v = uvPosition.m_y + uvRadius * sin(angle);

			uvs.push_back(Vector2D(u, v));
		}
	}

	// Generate Vertices for the middle of the cylinder
	for (int i = 0; i <= segments; i++)
	{
		float angle = (float)i / (float)segments * 2.0f * M_PI;

		// Adjusted UVs to distribute radially
		float u = bodyUVPosition.m_x - bodyUVSize.m_x / 2.0f + ((float)i / (float)segments) * (bodyUVSize.m_x);
		float v = bodyUVPosition.m_y - bodyUVSize.m_y / 2.0f;

		uvs.push_back(Vector2D(u, v));

		// Adjusted UVs to distribute radially
		v = bodyUVPosition.m_y - bodyUVSize.m_y / 2.0f + bodyUVSize.m_y;
		uvs.push_back(Vector2D(u, v));
	}

	mesh->SetUvs(uvs);

	std::cout << "Genere CustomConeUV Normal : " << std::endl;
	mesh->ComputeNormals();
	return mesh;
}

Mesh* MeshUtilities::CreateRectangle(const std::string& assetName, float width, float height)
{
	Mesh* mesh = AssetsManager::CreateMesh(assetName, true);
	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;

	std::vector<Vector3D> vertices =
	{
		{ -halfWidth, -halfHeight, 0 },
		{ halfWidth, -halfHeight, 0 },
		{ halfWidth, halfHeight, 0 },
		{ -halfWidth, halfHeight, 0 }
	};

	std::vector<unsigned int> indices =
	{
		0, 2, 1,
		0, 3, 2
	};

	mesh->SetVertices(vertices);
	mesh->SetIndices(indices);

	std::cout << "Genere Rectangle Normal : " << std::endl;
	mesh->ComputeNormals();

	return mesh;
}

Mesh* MeshUtilities::CreateRegularConvexPolygon(const std::string& assetName, int sideCount, float radius)
{
	Mesh* mesh = AssetsManager::CreateMesh(assetName, true);
	std::vector<Vector3D> vertices;
	float angle = 2.0f * M_PI / sideCount;

	for (int i = 0; i < sideCount; i++)
	{
		float x = cos(i * angle) * radius;
		float y = sin(i * angle) * radius;
		vertices.push_back({ x, y, 0 });
	}

	std::vector<unsigned int> indices;
	for (int i = 1; i < sideCount - 1; i++)
	{
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i + 1);
	}

	mesh->SetVertices(vertices);
	mesh->SetIndices(indices);
	std::cout << "Genere RegularConvexPolygon Normal : " << std::endl;
	mesh->ComputeNormals();

	return mesh;
}

Mesh* MeshUtilities::CreateCircle(const std::string& assetName, float radius)
{
	const int CIRCLESIDE = 40; // Or any other value that provides enough detail for a circle
	return MeshUtilities::CreateRegularConvexPolygon(assetName, CIRCLESIDE, radius);
}

Mesh* MeshUtilities::CreateGrid(const std::string& assetName,  int widthTileCount, int heightTileCount)
{
	Mesh* mesh = AssetsManager::CreateMesh(assetName, true);
	std::vector<Vector3D> vertices;
	std::vector<unsigned int> indices;
	float widthStep = 2.0f / widthTileCount;
	float heightStep = 2.0f / heightTileCount;

	for (int y = 0; y <= heightTileCount; y++)
	{
		for (int x = 0; x <= widthTileCount; x++)
		{
			vertices.push_back({ -1.0f + x * widthStep, -1.0f + y * heightStep, 0 });

			if (x < widthTileCount && y < heightTileCount)
			{
				indices.push_back(y * (widthTileCount + 1) + x);
				indices.push_back(y * (widthTileCount + 1) + x + 1);
				indices.push_back((y + 1) * (widthTileCount + 1) + x);

				indices.push_back((y + 1) * (widthTileCount + 1) + x);
				indices.push_back(y * (widthTileCount + 1) + x + 1);
				indices.push_back((y + 1) * (widthTileCount + 1) + x + 1);
			}
		}
	}

	mesh->SetVertices(vertices);
	mesh->SetIndices(indices);
	return mesh;
}