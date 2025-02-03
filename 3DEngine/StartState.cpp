#include <sstream>
#include "StartState.hpp"

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
        location = 0;
        OnGround = false;
        CameraPos2d = _data->inputManager.GetMousePosition(_data->window);

        locationCoordinates[0] = { 0, 0, 87 };
        locationCoordinates[1] = { 26.89, 0, 82.78 };

        for (int i = 0; i < 20; ++i) {
            double angleDeg = i * 18;
            double angleRad = angleDeg * 3.1415926535f / 180.0f;

            double x = 87 * sin(angleRad);
            double y = 87 * cos(angleRad);

            locationCoordinates[i].x = x;
            locationCoordinates[i].z = y;
        }



        vCamera = { 95, 76, -95 };
        fYaw = 2.3;
        fPitch = -0.5;
		//_data->tools.AddCube(-0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, _mesh);
        std::vector <std::string> texturesNames;

        _data->AssetManager.LoadTexture("DiceRoll", "../../Resources/Dice.png");
        RollButton.setTexture(_data->AssetManager.GetTexture("DiceRoll"));
        RollButton.setPosition(50, 50);
        RollButton.setScale(0.1, 0.1);
        _data->AssetManager.LoadFont("MainFont", "../../Resources/Font.ttf");
        _data->AssetManager.LoadFont("Numbers", "../../Resources/Numbers.otf");

        rollNumber.setFont(_data->AssetManager.GetFont("Numbers"));
        rollNumber.setString("0");
        rollNumber.setScale(2, 2);
        rollNumber.setPosition(200, 50);


        Level.setFont(_data->AssetManager.GetFont("MainFont"));
        Level.setString("LIMBO");
        Level.setScale(3, 3);
        Level.setPosition(1550, 50);




        _mesh.ReadOBJ("../../Resources/Object.obj" , texturesNames);
        for (std::string s : texturesNames)
            _data->AssetManager.LoadTextureMTL(s, "../../Resources/Object.mtl");

        Terrain.ReadOBJ("../../Resources/Limbo2.obj", texturesNames);
        for (std::string s : texturesNames)
            _data->AssetManager.LoadTextureMTL(s, "../../Resources/Limbo2.mtl");



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
            vCamera = _data->tools.VectorAdd(vCamera, _data->tools.MultiplyVector(vLookDir, fMovementSpeed));
            vCamera.y = y;
        }
        if (SPressed)
        {
            float y = vCamera.y;
            vCamera = _data->tools.VectorAdd(vCamera, _data->tools.MultiplyVector(vLookDir, -fMovementSpeed));
            vCamera.y = y;
        }
        if (APressed)
        {
            vec3d right = _data->tools.crossProduct(vLookDir, { 0.0f, 1.0f, 0.0f });
            right = _data->tools.normalizeVector(right);
            float y = vCamera.y;
            vCamera = _data->tools.VectorAdd(vCamera, _data->tools.MultiplyVector(right, -fMovementSpeed));
            vCamera.y = y;
        }
        if (DPressed)
        {
            vec3d right = _data->tools.crossProduct(vLookDir, { 0.0f, 1.0f, 0.0f });
            right = _data->tools.normalizeVector(right);
            float y = vCamera.y;
            vCamera = _data->tools.VectorAdd(vCamera, _data->tools.MultiplyVector(right, fMovementSpeed));
            vCamera.y = y;
        }
        if (SpacePressed && OnGround)
        {
            CameraYv += 4.0f;
        }
		while (_data->window.pollEvent(event))
		{
			if (sf::Event::Closed == event.type)
			{
				_data->window.close();
			}
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !_data->inputManager.IsSpriteHovered(RollButton, _data->window))
            {
                hidden = 1;
            }
            else hidden = 0;

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (_data->inputManager.IsSpriteClicked(RollButton, sf::Mouse::Left, _data->window))
                {
                    moveclock.restart();
                    int k;
                    k = _data->tools.Random(1, 6);
                    //location += k;
                    move = k;
                    rollNumber.setString(std::to_string(k));
                }
            }
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
                {
                    //std::cout << vCamera.x << ' ' << vCamera.y << ' ' << vCamera.z << " | " << fYaw << ' ' << fPitch << '\n';
                    //location++;
                    //finallocation++;
                    //move++;
                    move += 10;
                   // vCamera.y -= fMovementSpeed;
                }
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
            }
		}
	}
    void StartState::Update(float dt)
    {

        _data->window.clear(sf::Color(20, 20, 20));

        location = location % 20;
        if (moveclock.getElapsedTime().asSeconds() > 0.5 && !move)
        {
            if (location == 10)
                location = 7;
            if (location == 11)
                location = 1;
            if (location == 5)
                location = 2;
        }
        if (moveclock.getElapsedTime().asSeconds() > 0.5 && move)
        {
            moveclock.restart();
            location++;
            move--;
            location = location % 20;
        }

        


        if (hidden)
            _data->window.setMouseCursorVisible(0);
        else _data->window.setMouseCursorVisible(1);


        

        sf::Vector2i CameraPos2d2;
        CameraPos2d2 = _data->inputManager.GetMousePosition(_data->window);
        if(hidden)
        {
            fYaw += (CameraPos2d2.x - CameraPos2d.x) / 180.0f;
            fPitch -= (CameraPos2d2.y - CameraPos2d.y) / 180.0f;

            if ((CameraPos2d2.x - CameraPos2d.x) > 100)
            {
                std::cout << (CameraPos2d2.x - CameraPos2d.x) << '\n';
            }

            //std::cout << (CameraPos2d2.x - CameraPos2d.x) << '\n';

            if (CameraPos2d2.x > 1900 || CameraPos2d2.x < 20 || CameraPos2d2.y > 1060 || CameraPos2d2.y < 20)
            {
                sf::Mouse::setPosition(sf::Vector2i(960, 540));
                CameraPos2d2 = { 960, 540 };
            }
        }
        CameraPos2d = CameraPos2d2;
        






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


        float g = 0;
        CameraYv -= g;

        vCamera.y += CameraYv;
        if (vCamera.y <= 30)
        {
            CameraYv = 0;
            vCamera.y = 30;
            OnGround = true;
        }
        else OnGround = false;


        _data->tools.UpdateCamera(fYaw, fPitch, vCamera, vLookDir, matView);

        float ObjYaw;
        float ObjPitch;
        vec3d Obj = { 0, 0, 250 };
        //_data->tools.LookAtCamera(Obj, vCamera, ObjYaw, ObjPitch);
            

        _data->tools.TransformObj(fThetax - 1.57079632679, 0, fThetaz, locationCoordinates[location].x, locationCoordinates[location].y, locationCoordinates[location].z, 0.3, 0.3, 0.3, _mesh, ObjToClipp);
        _data->tools.TransformObj(-1.57079632679, 0, 0, 0, -5, 0, 1, 1, 1, Terrain, TerrainToClipp);

        _data->tools.TransformObj(1.57079632679, 0, 0, 997, -5, 0, 1, 1, 1, Terrain, TerrainToClipp2);
        _data->tools.TransformObj(1.57079632679, 0, 0, 0, -5, 997,1, 1, 1, Terrain, TerrainToClipp3);
        _data->tools.TransformObj(1.57079632679, 0, 0, 997, -5, 997, 1, 1, 1, Terrain, TerrainToClipp4);


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
		_data->window.draw(Level);
        _data->window.draw(rollNumber);
        _data->window.draw(RollButton);
		_data->window.display();
	}
}