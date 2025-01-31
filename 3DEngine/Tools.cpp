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
		std::cout << triangleCount;
		tris.reserve(triangleCount);
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
	float Tools::normalization(float min, float max, float value)
	{
		return (value - min) / (max - min);
	}
	float Tools::dotProduct(vec3d vect1, vec3d vect2)
	{
		return vect1.x * vect2.x + vect1.y * vect2.y + vect1.z * vect2.z;
	}
	vec3d Tools::normalizeVector(vec3d vect)
	{
		float t = sqrtf(vect.x * vect.x + vect.y * vect.y + vect.z * vect.z);
		if (t)
		{
			vect.x /= t;
			vect.y /= t;
			vect.z /= t;
		}		
		return vect;
	}
	vec3d Tools::crossProduct(vec3d vect1, vec3d vect2)
	{
		vec3d vect3;
		vect3.x = vect1.y * vect2.z - vect1.z * vect2.y;
		vect3.y = vect1.z * vect2.x - vect1.x * vect2.z;
		vect3.z = vect1.x * vect2.y - vect1.y * vect2.x;
		return vect3;
	}
	vec3d Tools::subtractVector(vec3d vect1, vec3d vect2)
	{
		vec3d vect3;
		vect3.x = vect1.x - vect2.x;
		vect3.y = vect1.y - vect2.y;
		vect3.z = vect1.z - vect2.z;
		return vect3;
	}
	void Tools::TransformObj(float fThetax, float fThetay, float fThetaz, float x, float y, float z, mesh _mesh, mesh &meshToDraw)
	{
		meshToDraw.tris.clear();
		mat4x4 matRotZ, matRotX, matRotY, matMov, transformation;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				transformation.m[i][j] = (i == j) ? 1 : 0;
			}
		}
		matRotZ.m[0][0] = cosf(fThetaz);
		matRotZ.m[0][1] = sinf(fThetaz);
		matRotZ.m[1][0] = -sinf(fThetaz);
		matRotZ.m[1][1] = cosf(fThetaz);
		matRotZ.m[2][2] = 1;
		matRotZ.m[3][3] = 1;

		matRotY.m[0][0] = cosf(fThetay);
		matRotY.m[1][1] = cosf(fThetay);
		matRotY.m[2][2] = 1;
		matRotY.m[1][0] = -sinf(fThetay);
		matRotY.m[0][1] = sinf(fThetay);

		matRotX.m[0][0] = 1;
		matRotX.m[1][1] = cosf(fThetax * 0.5f);
		matRotX.m[1][2] = sinf(fThetax * 0.5f);
		matRotX.m[2][1] = -sinf(fThetax * 0.5f);
		matRotX.m[2][2] = cosf(fThetax * 0.5f);
		matRotX.m[3][3] = 1;

		matMov.m[0][0] = 1;
		matMov.m[1][1] = 1;
		matMov.m[2][2] = 1;
		matMov.m[3][3] = 1;

		matMov.m[3][0] = x;//Translate X
		matMov.m[3][1] = y;//Translate Y
		matMov.m[3][2] = z;//Translate Z
		matMov.m[0][3] = 0;
		matMov.m[1][3] = 0;
		matMov.m[2][3] = 0;
		triangle triProjected, triTranslated;

		transformation = MultiplyMatrixMatrix(transformation, matRotX);
		transformation = MultiplyMatrixMatrix(transformation, matRotZ);
		transformation = MultiplyMatrixMatrix(transformation, matRotY);

		for (int i = 0; i < _mesh.tris.size(); i++)
		{
			triangle trigle;
			MultiplyMatrixVector(_mesh.tris[i].p[0], trigle.p[0], transformation);
			MultiplyMatrixVector(_mesh.tris[i].p[1], trigle.p[1], transformation);
			MultiplyMatrixVector(_mesh.tris[i].p[2], trigle.p[2], transformation);


			MultiplyMatrixVector(trigle.p[0], trigle.p[0], matMov);
			MultiplyMatrixVector(trigle.p[1], trigle.p[1], matMov);
			MultiplyMatrixVector(trigle.p[2], trigle.p[2], matMov);

			meshToDraw.tris.push_back(trigle);
		}

		//return _mesh;
	}
	mat4x4 Tools::Matrix_QuickInverse(mat4x4& m) // Only for Rotation/Translation Matrices
	{
		mat4x4 matrix;
		matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
		matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
		matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
		matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
		matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
		matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
		matrix.m[3][3] = 1.0f;
		return matrix;
	}
	vec3d Tools::MultiplyVector(vec3d vect1, float a)
	{
		vec3d rez;
		rez.x = vect1.x * a;
		rez.y = vect1.y * a;
		rez.z = vect1.z * a;
		return rez;
	}
	mat4x4 Tools::Matrix_PointAt(vec3d& pos, vec3d& target, vec3d& up)
	{
		// Calculate new forward direction
		vec3d newForward = subtractVector(target, pos);
		newForward = normalizeVector(newForward);

		// Calculate new Up direction
		vec3d a = MultiplyVector(newForward, dotProduct(up, newForward));
		vec3d newUp = subtractVector(up, a);
		newUp = normalizeVector(newUp);

		// New Right direction is easy, its just cross product
		vec3d newRight = crossProduct(newUp, newForward);

		// Construct Dimensioning and Translation Matrix	
		mat4x4 matrix;
		matrix.m[0][0] = newRight.x;	matrix.m[0][1] = newRight.y;	matrix.m[0][2] = newRight.z;	matrix.m[0][3] = 0.0f;
		matrix.m[1][0] = newUp.x;		matrix.m[1][1] = newUp.y;		matrix.m[1][2] = newUp.z;		matrix.m[1][3] = 0.0f;
		matrix.m[2][0] = newForward.x;	matrix.m[2][1] = newForward.y;	matrix.m[2][2] = newForward.z;	matrix.m[2][3] = 0.0f;
		matrix.m[3][0] = pos.x;			matrix.m[3][1] = pos.y;			matrix.m[3][2] = pos.z;			matrix.m[3][3] = 1.0f;
		return matrix;

	}
	vec3d Tools::VectorAdd(vec3d vect1, vec3d vect2)
	{
		vec3d rez;
		rez.x = vect1.x + vect2.x;
		rez.y = vect1.y + vect2.y;
		rez.z = vect1.z + vect2.z;
		return rez;
	}
	mat4x4 Tools::Matrix_MakeRotationY(float fAngleRad)
	{
		mat4x4 matrix;
		matrix.m[0][0] = cosf(fAngleRad);
		matrix.m[0][2] = sinf(fAngleRad);
		matrix.m[2][0] = -sinf(fAngleRad);
		matrix.m[1][1] = 1.0f;
		matrix.m[2][2] = cosf(fAngleRad);
		matrix.m[3][3] = 1.0f;
		return matrix;
	}
}