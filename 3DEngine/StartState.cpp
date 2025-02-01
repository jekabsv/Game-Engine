#include <sstream>
#include "StartState.hpp"

#include <iostream>
#include <list>

namespace game
{


    //float CalculateZDepth(const triangle& tri)
    //{
    //    // Calculate the centroid (average of the 3 vertices)
    //    float z = (tri.p[0].y + tri.p[1].y + tri.p[2].y) / 3.0f;
    //    return z;
    //}
    //bool CompareByZDepth(const triangle& a, const triangle& b)
    //{
    //    // Sort in descending order (furthest first)
    //    return CalculateZDepth(a) > CalculateZDepth(b);
    //}



    //float CalculateDistanceToCamera(const triangle& tri, const vec3d& camera)
    //{
    //    // Calculate the centroid (average of the 3 vertices)
    //    vec3d centroid = {
    //        (tri.p[0].x + tri.p[1].x + tri.p[2].x) / 3.0f,
    //        (tri.p[0].y + tri.p[1].y + tri.p[2].y) / 3.0f,
    //        (tri.p[0].z + tri.p[1].z + tri.p[2].z) / 3.0f
    //    };

    //    // Calculate the distance from the camera to the centroid of the triangle
    //    float dx = centroid.x - camera.x;
    //    float dy = centroid.y - camera.y;
    //    float dz = centroid.z - camera.z;

    //    return sqrtf(dx * dx + dy * dy + dz * dz);
    //}
    //bool CompareByDistanceToCamera(const triangle& a, const triangle& b, const vec3d& camera)
    //{
    //    // Sort by distance from the camera (nearest first)
    //    return CalculateDistanceToCamera(a, camera) < CalculateDistanceToCamera(b, camera);
    //}
    float CalculateAverageZ(const triangle& t)
    {
        return (t.p[0].z + t.p[1].z + t.p[2].z) / 3.0f;
    }

    bool CompareByAverageZ(const triangle& t1, const triangle& t2)
    {
        return CalculateAverageZ(t1) > CalculateAverageZ(t2); // Sort in descending order (further first)
    }





	StartState::StartState(GameDataRef data) : _data(data)
	{


	}
	void StartState::Init()
	{
        vCamera = { 0, 0, -10 };
		//_data->tools.AddCube(-0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, _mesh);
        std::vector <std::string> texturesNames;
        //_data->tools.AddCube(0, 0, 0, 1, 1, 1, _mesh);
        _mesh.ReadOBJ(OBJ_FILE_PATH, texturesNames);
        Terrain.ReadSTLBinary("C:/Users/jekabins/Downloads/Terrain.stl");


        for (std::string s : texturesNames)
            _data->AssetManager.LoadTextureMTL(s, "C:/Users/jekabins/Downloads/obj.mtl");


        /*for (int i = 0;i < _mesh.tris.size();i++)
        {
            for (int j = 0;j < 3;j++)
            {
                std::cout << _mesh.tris[i].p[j].x << ' ' << _mesh.tris[i].p[j].y << ' ' << _mesh.tris[i].p[j].z << ' ';
            }
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
                    vCamera = _data->tools.VectorAdd(vCamera, _data->tools.MultiplyVector(vLookDir, fMovementSpeed));
                if (event.key.code == sf::Keyboard::S)
                    vCamera = _data->tools.VectorAdd(vCamera, _data->tools.MultiplyVector(vLookDir, -fMovementSpeed));
                if (event.key.code == sf::Keyboard::A)
                {
                    vec3d right = _data->tools.crossProduct(vLookDir, { 0.0f, 1.0f, 0.0f });
                    right = _data->tools.normalizeVector(right);  // Make sure the right vector is normalized
                    vCamera = _data->tools.VectorAdd(vCamera, _data->tools.MultiplyVector(right, -fMovementSpeed));
                }
                if (event.key.code == sf::Keyboard::D)
                {
                    vec3d right = _data->tools.crossProduct(vLookDir, { 0.0f, 1.0f, 0.0f });
                    right = _data->tools.normalizeVector(right);  // Normalize

                    vCamera = _data->tools.VectorAdd(vCamera, _data->tools.MultiplyVector(right, fMovementSpeed));
                }
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
        //fThetay = 1.0f * clock.getElapsedTime().asSeconds();
        vec3d vLight{ -3, 1, -4 };
        vLight = _data->tools.normalizeVector(vLight);
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
        if (vCamera.y <= 30)
        {
            CameraYv = 0;
            vCamera.y = 30;
        }


        _data->tools.UpdateCamera(fYaw, fPitch, vCamera, vLookDir, matView);

        float ObjYaw;
        float ObjPitch;
        vec3d Obj = { 0, 0, 250 };
        _data->tools.LookAtCamera(Obj, vCamera, ObjYaw, ObjPitch);
            

        _data->tools.TransformObj(fThetax - 1.57079632679, ObjYaw + 3.14159, fThetaz, 0, 0, 250, _mesh, ObjToClipp);
        _data->tools.TransformObj(1.57079632679, 0, 0, 0, -5, 0, Terrain, TerrainToClipp);
        _data->tools.TransformObj(1.57079632679, 0, 0, 997, -5, 0, Terrain, TerrainToClipp2);
        _data->tools.TransformObj(1.57079632679, 0, 0, 0, -5, 997, Terrain, TerrainToClipp3);
        _data->tools.TransformObj(1.57079632679, 0, 0, 997, -5, 997, Terrain, TerrainToClipp4);


        _data->tools.CameraClipp(ObjToClipp, vCamera, vLookDir, vLookDir, matView, matProj, ObjToDraw);
        _data->tools.CameraClipp(TerrainToClipp, vCamera, vLookDir, vLight, matView, matProj, TerrainToDraw);
        _data->tools.CameraClipp(TerrainToClipp2, vCamera, vLookDir, vLight, matView, matProj, TerrainToDraw2);
        _data->tools.CameraClipp(TerrainToClipp3, vCamera, vLookDir, vLight, matView, matProj, TerrainToDraw3);
        _data->tools.CameraClipp(TerrainToClipp4, vCamera, vLookDir, vLight, matView, matProj, TerrainToDraw4);
        
        std::sort(ObjToDraw.tris.begin(), ObjToDraw.tris.end(), CompareByAverageZ);
        std::sort(TerrainToDraw.tris.begin(), TerrainToDraw.tris.end(), CompareByAverageZ);
        std::sort(TerrainToDraw2.tris.begin(), TerrainToDraw2.tris.end(), CompareByAverageZ);
        std::sort(TerrainToDraw3.tris.begin(), TerrainToDraw3.tris.end(), CompareByAverageZ);
        std::sort(TerrainToDraw4.tris.begin(), TerrainToDraw4.tris.end(), CompareByAverageZ);


        _data->tools.ClipNDraw(TerrainToDraw4, _data->window, _data->AssetManager._textures);
        _data->tools.ClipNDraw(TerrainToDraw2, _data->window, _data->AssetManager._textures);
        _data->tools.ClipNDraw(TerrainToDraw3, _data->window,  _data->AssetManager._textures);
        _data->tools.ClipNDraw(TerrainToDraw, _data->window, _data->AssetManager._textures);
        _data->tools.ClipNDraw(ObjToDraw, _data->window, _data->AssetManager._textures);


        //std::cout << _data->AssetManager.GetTexture("color_15277357").getSize().x << ' ' << _data->AssetManager.GetTexture("color_15277357").getSize().y;
        //std::cout << '\n';
    }



	void StartState::Draw(float dt)
	{
		//_data->window.clear();
		//_data->window.draw(rect);
		_data->window.display();
	}
}