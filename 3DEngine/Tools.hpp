#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include "DEFINITIONS.hpp"
#include <cstdint>


namespace game
{
	struct vec3d
	{
		float x, y, z;
	};
	struct vec2d
	{
		float x, y;
	};

	struct triangle
	{
		vec3d p[3];
		sf::Color color;
		vec2d t[3];
		std::string TextureName;
	};
	struct mesh
	{
		std::vector<triangle> tris;
		std::vector<vec3d> vertices;
		std::vector<vec2d> texCoords;

		vec3d ObjPos = { 0, 0, 0 };
		vec3d Rotation = { 0, 0, 0 };
		vec3d Scale = { 0, 0, 0 };
		uint8_t transparency;

		void ReadSTLBinary(std::string filename);
		void ReadOBJ(std::string filename, std::vector<std::string>& textures);
	};

	struct mat4x4
	{
		float m[4][4] = { 0 };
	};
	class Tools
	{
	public:

		class VectorArithmetic
		{
		public:
			vec3d crossProduct(vec3d vect1, vec3d vect2);
			float dotProduct(vec3d vect1, vec3d vect2);
			vec3d normalizeVector(vec3d vect);
			vec3d subtractVector(vec3d vect1, vec3d vect2);
			vec3d VectorAdd(vec3d vect1, vec3d vect2);
			vec3d MultiplyVector(vec3d vect1, float a);
		};
		class MatrixArithmetic
		{
		public:
			MatrixArithmetic(VectorArithmetic& o) : vectorArithmetic(o) {}
			void MultiplyMatrixVector(const vec3d& i, vec3d& o, const mat4x4& m);
			mat4x4 MultiplyMatrixMatrix(const mat4x4& mat1, const mat4x4& mat2);
			mat4x4 Matrix_QuickInverse(mat4x4& m);
			mat4x4 Matrix_PointAt(vec3d& pos, vec3d& target, vec3d& up);
			mat4x4 Matrix_MakeRotationY(float fAngleRad);
		private:
			VectorArithmetic& vectorArithmetic;
		};
		class Rendering3D
		{
		public:
			float normalization(float min, float max, float value);
			Rendering3D(VectorArithmetic& o1, MatrixArithmetic& o2) : vectorArithmetic(o1), matrixArithmetic(o2) {};
			vec3d Vector_IntersectPlane(vec3d& plane_p, vec3d& plane_n, vec3d& lineStart, vec3d& lineEnd);
			int TriangleClipAgainstPlane(vec3d plane_p, vec3d plane_n, triangle& in_tri, triangle& out_tri1, triangle& out_tri2);
			bool CameraClipp(mesh& meshToClipp, vec3d& vCamera, vec3d& vLookDir, vec3d vLight, mat4x4& matView, mat4x4& matProj, mesh& MeshClipped);
			bool UpdateCamera(float& fYaw, float& fPitch, vec3d& vCamera, vec3d& vLookDir, mat4x4& matView);
			bool WallClipp(const mesh& meshToDraw, mesh& meshClipped);
			bool DrawMeshesWithDepthBuffer2(const std::vector<mesh>& meshes, sf::RenderWindow& window, std::map<std::string, sf::Texture>& textures);
			void DrawMeshesWithOpenGL(const std::vector<mesh>& meshes, std::map<std::string, sf::Texture>& textures, sf::RenderWindow& window);
			bool DrawMesh(const mesh& meshToDraw, sf::RenderWindow& window, std::map<std::string, sf::Texture>& textures, int Transparency);
			bool DrawMeshesWithDepthBuffer(const std::vector<mesh>& meshes, sf::RenderWindow& window, std::map<std::string, sf::Texture>& textures);
			void TransformObj(float fThetax, float fThetay, float fThetaz, float x, float y, float z, float scaleX, float scaleY, float scaleZ, mesh& _mesh, mesh& meshToDraw, int Transparency);
			void TranformMeshes(std::vector<mesh>& MeshesTransformed, std::vector<mesh>& MeshesToTransform);
			void RasterizeTriangleToBuffer(const triangle& tri, std::vector<sf::Uint8>& pixelData, std::vector<float>& depthBuffer, const std::map<std::string, sf::Image>& textureImages, int width, int height);
			bool Clip(std::vector<mesh>& MeshesToClipp, std::vector<mesh>& MeshesToRender, vec3d& vCamera, vec3d& vLookDir, vec3d& vLight, mat4x4& matView, mat4x4& matProj);
		private:
			VectorArithmetic& vectorArithmetic;
			MatrixArithmetic& matrixArithmetic;
		};
		class Utility
		{
		public:
			Utility(VectorArithmetic& o) : vectorArithmetic(o) {};
			void AddCube(float x1, float y1, float z1, float x2, float y2, float z2, mesh& mesh);
			float normalization(float min, float max, float value);
			void LookAtCamera(vec3d& objPos, vec3d& vCamera, float& fYaw, float& fPitch);
			int Random(int from, int to);
		private:
			VectorArithmetic vectorArithmetic;
		};
		VectorArithmetic vectorArithmetic;
		MatrixArithmetic matrixArithmetic{ vectorArithmetic };
		Rendering3D rendering3D{ vectorArithmetic, matrixArithmetic };
		Utility utility{ vectorArithmetic };
	};




}