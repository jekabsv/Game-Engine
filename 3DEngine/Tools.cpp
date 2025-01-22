#include "Tools.hpp"
#include <iostream>
namespace game
{
	void Tools::MultiplyMatrixVector(vec3d& i, vec3d& o, mat4x4& m)
	{
		o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
		o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
		o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
		float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

		if (w != 0.0f)
		{
			o.x /= w; o.y /= w; o.z /= w;
		}
	}
	mat4x4 Tools::MultiplyMatrixMatrix(mat4x4 mat, mat4x4 mat2)
	{
		mat4x4 rez;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				rez.m[i][j] = 0.0f;
				for (int k = 0; k < 4; ++k)
				{
					rez.m[i][j] += mat.m[i][k] * mat2.m[k][j];
				}
			}
		}
		return rez;
	}
	void Tools::AddCube(float x1, float y1, float z1, float x2, float y2, float z2, mesh& mesh)
	{

		triangle tri;

		tri.p[0].x = x1; tri.p[0].y = y1; tri.p[0].z = z1;
		tri.p[1].x = x1; tri.p[1].y = y2; tri.p[1].z = z1;
		tri.p[2].x = x2; tri.p[2].y = y2; tri.p[2].z = z1;
		mesh.tris.push_back(tri);

		tri.p[0].x = x1; tri.p[0].y = y1; tri.p[0].z = z1;
		tri.p[1].x = x2; tri.p[1].y = y2; tri.p[1].z = z1;
		tri.p[2].x = x2; tri.p[2].y = y1; tri.p[2].z = z1;
		mesh.tris.push_back(tri);

		tri.p[0].x = x2; tri.p[0].y = y1; tri.p[0].z = z1;
		tri.p[1].x = x2; tri.p[1].y = y2; tri.p[1].z = z1;
		tri.p[2].x = x2; tri.p[2].y = y2; tri.p[2].z = z2;
		mesh.tris.push_back(tri);

		tri.p[0].x = x2; tri.p[0].y = y1; tri.p[0].z = z1;
		tri.p[1].x = x2; tri.p[1].y = y2; tri.p[1].z = z2;
		tri.p[2].x = x2; tri.p[2].y = y1; tri.p[2].z = z2;
		mesh.tris.push_back(tri);

		tri.p[0].x = x2; tri.p[0].y = y1; tri.p[0].z = z2;
		tri.p[1].x = x2; tri.p[1].y = y2; tri.p[1].z = z2;
		tri.p[2].x = x1; tri.p[2].y = y2; tri.p[2].z = z2;
		mesh.tris.push_back(tri);

		tri.p[0].x = x2; tri.p[0].y = y1; tri.p[0].z = z2;
		tri.p[1].x = x1; tri.p[1].y = y2; tri.p[1].z = z2;
		tri.p[2].x = x1; tri.p[2].y = y1; tri.p[2].z = z2;
		mesh.tris.push_back(tri);

		tri.p[0].x = x1; tri.p[0].y = y1; tri.p[0].z = z2;
		tri.p[1].x = x1; tri.p[1].y = y2; tri.p[1].z = z2;
		tri.p[2].x = x1; tri.p[2].y = y2; tri.p[2].z = z1;
		mesh.tris.push_back(tri);

		tri.p[0].x = x1; tri.p[0].y = y1; tri.p[0].z = z2;
		tri.p[1].x = x1; tri.p[1].y = y2; tri.p[1].z = z1;
		tri.p[2].x = x1; tri.p[2].y = y1; tri.p[2].z = z1;
		mesh.tris.push_back(tri);

		tri.p[0].x = x1; tri.p[0].y = y2; tri.p[0].z = z1;
		tri.p[1].x = x1; tri.p[1].y = y2; tri.p[1].z = z2;
		tri.p[2].x = x2; tri.p[2].y = y2; tri.p[2].z = z2;

		mesh.tris.push_back(tri);
		tri.p[0].x = x1; tri.p[0].y = y2; tri.p[0].z = z1;
		tri.p[1].x = x2; tri.p[1].y = y2; tri.p[1].z = z2;
		tri.p[2].x = x2; tri.p[2].y = y2; tri.p[2].z = z1;

		mesh.tris.push_back(tri);

		tri.p[0].x = x2; tri.p[0].y = y1; tri.p[0].z = z2;
		tri.p[1].x = x1; tri.p[1].y = y1; tri.p[1].z = z2;
		tri.p[2].x = x1; tri.p[2].y = y1; tri.p[2].z = z1;

		mesh.tris.push_back(tri);
		tri.p[0].x = x2; tri.p[0].y = y1; tri.p[0].z = z2;
		tri.p[1].x = x1; tri.p[1].y = y1; tri.p[1].z = z1;
		tri.p[2].x = x2; tri.p[2].y = y1; tri.p[2].z = z1;

		mesh.tris.push_back(tri);
	}
	void mesh::ReadSTLBinary(std::string& filename)
	{
		std::ifstream file(filename, std::ios::binary);
		if (!file.is_open())
		{
			std::cout << "Failed to open STL file: " << filename << std::endl;
		}
		//changes
		char header[80] = { 0 };
		file.read(header, 80);
		uint32_t triangleCount = 0;
		file.read(reinterpret_cast<char*>(&triangleCount), sizeof(uint32_t));

		for (uint32_t i = 0; i < triangleCount; ++i)
		{
			triangle tri;
			vec3d normal, v1, v2, v3;

			file.read(reinterpret_cast<char*>(&normal), sizeof(vec3d));
			file.read(reinterpret_cast<char*>(&v1), sizeof(vec3d));
			file.read(reinterpret_cast<char*>(&v2), sizeof(vec3d));
			file.read(reinterpret_cast<char*>(&v3), sizeof(vec3d));

			tri.p[0] = v1;
			tri.p[1] = v2;
			tri.p[2] = v3;

			tris.push_back(tri);
			file.ignore(2);
		}
		file.close();
	}
}