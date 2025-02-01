#include <sstream>
#include "StartState.hpp"

#include <iostream>
#include <list>

namespace game
{


    float CalculateZDepth(const triangle& tri)
    {
        // Calculate the centroid (average of the 3 vertices)
        float z = (tri.p[0].z + tri.p[1].z + tri.p[2].z) / 3.0f;
        return z;
    }
    bool CompareByZDepth(const triangle& a, const triangle& b)
    {
        // Sort in descending order (furthest first)
        return CalculateZDepth(a) > CalculateZDepth(b);
    }



	StartState::StartState(GameDataRef data) : _data(data)
	{


	}
	void StartState::Init()
	{
        vCamera = { 0, 0, -10 };
		//_data->tools.AddCube(-0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, _mesh);
		std::string a = "C:/Users/jekabins/Downloads/test2.stl";
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
            if (event.type == sf::Event::KeyPressed)
            {
                float fRotationSpeed = 2.0f / 60;
                float fMovementSpeed = 5.0f;

                if (event.key.code == sf::Keyboard::Left)
                    fYaw -= fRotationSpeed;
                if (event.key.code == sf::Keyboard::Right)
                    fYaw += fRotationSpeed;
                if (event.key.code == sf::Keyboard::Up)
                    fPitch += fRotationSpeed;
                if (event.key.code == sf::Keyboard::Down)
                    fPitch -= fRotationSpeed;
                if (event.key.code == sf::Keyboard::W)
                    vCamera.z += fMovementSpeed;
                if (event.key.code == sf::Keyboard::S)
                    vCamera.z -= fMovementSpeed;
                if (event.key.code == sf::Keyboard::A)
                    vCamera.x += fMovementSpeed;
                if (event.key.code == sf::Keyboard::D)
                    vCamera.x -= fMovementSpeed;
                if (event.key.code == sf::Keyboard::Space)
                    vCamera.y += fMovementSpeed;
                if (event.key.code == sf::Keyboard::LShift)
                    vCamera.y -= fMovementSpeed;
                
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
       // fThetax = 1.0f * clock.getElapsedTime().asSeconds();

       
        vec3d vLight{ -0.0333087, 0.0333272, 0.998889 };
        vec3d vLookDir;
        mat4x4 matView;
        _data->tools.UpdateCamera(fYaw, fPitch, vCamera, vLookDir, matView);

        //vec3d vUp{ 0, 1, 0 };


        //
        //vLookDir.x = cosf(fYaw) * cosf(fPitch);
        //vLookDir.y = sinf(fPitch);
        //vLookDir.z = sinf(fYaw) * cosf(fPitch);
        //vLookDir = _data->tools.normalizeVector(vLookDir);
        ////std::cout << vLookDir.x << ' ' << vLookDir.y << ' ' << vLookDir.z << '\n';


        //vec3d vTarget = _data->tools.VectorAdd(vCamera, vLookDir);
        //mat4x4 matCamera = _data->tools.Matrix_PointAt(vCamera, vTarget, vUp);

        //mat4x4 matView = _data->tools.Matrix_QuickInverse(matCamera);

        mesh meshToClipp, meshToDraw;
        _data->tools.TransformObj(fThetax, fThetay, fThetaz, 0, 0, 250, _mesh, meshToClipp);

        _data->tools.CameraClipp(meshToClipp, vCamera, vLookDir, vLight, matView, matProj, meshToDraw);

        std::sort(meshToDraw.tris.begin(), meshToDraw.tris.end(), CompareByZDepth);

        for (auto& triToRaster : meshToDraw.tris)
        {
            triangle clipped[2];
            std::list <triangle> listTriangles;
            listTriangles.push_back(triToRaster);
            int nNewTriangles = 1;

            for (int p = 0; p < 4; p++)
            {
                int nTrisToAdd = 0;
                while (nNewTriangles > 0)
                {
                    triangle test = listTriangles.front();
                    listTriangles.pop_front();
                    nNewTriangles--;
                    switch (p)
                    {
                    case 0:	nTrisToAdd = _data->tools.TriangleClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                    case 1:	nTrisToAdd = _data->tools.TriangleClipAgainstPlane({ 0.0f, (float)SCREEN_HEIGHT - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                    case 2:	nTrisToAdd = _data->tools.TriangleClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                    case 3:	nTrisToAdd = _data->tools.TriangleClipAgainstPlane({ (float)SCREEN_WIDTH - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                    }
                    for (int w = 0; w < nTrisToAdd; w++)
                        listTriangles.push_back(clipped[w]);
                }
                nNewTriangles = listTriangles.size();
            }
            for (auto& x : listTriangles)
            {
                sf::VertexArray trigle(sf::Triangles, 3);
                trigle[0].color = x.color;
                trigle[1].color = x.color;
                trigle[2].color = x.color;

                trigle[0].position.x = x.p[0].x;
                trigle[0].position.y = x.p[0].y;
                trigle[1].position.x = x.p[1].x;
                trigle[1].position.y = x.p[1].y;
                trigle[2].position.x = x.p[2].x;
                trigle[2].position.y = x.p[2].y;

                _data->window.draw(trigle);
            }
        }
    }

	void StartState::Draw(float dt)
	{
		//_data->window.clear();
		//_data->window.draw(rect);
		_data->window.display();
	}
}