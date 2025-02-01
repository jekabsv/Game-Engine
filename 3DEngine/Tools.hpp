#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include "DEFINITIONS.hpp"

namespace game
{
	struct vec3d
	{
		float x, y, z;
	};

	struct triangle
	{
		vec3d p[3];
		sf::Color color;
	};
	struct mesh
	{
		std::vector<triangle> tris;
		void ReadSTLBinary(std::string& filename);

	};

	struct mat4x4
	{
		float m[4][4] = { 0 };
	};
	class Tools
	{
	public:
		void MultiplyMatrixVector(vec3d& i, vec3d& o, mat4x4& m);
		mat4x4 MultiplyMatrixMatrix(mat4x4 mat, mat4x4 mat2);
		void AddCube(float x1, float y1, float z1, float x2, float y2, float z2, mesh& mesh);
		float normalization(float min, float max, float value);
		float dotProduct(vec3d vect1, vec3d vect2);
		vec3d normalizeVector(vec3d vect);
		vec3d crossProduct(vec3d vect1, vec3d vect2);
		vec3d subtractVector(vec3d vect1, vec3d vect2);
		void TransformObj(float fthetax, float fthetay, float fthetaz, float x, float y, float z, mesh _mesh, mesh& meshToDraw);
		mat4x4 Matrix_QuickInverse(mat4x4& m);
		vec3d MultiplyVector(vec3d vect1, float a);
		mat4x4 Matrix_PointAt(vec3d& pos, vec3d& target, vec3d& up);
		vec3d VectorAdd(vec3d vect1, vec3d vect2);
		mat4x4 Matrix_MakeRotationY(float fAngleRad);
		vec3d Vector_IntersectPlane(vec3d& plane_p, vec3d& plane_n, vec3d& lineStart, vec3d& lineEnd);
		int TriangleClipAgainstPlane(vec3d plane_p, vec3d plane_n, triangle& in_tri, triangle& out_tri1, triangle& out_tri2);
		bool CameraClipp(mesh& meshToClipp, vec3d& vCamera, vec3d& vLookDir, vec3d vLight, mat4x4& matView, mat4x4& matProj, mesh& MeshClipped);
		bool UpdateCamera(float& fYaw, float& fPitch, vec3d& vCamera, vec3d& vLookDir, mat4x4& matView);
		bool ClipNDraw(mesh meshToDraw, sf::RenderWindow& window);
	};

}