#include "MeshUtilities.h"
# define M_PI           3.14159265358979323846  /* pi */

Mesh* MeshUtilities::CreateCube(float size)
{
	Mesh* mesh = new Mesh();
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

	return mesh;
}

Mesh* MeshUtilities::CreateRectangle(float width, float height)
{
	Mesh* mesh = new Mesh();
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
	return mesh;
}

Mesh* MeshUtilities::CreateRegularConvexPolygon(int sideCount, float radius)
{
	Mesh* mesh = new Mesh();
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
	return mesh;
}

Mesh* MeshUtilities::CreateCircle(float radius)
{
	const int CIRCLESIDE = 40; // Or any other value that provides enough detail for a circle
	return MeshUtilities::CreateRegularConvexPolygon(CIRCLESIDE, radius);
}

Mesh* MeshUtilities::CreateGrid(int widthTileCount, int heightTileCount)
{
	Mesh* mesh = new Mesh();
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