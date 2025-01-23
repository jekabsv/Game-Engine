#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

namespace game
{
	struct vec3d
	{
		float x, y, z;
	};

	struct triangle
	{
		vec3d p[3];
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
	};

}