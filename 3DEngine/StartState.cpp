#include <sstream>
#include "StartState.hpp"

#include <iostream>

namespace game
{
	StartState::StartState(GameDataRef data) : _data(data)
	{


	}
	void StartState::Init()
	{
		//_data->tools.AddCube(-0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, _mesh);
		std::string a = "C:/Users/jekabs.vidrusks/Downloads/test2.stl";
		_mesh.ReadSTLBinary(a);


		/*for (auto x : _mesh.tris)
		{
			std::cout << "1: " << x.p[0].x << ' ' << x.p[0].y << ' ' << x.p[0].z << " | " << x.p[1].x << ' ' << x.p[1].y << ' ' << x.p[1].z << " | " << x.p[2].x << ' ' << x.p[2].y << ' ' << x.p[2].z;
			std::cout << '\n';
		}*/

		float fNear = 0.1f;
		float fFar = 1000.0f;
		float fFov = 90.0f;
		float fAspectRatio = (float)SCREEN_HEIGHT / (float)SCREEN_WIDTH;
		float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

		matProj.m[0][0] = fAspectRatio * fFovRad;
		matProj.m[1][1] = fFovRad;
		matProj.m[2][2] = fFar / (fFar - fNear);
		matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
		matProj.m[2][3] = 1.0f;
		matProj.m[3][3] = 0.0f;
	}
	void StartState::Pause()
	{

	}
	void StartState::Resume()
	{

	}
	void StartState::HandleInput()
	{
		sf::Event event;

		while (_data->window.pollEvent(event))
		{
			if (sf::Event::Closed == event.type)
			{
				_data->window.close();
			}
		}
	}
	void StartState::Update(float dt)
	{
		_data->window.clear();
		mat4x4 matRotZ, matRotX, matRotY, matMov, transformation;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				transformation.m[i][j] = (i == j) ? 1 : 0;
			}
		}
		fThetax = 1.0f * clock.getElapsedTime().asSeconds();
		fThetaz = 1.0f * clock.getElapsedTime().asSeconds();
		fThetay = 1.0f * clock.getElapsedTime().asSeconds();


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

		matMov.m[3][0] = 0;//Translate X
		matMov.m[3][1] = 0;//Translate Y
		matMov.m[3][2] = 250;//Translate Z
		matMov.m[0][3] = 0;
		matMov.m[1][3] = 0;
		matMov.m[2][3] = 0;

		vec3d vCamera{ 0,0,1 };

		transformation = _data->tools.MultiplyMatrixMatrix(transformation, matRotX);
		transformation = _data->tools.MultiplyMatrixMatrix(transformation, matRotZ);
		transformation = _data->tools.MultiplyMatrixMatrix(transformation, matRotY);

		for (auto tri : _mesh.tris)
		{
			triangle triProjected, triTranslated;

			_data->tools.MultiplyMatrixVector(tri.p[0], triTranslated.p[0], transformation);
			_data->tools.MultiplyMatrixVector(tri.p[1], triTranslated.p[1], transformation);
			_data->tools.MultiplyMatrixVector(tri.p[2], triTranslated.p[2], transformation);


			_data->tools.MultiplyMatrixVector(triTranslated.p[0], triTranslated.p[0], matMov);
			_data->tools.MultiplyMatrixVector(triTranslated.p[1], triTranslated.p[1], matMov);
			_data->tools.MultiplyMatrixVector(triTranslated.p[2], triTranslated.p[2], matMov);

			vec3d normal, vect1, vect2;
			vect1 = _data->tools.subtractVector(triTranslated.p[1], triTranslated.p[0]);
			vect2 = _data->tools.subtractVector(triTranslated.p[2], triTranslated.p[0]);

			normal = _data->tools.crossProduct(vect1, vect2);
			normal = _data->tools.normalizeVector(normal);

			//std::cout << normal.x << '\n';
			float dotproduct = _data->tools.dotProduct(normal, vCamera);
			vec3d cameratopoint = _data->tools.subtractVector(triTranslated.p[0], vCamera);
			if (_data->tools.dotProduct(normal, cameratopoint) > 0.0f)
				continue;


			_data->tools.MultiplyMatrixVector(triTranslated.p[0], triProjected.p[0], matProj);
			_data->tools.MultiplyMatrixVector(triTranslated.p[1], triProjected.p[1], matProj);
			_data->tools.MultiplyMatrixVector(triTranslated.p[2], triProjected.p[2], matProj);

			triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
			triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
			triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;
			triProjected.p[0].x *= 0.5f * (float)SCREEN_WIDTH;
			triProjected.p[0].y *= 0.5f * (float)SCREEN_HEIGHT;
			triProjected.p[1].x *= 0.5f * (float)SCREEN_WIDTH;
			triProjected.p[1].y *= 0.5f * (float)SCREEN_HEIGHT;
			triProjected.p[2].x *= 0.5f * (float)SCREEN_WIDTH;
			triProjected.p[2].y *= 0.5f * (float)SCREEN_HEIGHT;

			sf::VertexArray trigle(sf::Triangles, 3);
			//std::cout << dotproduct << '\n';
			trigle[0].color = sf::Color(fabs(dotproduct) * 255.0f, fabs(dotproduct) * 255.0f, fabs(dotproduct) * 255.0f);
			trigle[1].color = sf::Color(fabs(dotproduct) * 255.0f, fabs(dotproduct) * 255.0f, fabs(dotproduct) * 255.0f);
			trigle[2].color = sf::Color(fabs(dotproduct) * 255.0f, fabs(dotproduct) * 255.0f, fabs(dotproduct) * 255.0f);


			trigle[0].position.x = triProjected.p[0].x;
			trigle[0].position.y = triProjected.p[0].y;
			trigle[1].position.x = triProjected.p[1].x;
			trigle[1].position.y = triProjected.p[1].y;
			trigle[2].position.x = triProjected.p[2].x;
			trigle[2].position.y = triProjected.p[2].y;
			/*trigle[3].position.x = triProjected.p[0].x;
			trigle[3].position.y = triProjected.p[0].y;*/

			_data->window.draw(trigle);
		}

	}

	void StartState::Draw(float dt)
	{
		//_data->window.clear();
		//_data->window.draw(rect);
		_data->window.display();
	}
}