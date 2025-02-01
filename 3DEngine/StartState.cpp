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
        a = "C:/Users/jekabins/Downloads/Terrain.stl";
        Terrain.ReadSTLBinary(a);



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
                {
                    //vCamera.y += fMovementSpeed;
                    CameraYv += 0.5f;
                }
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

        vec3d vLight{ -3, 1, -4 };
        vLight = _data->tools.normalizeVector(vLight);
        vec3d vLookDir;
        mat4x4 matView;
       // mesh meshToClipp, meshToDraw;

        mesh ObjToClipp, ObjToDraw;
        mesh TerrainToClipp, TerrainToDraw;
        mesh TerrainToClipp2, TerrainToDraw2;
        mesh TerrainToClipp3, TerrainToDraw3;
        mesh TerrainToClipp4, TerrainToDraw4;


        float g = 0.01f;
        CameraYv -= g;

        vCamera.y += CameraYv;
        if (vCamera.y <= 0)
        {
            CameraYv = 0;
            vCamera.y = 0;
        }


        _data->tools.UpdateCamera(fYaw, fPitch, vCamera, vLookDir, matView);



        _data->tools.TransformObj(fThetax, fThetay, fThetaz, 0, 20, 250, _mesh, ObjToClipp);
        _data->tools.TransformObj(fThetax + 3.14159, fThetay, fThetaz, 0, -5, 0, Terrain, TerrainToClipp);
        _data->tools.TransformObj(fThetax + 3.14159, fThetay, fThetaz, 997, -5, 0, Terrain, TerrainToClipp2);
        _data->tools.TransformObj(fThetax + 3.14159, fThetay, fThetaz, 0, -5, 997, Terrain, TerrainToClipp3);
        _data->tools.TransformObj(fThetax + 3.14159, fThetay, fThetaz, 997, -5, 997, Terrain, TerrainToClipp4);



        


        _data->tools.CameraClipp(ObjToClipp, vCamera, vLookDir, vLight, matView, matProj, ObjToDraw);
        _data->tools.CameraClipp(TerrainToClipp, vCamera, vLookDir, vLight, matView, matProj, TerrainToDraw);
        _data->tools.CameraClipp(TerrainToClipp2, vCamera, vLookDir, vLight, matView, matProj, TerrainToDraw2);
        _data->tools.CameraClipp(TerrainToClipp3, vCamera, vLookDir, vLight, matView, matProj, TerrainToDraw3);
        _data->tools.CameraClipp(TerrainToClipp4, vCamera, vLookDir, vLight, matView, matProj, TerrainToDraw4);

        std::sort(ObjToDraw.tris.begin(), ObjToDraw.tris.end(), CompareByZDepth);
        std::sort(TerrainToDraw.tris.begin(), TerrainToDraw.tris.end(), CompareByZDepth);
        std::sort(TerrainToDraw2.tris.begin(), TerrainToDraw2.tris.end(), CompareByZDepth);
        std::sort(TerrainToDraw3.tris.begin(), TerrainToDraw3.tris.end(), CompareByZDepth);
        std::sort(TerrainToDraw4.tris.begin(), TerrainToDraw4.tris.end(), CompareByZDepth);

       
        _data->tools.ClipNDraw(TerrainToDraw4, _data->window);
        _data->tools.ClipNDraw(TerrainToDraw2, _data->window);
        _data->tools.ClipNDraw(TerrainToDraw3, _data->window);
         _data->tools.ClipNDraw(TerrainToDraw, _data->window);
        _data->tools.ClipNDraw(ObjToDraw, _data->window);
        
    }



	void StartState::Draw(float dt)
	{
		//_data->window.clear();
		//_data->window.draw(rect);
		_data->window.display();
	}
}