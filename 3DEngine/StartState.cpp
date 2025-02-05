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
        skipOtherPlayer = 0;
        RollCheck = 0;
        SwitchPlayers = 0;
        vcameraFinal = 75;
        Background[0] = sf::Color(20, 20, 20);
        Background[1] = sf::Color(45, 30, 30);
        level = 0;

        nrFinished = 0;
        DrawCard = 0;
        PlayerToMove = 2;
        OnGround = false;
        CameraPos2d = _data->inputManager.GetMousePosition(_data->window);

        for (int i = 0;i < 3;i++)
        {
            playersToSkip[i] = 0;
            NextMoveTwice[i] = 0;
            NoBonuses[i] = 0;
            NoPunishments[i] = 0;
        }


        for (int i = 0; i < 20; ++i) {
            double angleDeg = i * 18;
            double angleRad = angleDeg * 3.1415926535f / 180.0f;

            double x = 87 * sin(angleRad);
            double y = 87 * cos(angleRad);

            locationCoordinates[i].x = x;
            locationCoordinates[i].z = y;
            locationCoordinates[i].y = -150*i;
        }








        vCamera = { 95, 75, -95 };
        fYaw = 2.3;
        fPitch = -0.5;
		//_data->tools.AddCube(-0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, _mesh);
        std::vector <std::string> texturesNames;


        _data->AssetManager.LoadTexture("LimboCard1", "../../Resources/Cards/LimboCard1.png");
        _data->AssetManager.LoadTexture("LimboCard2", "../../Resources/Cards/LimboCard2.png");
        _data->AssetManager.LoadTexture("LimboCard3", "../../Resources/Cards/LimboCard3.png");
        _data->AssetManager.LoadTexture("LimboCard4", "../../Resources/Cards/LimboCard4.png");

        _data->AssetManager.LoadTexture("LustCard1", "../../Resources/Cards/LustCard1.png");
        _data->AssetManager.LoadTexture("LustCard2", "../../Resources/Cards/LustCard2.png");
        _data->AssetManager.LoadTexture("LustCard3", "../../Resources/Cards/LustCard3.png");

        _data->AssetManager.LoadTexture("GluttonyCard1", "../../Resources/Cards/GluttonyCard1.png");
        _data->AssetManager.LoadTexture("GluttonyCard2", "../../Resources/Cards/GluttonyCard2.png");
        _data->AssetManager.LoadTexture("GluttonyCard3", "../../Resources/Cards/GluttonyCard3.png");
        _data->AssetManager.LoadTexture("GluttonyCard4", "../../Resources/Cards/GluttonyCard4.png");

        _data->AssetManager.LoadTexture("GreedCard1", "../../Resources/Cards/GreedCard1.png");
        _data->AssetManager.LoadTexture("GreedCard2", "../../Resources/Cards/GreedCard2.png");
        _data->AssetManager.LoadTexture("GreedCard3", "../../Resources/Cards/GreedCard3.png");
        _data->AssetManager.LoadTexture("GreedCard4", "../../Resources/Cards/GreedCard4.png");

        _data->AssetManager.LoadTexture("WrathCard1", "../../Resources/Cards/WrathCard1.png");
        _data->AssetManager.LoadTexture("WrathCard2", "../../Resources/Cards/WrathCard2.png");
        _data->AssetManager.LoadTexture("WrathCard3", "../../Resources/Cards/WrathCard3.png");
        _data->AssetManager.LoadTexture("WrathCard4", "../../Resources/Cards/WrathCard4.png");

        _data->AssetManager.LoadTexture("HeresyCard1", "../../Resources/Cards/HeresyCard1.png");
        _data->AssetManager.LoadTexture("HeresyCard2", "../../Resources/Cards/HeresyCard2.png");
        _data->AssetManager.LoadTexture("HeresyCard3", "../../Resources/Cards/HeresyCard3.png");
        _data->AssetManager.LoadTexture("HeresyCard4", "../../Resources/Cards/HeresyCard4.png");

        _data->AssetManager.LoadTexture("ViolenceCard1", "../../Resources/Cards/ViolenceCard1.png");
        _data->AssetManager.LoadTexture("ViolenceCard2", "../../Resources/Cards/ViolenceCard2.png");
        _data->AssetManager.LoadTexture("ViolenceCard3", "../../Resources/Cards/ViolenceCard3.png");

        _data->AssetManager.LoadTexture("FraudCard1", "../../Resources/Cards/FraudCard1.png");
        _data->AssetManager.LoadTexture("FraudCard2", "../../Resources/Cards/FraudCard2.png");
        _data->AssetManager.LoadTexture("FraudCard3", "../../Resources/Cards/FraudCard3.png");
        _data->AssetManager.LoadTexture("FraudCard4", "../../Resources/Cards/FraudCard4.png");

        _data->AssetManager.LoadTexture("BetrayalCard1", "../../Resources/Cards/BetrayalCard1.png");
        _data->AssetManager.LoadTexture("BetrayalCard2", "../../Resources/Cards/BetrayalCard2.png");
        _data->AssetManager.LoadTexture("BetrayalCard3", "../../Resources/Cards/BetrayalCard3.png");
        _data->AssetManager.LoadTexture("BetrayalCard4", "../../Resources/Cards/BetrayalCard4.png");


        Card.setTexture(_data->AssetManager.GetTexture("LimboCard1"));
        Card.setScale(1, 1);
        Card.setOrigin(Card.getGlobalBounds().width/2, Card.getGlobalBounds().height/2);
        Card.setPosition(960, 540);


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


        LevelTxt.setFont(_data->AssetManager.GetFont("MainFont"));
        LevelTxt.setString("LIMBO");
        LevelTxt.setScale(3, 3);
        LevelTxt.setPosition(1550, 50);

        players[0].pointsTxt.setFont(_data->AssetManager.GetFont("Numbers"));
        players[0].pointsTxt.setString("0");
        players[0].pointsTxt.setPosition(820, 130);
        players[1].pointsTxt.setFont(_data->AssetManager.GetFont("Numbers"));
        players[1].pointsTxt.setString("0");
        players[1].pointsTxt.setPosition(920, 130);
        players[2].pointsTxt.setFont(_data->AssetManager.GetFont("Numbers"));
        players[2].pointsTxt.setString("0");
        players[2].pointsTxt.setPosition(1020, 130);


        players[0].Obj.ReadOBJ("../../Resources/Player1.obj", texturesNames);
        for (std::string s : texturesNames)
            _data->AssetManager.LoadTextureMTL(s, "../../Resources/Player1.mtl");

        players[1].Obj.ReadOBJ("../../Resources/Player2.obj", texturesNames);
        for (std::string s : texturesNames)
            _data->AssetManager.LoadTextureMTL(s, "../../Resources/Player2.mtl");

        players[2].Obj.ReadOBJ("../../Resources/Player3.obj", texturesNames);
        for (std::string s : texturesNames)
            _data->AssetManager.LoadTextureMTL(s, "../../Resources/Player3.mtl");

        Levels[0].ReadOBJ("../../Resources/Limbo.obj", texturesNames);
        for (std::string s : texturesNames)
            _data->AssetManager.LoadTextureMTL(s, "../../Resources/Limbo.mtl");
        Levels[1].ReadOBJ("../../Resources/Limbo.obj", texturesNames);
        for (std::string s : texturesNames)
            _data->AssetManager.LoadTextureMTL(s, "../../Resources/Limbo.mtl");

        _data->AssetManager.LoadTexture("Icon1", "../../Resources/icon1.png");
        icon[0].setTexture(_data->AssetManager.GetTexture("Icon1"));
        _data->AssetManager.LoadTexture("Icon2", "../../Resources/icon2.png");
        icon[1].setTexture(_data->AssetManager.GetTexture("Icon2"));
        _data->AssetManager.LoadTexture("Icon3", "../../Resources/icon3.png");
        icon[2].setTexture(_data->AssetManager.GetTexture("Icon3"));
        

        icon[0].setPosition(800, 70);
        icon[0].setScale(2, 2);
        icon[1].setPosition(900, 70);
        icon[1].setScale(2, 2);
        icon[2].setPosition(1000, 70);
        icon[2].setScale(2, 2);


       // Levels[1].ReadOBJ("../../Resources/Player3.obj", texturesNames);
        //for (std::string s : texturesNames)
         //   _data->AssetManager.LoadTextureMTL(s, "../../Resources/Player3.mtl");



        

        /*Terrain.ReadOBJ("../../Resources/Limbo2.obj", texturesNames);
        for (std::string s : texturesNames)
            _data->AssetManager.LoadTextureMTL(s, "../../Resources/Limbo2.mtl");*/



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
        if (_data->inputManager.IsSpriteClicked(icon[0], sf::Mouse::Left, _data->window))
        {
            if(SwitchPlayers)
            {
                if (PlayerToMove != 0 && !players[0].finished)
                {
                    int loc = players[PlayerToMove].location;
                    players[PlayerToMove].location = players[0].location;
                    players[0].location = loc;
                    SwitchPlayers = 0;
                }
            }
            if (skipOtherPlayer)
            {
                if (PlayerToMove != 0)
                {
                    skipOtherPlayer = 0;
                    playersToSkip[0] = 1;
                }
            }
        }
        if (_data->inputManager.IsSpriteClicked(icon[1], sf::Mouse::Left, _data->window))
        {
            if (SwitchPlayers)
            {
                if (PlayerToMove != 1 && !players[1].finished)
                {
                    int loc = players[PlayerToMove].location;
                    players[PlayerToMove].location = players[2].location;
                    players[2].location = loc;
                    SwitchPlayers = 0;
                }
            }
            if (skipOtherPlayer)
            {
                if (PlayerToMove != 1)
                {
                    skipOtherPlayer = 0;
                    playersToSkip[1] = 1;
                }
            }
        }
        if (_data->inputManager.IsSpriteClicked(icon[2], sf::Mouse::Left, _data->window))
        {
            if (SwitchPlayers)
            {
                if (PlayerToMove != 2 && !players[2].finished)
                {
                    int loc = players[PlayerToMove].location;
                    players[PlayerToMove].location = players[2].location;
                    players[2].location = loc;
                    SwitchPlayers = 0;
                }
            }
            if (skipOtherPlayer)
            {
                if (PlayerToMove != 2)
                {
                    skipOtherPlayer = 0;
                    playersToSkip[2] = 1;
                }
            }
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
                if (_data->inputManager.IsSpriteClicked(RollButton, sf::Mouse::Left, _data->window) && RollCooldown.getElapsedTime().asSeconds() > 0.5 
                    && !players[PlayerToMove].ToMove && !DrawCard && !SwitchPlayers && !RollCheck && !skipOtherPlayer)
                {
                    
                    RollCooldown.restart();
                    PlayerToMove++;
                    PlayerToMove = PlayerToMove % 3;
                    if (NoBonuses[PlayerToMove])
                    {
                        NoBonuses[PlayerToMove]--;
                    }
                    if (NoPunishments[PlayerToMove])
                    {
                        NoPunishments[PlayerToMove]--;
                    }
                    if (NextMoveTwice[PlayerToMove] == 1)
                    {
                        NextMoveTwice[PlayerToMove]++;
                    }
                    while(playersToSkip[PlayerToMove])
                    {
                        //Skip Text
                        playersToSkip[PlayerToMove]--;
                        PlayerToMove++;
                        PlayerToMove = PlayerToMove % 3;
                    }
                    if (players[PlayerToMove].finished)
                    {
                        playersToSkip[PlayerToMove] = 0;
                        PlayerToMove++;
                    }
                    if (players[PlayerToMove].finished)
                    {
                        playersToSkip[PlayerToMove] = 0;
                        PlayerToMove++;
                    }


                    moveclock.restart();
                    int k;
                    k = _data->tools.Random(1, 6);
                    //location += k;
                    players[PlayerToMove].ToMove = k;
                    rollNumber.setString(std::to_string(k));
                }
                if (_data->inputManager.IsSpriteClicked(RollButton, sf::Mouse::Left, _data->window)
                    && !DrawCard && RollCheck)
                {
                    RollCheck = 0;
                    int k;
                    k = _data->tools.Random(1, 6);
                    rollNumber.setString(std::to_string(k));
                    if (k > 3)
                    {
                        if (cardRolled / 10 == 0 && cardRolled % 10 == 1)
                        {
                            PlayerToMove--;
                            if (PlayerToMove < 0)
                            {
                                PlayerToMove = 2;
                            }
                        }
                        else
                        {
                            players[PlayerToMove].location += 3;
                            players[PlayerToMove].location %= 20;
                        }
                    }
                    if (k < 4)
                    {
                        if (cardRolled / 10 == 0 && cardRolled % 10 == 1)
                        {

                        }
                        else
                        {
                            players[PlayerToMove].location -= 3;
                            if (players[PlayerToMove].location < 0)
                                players[PlayerToMove].location = 0;
                        }
                    }
                }
                if (_data->inputManager.IsSpriteClicked(Card, sf::Mouse::Left, _data->window) && DrawCard)
                {
                    DrawCard = 0;
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
                    PlayerToMove++;
                    PlayerToMove = PlayerToMove % 3;
                    players[PlayerToMove].ToMove += 1;
                    
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
        //Card.setTexture(_data->AssetManager.GetTexture("NoCard"));


        //std::cout << vcameraFinal << ' ';
        /*if (vCamera.y > vcameraFinal)
        {
            std::cout << "hello " << vCamera.y << '\n';
            --vCamera.y;
            
        }*/
        _data->window.clear(Background[level]);




        players[PlayerToMove].location = players[PlayerToMove].location % 20;
        if (moveclock.getElapsedTime().asSeconds() > 0.3 && !players[PlayerToMove].ToMove)
        {
            if(level == 0)
            {
                if (players[PlayerToMove].location == 10)
                    players[PlayerToMove].location = 7;
                if (players[PlayerToMove].location == 11)
                    players[PlayerToMove].location = 1;
                if (players[PlayerToMove].location == 5)
                    players[PlayerToMove].location = 2;
            }
            
            
        }
        if (moveclock.getElapsedTime().asSeconds() > 0.3 && players[PlayerToMove].ToMove)
        {
            moveclock.restart();
            players[PlayerToMove].location++;
            players[PlayerToMove].ToMove--;
            players[PlayerToMove].location = players[PlayerToMove].location % 20;

            if ((players[PlayerToMove].location == 1 || players[PlayerToMove].location == 4 || players[PlayerToMove].location == 8 ||
                players[PlayerToMove].location == 12 || players[PlayerToMove].location == 16 || players[PlayerToMove].location == 18) && !players[PlayerToMove].ToMove && level == 0)
            {
                DrawCard = 1;
                Card.setTexture(_data->AssetManager.GetTexture("NoCard"));
                int k = _data->tools.Random(0, 8);
                if (k == 0)
                {
                    int c = _data->tools.Random(0, 3);
                    cardRolled = 10 * k + c;
                    if (c == 0 && !NoBonuses[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("LimboCard1"));
                        players[PlayerToMove].location += 2;
                        players[PlayerToMove].location %= 20;
                    }
                    if (c == 1)
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("LimboCard2"));
                        RollCheck = 1;
                    }
                    if (c == 2 && !NoPunishments[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("LimboCard3"));
                        playersToSkip[PlayerToMove] = 1;
                    }
                    if (c == 3 && !NoBonuses[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("LimboCard4"));
                        skipOtherPlayer = 1;
                    }
                }
                if (k == 1)
                {
                    int c = _data->tools.Random(0, 2);
                    cardRolled = 10 * k + c;
                    if (c == 0 && !NoBonuses[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("LustCard1"));
                        skipOtherPlayer = 1;
                    }
                    if (c == 1 && !NoBonuses[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("LustCard2"));
                        PlayerToMove--;
                        if (PlayerToMove < 0)
                            PlayerToMove = 2;
                    }
                    if (c == 2 && !NoPunishments[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("LustCard3"));
                        players[PlayerToMove].location -= 2;
                        if (players[PlayerToMove].location < 0)
                            players[PlayerToMove].location = 0;
                    }
                }
                if (k == 2)
                {
                    int c = _data->tools.Random(0, 3);
                    cardRolled = 10 * k + c;
                    if (c == 0 && !NoBonuses[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("GluttonyCard1"));
                        NextMoveTwice[PlayerToMove] = 1;
                    }
                    if (c == 1 && !NoPunishments[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("GluttonyCard2"));
                        playersToSkip[PlayerToMove] = 1;
                    }
                    if (c == 2 && !NoPunishments[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("GluttonyCard3"));
                        players[PlayerToMove].location -= 3;
                        if (players[PlayerToMove].location < 0)
                            players[PlayerToMove].location = 0;
                    }
                    if (c == 3 && !NoBonuses[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("GluttonyCard4"));
                        players[PlayerToMove].location += 2;
                        players[PlayerToMove].location %= 20;
                    }
                }
                if (k == 3)
                {
                    int c = _data->tools.Random(0, 3);
                    cardRolled = 10 * k + c;
                    if (c == 0 && !NoPunishments[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("GreedCard1"));
                        players[PlayerToMove].location -= 2;
                        if (players[PlayerToMove].location < 0)
                            players[PlayerToMove].location = 0;
                    }
                    if (c == 1 && !NoPunishments[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("GreedCard2"));
                        playersToSkip[PlayerToMove] = 1;
                    }
                    if (c == 2 && !NoBonuses[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("GreedCard3"));
                        PlayerToMove--;
                        if (PlayerToMove < 0)
                            PlayerToMove = 2;
                    }
                    if (c == 3 && !NoBonuses[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("GreedCard4"));
                        players[PlayerToMove].location += 3;
                        players[PlayerToMove].location %= 20;
                    }
                }
                if (k == 4)
                {
                    int c = _data->tools.Random(0, 3);
                    cardRolled = 10 * k + c;
                    if (c == 0 && !NoPunishments[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("WrathCard1"));
                        playersToSkip[PlayerToMove] = 1;
                    }
                    if (c == 1 && !NoPunishments[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("WrathCard2"));
                        players[PlayerToMove].location -= 3;
                        if (players[PlayerToMove].location < 0)
                            players[PlayerToMove].location = 0;
                    }
                    if (c == 2 && !NoBonuses[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("WrathCard3"));
                        int locmax;
                        if (PlayerToMove == 0)
                            locmax = std::max(players[1].location, players[2].location);
                        if (PlayerToMove == 1)
                            locmax = std::max(players[0].location, players[2].location);
                        if (PlayerToMove == 1)
                            locmax = std::max(players[1].location, players[0].location);
                        locmax++;
                        locmax %= 20;
                        if(players[PlayerToMove].location < locmax)
                            players[PlayerToMove].location = locmax;
                    }
                    if (c == 3 && !NoBonuses[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("WrathCard4"));
                        players[PlayerToMove].location += 2;
                        players[PlayerToMove].location %= 20;
                    }
                }
                if (k == 5)
                {
                    int c = _data->tools.Random(0, 3);
                    cardRolled = 10 * k + c;
                    if (c == 0 && !NoBonuses[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("HeresyCard1"));
                        players[PlayerToMove].location += 2;
                        players[PlayerToMove].location %= 20;
                    }
                    if (c == 1 && !NoPunishments[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("HeresyCard2"));
                        NoBonuses[PlayerToMove] = 3;
                    }
                    if (c == 2 && !NoBonuses[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("HeresyCard3"));
                        NoPunishments[PlayerToMove] = 3;
                    }
                    if (c == 3 && !NoPunishments[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("HeresyCard4"));
                        playersToSkip[PlayerToMove] = 1;
                    }
                }
                if (k == 6)
                {
                    int c = _data->tools.Random(0, 2);
                    cardRolled = 10 * k + c;
                    if (c == 0 && !NoBonuses[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("ViolenceCard1"));
                        players[PlayerToMove].location += 4;
                        players[PlayerToMove].location %= 20;
                    }
                    if (c == 1 && !NoPunishments[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("ViolenceCard2"));
                        playersToSkip[PlayerToMove] = 1;
                    }
                    if (c == 2 && !NoPunishments[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("ViolenceCard3"));
                        players[PlayerToMove].location -= 2;
                        if (players[PlayerToMove].location < 0)
                            players[PlayerToMove].location = 0;
                    }
                }
                if (k == 7)
                {
                    int c = _data->tools.Random(0, 2);
                    cardRolled = 10 * k + c;
                    if (c == 0 && !NoPunishments[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("FraudCard1"));
                        players[PlayerToMove].location = 0;
                    }
                    if (c == 1 && !NoPunishments[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("FraudCard2"));
                        playersToSkip[PlayerToMove] = 2;
                    }
                    if (c == 2)
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("FraudCard3"));
                        RollCheck = 1;
                    }
                    if (c == 3 && !NoBonuses[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("FraudCard4"));
                        SwitchPlayers = 1;
                    }
                }
                if (k == 8)
                {
                    int c = _data->tools.Random(0, 3);
                    cardRolled = 10 * k + c;
                    if (c == 0 && !NoBonuses[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("BetrayalCard1"));
                        if(players[PlayerToMove].location != 19)
                        {
                            int locmax;
                            locmax = std::max(players[0].location, std::max(players[1].location, players[2].location));
                            if (locmax == 19)
                                locmax--;
                            players[PlayerToMove].location = locmax;
                        }
                    }
                    if (c == 1 && !NoBonuses[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("BetrayalCard2"));
                        players[PlayerToMove].location += 3;
                        players[PlayerToMove].location %= 20;
                    }
                    if (c == 2)
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("BetrayalCard3"));
                        //+4 or -4
                    }
                    if (c == 3 && !NoPunishments[PlayerToMove])
                    {
                        Card.setTexture(_data->AssetManager.GetTexture("BetrayalCard4"));
                        playersToSkip[PlayerToMove] = 2;
                    }
                }
            }
            if (players[PlayerToMove].location == 19 && !players[PlayerToMove].ToMove)
            {
                if (nrFinished == 0)
                    players[PlayerToMove].points += 2;
                else if (nrFinished == 1)
                    players[PlayerToMove].points += 1;
                players[PlayerToMove].finished = 1;
                nrFinished++;
                players[PlayerToMove].pointsTxt.setString(std::to_string(players[PlayerToMove].points));
            }
        }
       if (NextMoveTwice[PlayerToMove] == 2 && !players[PlayerToMove].ToMove)
       {
           PlayerToMove--;
           NextMoveTwice[PlayerToMove] == 0;
       }


        if (nrFinished == 3)
        {
            level++;
            nrFinished = 0;
            vcameraFinal -= 150;
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
            if (CameraPos2d2.x > 1900 || CameraPos2d2.x < 20 || CameraPos2d2.y > 1060 || CameraPos2d2.y < 20)
            {
                sf::Mouse::setPosition(sf::Vector2i(960, 540));
                CameraPos2d2 = { 960, 540 };
            }
        }
        CameraPos2d = CameraPos2d2;
        






        vec3d vLight{ -0.6, 0.2, -0.8 };
        //vLight = _data->tools.normalizeVector(vLight);


        mat4x4 matView;

        mesh Player1ToClipp, Player1ToDraw;
        mesh Player2ToClipp, Player2ToDraw;
        mesh Player3ToClipp, Player3ToDraw;


        mesh TerrainToClipp, TerrainToDraw;
        mesh TerrainToClipp2, TerrainToDraw2;



        float g = 0.25;
        CameraYv -= g;

        vCamera.y += CameraYv;
       if (vCamera.y <= vcameraFinal)
        {
            CameraYv = 0;
            vCamera.y = vcameraFinal;
            OnGround = true;
        }
        else OnGround = false;


        _data->tools.UpdateCamera(fYaw, fPitch, vCamera, vLookDir, matView);

        float ObjYaw;
        float ObjPitch;
        vec3d Obj = { 0, 0, 250 };
        //_data->tools.LookAtCamera(Obj, vCamera, ObjYaw, ObjPitch);
            



        _data->tools.TransformObj(fThetax - 1.57079632679, 3.14159265358767, fThetaz, locationCoordinates[players[0].location].x,
            locationCoordinates[level].y, locationCoordinates[players[0].location].z, 0.3, 0.3, 0.3, players[0].Obj, Player1ToClipp);
        _data->tools.TransformObj(fThetax - 1.57079632679, 3.14159265358767, fThetaz, locationCoordinates[players[1].location].x,
            locationCoordinates[level].y, locationCoordinates[players[1].location].z, 0.3, 0.3, 0.3, players[1].Obj, Player2ToClipp);
        _data->tools.TransformObj(fThetax - 1.57079632679, 3.14159265358767, fThetaz, locationCoordinates[players[2].location].x,
            locationCoordinates[level].y, locationCoordinates[players[2].location].z, 0.3, 0.3, 0.3, players[2].Obj, Player3ToClipp);


        _data->tools.TransformObj(-1.57079632679, 0, 0, 0, 0, 0, 1, 1, 1, Levels[level], TerrainToClipp);
        _data->tools.TransformObj(-1.57079632679, 0, 0, 0, -150, 0, 1, 1, 1, Levels[level], TerrainToClipp2);

        _data->tools.CameraClipp(Player1ToClipp, vCamera, vLookDir, vLookDir, matView, matProj, Player1ToDraw);
        _data->tools.CameraClipp(Player2ToClipp, vCamera, vLookDir, vLookDir, matView, matProj, Player2ToDraw);
        _data->tools.CameraClipp(Player3ToClipp, vCamera, vLookDir, vLookDir, matView, matProj, Player3ToDraw);

        _data->tools.CameraClipp(TerrainToClipp, vCamera, vLookDir, vLight, matView, matProj, TerrainToDraw);
        _data->tools.CameraClipp(TerrainToClipp2, vCamera, vLookDir, vLight, matView, matProj, TerrainToDraw2);
        
        std::sort(Player1ToDraw.tris.begin(), Player1ToDraw.tris.end(), CompareByAverageZ);
        std::sort(Player2ToDraw.tris.begin(), Player2ToDraw.tris.end(), CompareByAverageZ);
        std::sort(Player3ToDraw.tris.begin(), Player3ToDraw.tris.end(), CompareByAverageZ);

        std::sort(TerrainToDraw.tris.begin(), TerrainToDraw.tris.end(), CompareByAverageZ);
        std::sort(TerrainToDraw2.tris.begin(), TerrainToDraw2.tris.end(), CompareByAverageZ);

        _data->tools.ClipNDraw(TerrainToDraw2, _data->window, _data->AssetManager._textures, 255);
        _data->tools.ClipNDraw(TerrainToDraw, _data->window, _data->AssetManager._textures, 255);

        if(PlayerToMove == 0)
            _data->tools.ClipNDraw(Player1ToDraw, _data->window, _data->AssetManager._textures, 255);
        else _data->tools.ClipNDraw(Player1ToDraw, _data->window, _data->AssetManager._textures, 122);
        if (PlayerToMove == 1)
            _data->tools.ClipNDraw(Player2ToDraw, _data->window, _data->AssetManager._textures, 255);
        else _data->tools.ClipNDraw(Player2ToDraw, _data->window, _data->AssetManager._textures, 122);
        if(PlayerToMove == 2)
             _data->tools.ClipNDraw(Player3ToDraw, _data->window, _data->AssetManager._textures, 255);
        else _data->tools.ClipNDraw(Player3ToDraw, _data->window, _data->AssetManager._textures, 122);
    }



	void StartState::Draw(float dt)
	{
        sf::RectangleShape border;
        border.setSize(sf::Vector2f(64, 64));
        border.setPosition(icon[PlayerToMove].getPosition().x, 70);
        border.setFillColor(sf::Color(0, 0, 0, 0));
        border.setOutlineThickness(3);
        border.setOutlineColor(sf::Color(255, 255, 255));


        _data->window.draw(players[0].pointsTxt);
        _data->window.draw(players[1].pointsTxt);
        _data->window.draw(players[2].pointsTxt);

        _data->window.draw(icon[0]);
        _data->window.draw(icon[1]);
        _data->window.draw(icon[2]);
        _data->window.draw(border);
        if(DrawCard)
            _data->window.draw(Card);

        border.setOutlineColor(sf::Color::Red);
        if (playersToSkip[0])
        {
            border.setPosition(icon[0].getPosition().x, 70);
            _data->window.draw(border);
        }
        if (playersToSkip[1])
        {
            border.setPosition(icon[1].getPosition().x, 70);
            _data->window.draw(border);
        }
        if (playersToSkip[2])
        {
            border.setPosition(icon[2].getPosition().x, 70);
            _data->window.draw(border);
        }


		_data->window.draw(LevelTxt);
        _data->window.draw(rollNumber);
        _data->window.draw(RollButton);
		_data->window.display();
	}
}