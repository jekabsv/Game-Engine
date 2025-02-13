#include <sstream>
#include "StartState.hpp"

#include <algorithm>
#include <iostream>
#include <list>

namespace game
{
    float CalculateAverageZ(const triangle& t)
    {
        return (t.p[0].z + t.p[1].z + t.p[2].z) / 3.0f;
    }
    bool CompareByAverageZ(const triangle& t1, const triangle& t2)
    {
        return CalculateAverageZ(t1) > CalculateAverageZ(t2);
    }
	
    StartState::StartState(GameDataRef data) : _data(data)
	{


	}
	void StartState::Init()
	{

        MeshesToTranform.clear();
        MeshesToTranform.resize(2);

        vLight = { -0.6f, 0.2f, -0.8f };
        std::vector <std::string> Textures;
        mesh cube;

        MeshesToTranform[0].ReadOBJ("../../Resources/Obj.obj", Textures);
        for (auto x : Textures)
            _data->AssetManager.LoadTextureMTL(x, "../../Resources/Obj.mtl");


       MeshesToTranform[1].ReadOBJ("../../Resources/Obj.obj", Textures);
        for (auto x : Textures)
            _data->AssetManager.LoadTextureMTL(x, "../../Resources/Obj.mtl");



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
        float fRotationSpeed = 2.0f / 60;
        float fMovementSpeed = 5.0f;

        sf::Event event;
        if (WPressed)
        {
            float y = vCamera.y;
            vCamera = _data->tools.vectorArithmetic.VectorAdd(vCamera, _data->tools.vectorArithmetic.MultiplyVector(vLookDir, fMovementSpeed));
            vCamera.y = y;
        }
        if (SPressed)
        {
            float y = vCamera.y;
            vCamera = _data->tools.vectorArithmetic.VectorAdd(vCamera, _data->tools.vectorArithmetic.MultiplyVector(vLookDir, -fMovementSpeed));
            vCamera.y = y;
        }
        if (APressed)
        {
            vec3d right = _data->tools.vectorArithmetic.crossProduct(vLookDir, { 0.0f, 1.0f, 0.0f });
            right = _data->tools.vectorArithmetic.normalizeVector(right);
            float y = vCamera.y;
            vCamera = _data->tools.vectorArithmetic.VectorAdd(vCamera, _data->tools.vectorArithmetic.MultiplyVector(right, -fMovementSpeed));
            vCamera.y = y;
        }
        if (DPressed)
        {
            vec3d right = _data->tools.vectorArithmetic.crossProduct(vLookDir, { 0.0f, 1.0f, 0.0f });
            right = _data->tools.vectorArithmetic.normalizeVector(right);
            float y = vCamera.y;
            vCamera = _data->tools.vectorArithmetic.VectorAdd(vCamera, _data->tools.vectorArithmetic.MultiplyVector(right, fMovementSpeed));
            vCamera.y = y;
        }
        if (SpacePressed)
        {
            vCamera.y += 2;
        }
        if (LShiftPressed)
        {
            vCamera.y -= 2;
        }

        while (_data->window.pollEvent(event))
        {
            if (sf::Event::Closed == event.type)
            {
                _data->window.close();
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                hidden = 1;
            }
            else hidden = 0;

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Left)
                    fYaw -= fRotationSpeed;
                if (event.key.code == sf::Keyboard::Right)
                    fYaw += fRotationSpeed;
                if (event.key.code == sf::Keyboard::Up)
                    fPitch += fRotationSpeed;
                if (event.key.code == sf::Keyboard::Down)
                    fPitch -= fRotationSpeed;


                if (event.key.code == sf::Keyboard::W)
                    WPressed = true;
                if (event.key.code == sf::Keyboard::S)
                    SPressed = true;
                if (event.key.code == sf::Keyboard::A)
                    APressed = true;
                if (event.key.code == sf::Keyboard::D)
                    DPressed = true;
                if (event.key.code == sf::Keyboard::Space)
                    SpacePressed = true;
                if (event.key.code == sf::Keyboard::LShift)
                    LShiftPressed = true;
            }
            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::W)
                    WPressed = false;
                if (event.key.code == sf::Keyboard::S)
                    SPressed = false;
                if (event.key.code == sf::Keyboard::A)
                    APressed = false;
                if (event.key.code == sf::Keyboard::D)
                    DPressed = false;
                if (event.key.code == sf::Keyboard::Space)
                    SpacePressed = false;
                if (event.key.code == sf::Keyboard::LShift)
                    LShiftPressed = false;
            }
        }
    }
    void StartState::Update(float dt)
    {
        MeshesTransformed.clear();
        MeshesToRender.clear();
        MeshesTransformed.resize(2);
        MeshesToRender.resize(2);


        if (hidden)
            _data->window.setMouseCursorVisible(0);
        else _data->window.setMouseCursorVisible(1);
        sf::Vector2i CameraPos2d2;
        CameraPos2d2 = _data->inputManager.GetMousePosition(_data->window);
        if(hidden)
        {
            fYaw += (CameraPos2d2.x - CameraPos2d.x) / 180.0f;
            fPitch -= (CameraPos2d2.y - CameraPos2d.y) / 180.0f;
            if (CameraPos2d2.x > SCREEN_WIDTH-20 || CameraPos2d2.x < 20 || CameraPos2d2.y > SCREEN_HEIGHT-20 || CameraPos2d2.y < 20)
            {
                sf::Mouse::setPosition(sf::Vector2i(960, 540));
                CameraPos2d2 = { 960, 540 };
            }
        }
        CameraPos2d = CameraPos2d2;


        _data->tools.rendering3D.UpdateCamera(fYaw, fPitch, vCamera, vLookDir, matView);








        mesh CubeToCameraClipp, CubeToWallClipp, CubeToDraw;
        mesh CubeToCameraClipp2, CubeToWallClipp2, CubeToDraw2;


        _data->tools.rendering3D.TransformObj(-3.1415926535 / 2, 0, 0, 0, 0, 0, 1, 1, 1, MeshesToTranform[0], MeshesTransformed[0], 255);
        _data->tools.rendering3D.TransformObj(-3.1415926535 / 2, 0, 0, 0, 0, 50, 1, 1, 1, MeshesToTranform[1], MeshesTransformed[1], 255);

        

        //_data->tools.rendering3D.Clip(MeshesTransformed, MeshesToRender, vCamera, vLookDir, vLight, matView, matProj);
        _data->tools.rendering3D.CameraClipp(MeshesTransformed[0], vCamera, vLookDir, vLight, matView, matProj, MeshesToRender[0]);
        _data->tools.rendering3D.CameraClipp(MeshesTransformed[1], vCamera, vLookDir, vLight, matView, matProj, MeshesToRender[1]);



        _data->tools.rendering3D.WallClipp(CubeToWallClipp, CubeToDraw);
        _data->tools.rendering3D.WallClipp(CubeToWallClipp2, CubeToDraw2);




        _data->window.clear();
    }


    
	void StartState::Draw(float dt)
	{
        //_data->tools.rendering3D.DrawMesh(MeshesToRender[0], _data->window, _data->AssetManager._textures, 255);
        //_data->tools.rendering3D.DrawMesh(MeshesToRender[1], _data->window, _data->AssetManager._textures, 255);
        _data->tools.rendering3D.DrawMeshesWithDepthBuffer(MeshesToRender, _data->window, _data->AssetManager._textures);
		_data->window.display();
	}
}