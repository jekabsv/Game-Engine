#pragma once

#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "Game.hpp"
#include <array>


namespace game
{
	class player
	{
	public:
		int location;
		int ToMove;
		mesh Obj;
		bool finished = 0;
		int points = 0;
		sf::Text pointsTxt;
	};

	class StartState : public State
	{
	public:
		StartState(GameDataRef data);
		void Init() override;
		void HandleInput() override;
		void Update(float dt) override;
		void Draw(float dt) override;
		void Pause() override;
		void Resume() override;
	private:
		GameDataRef _data;
		sf::Clock clock;
		mesh Terrain;
		float fThetax;
		float fThetay;
		float fThetaz;
		mat4x4 matProj;
		float fYaw = 1.57079632679f;   // Yaw (left/right rotation)
		float fPitch = 0.0f; // Pitch (up/down rotation)
		vec3d vCamera;
		float CameraYv = 0.0f;
		sf::Texture texture;
		vec3d vLookDir;
		bool WPressed, SPressed, APressed, DPressed, SpacePressed, OnGround, hidden;
		sf::Vector2i CameraPos2d;
		sf::Text LevelTxt;
		int level;
		sf::Sprite RollButton;
		sf::Text rollNumber;
		vec3d locationCoordinates[20];
		sf::Clock moveclock;
		
		bool ReadyToRoll;
		sf::Clock RollCooldown;
		int PlayerToMove;
		player players[3];
		bool playersToSkip[3];
		sf::Sprite Card;
		bool DrawCard;
		mesh Levels[9];
		int nrFinished = 0;

		sf::Sprite icon[3];
		sf::Color Background[9];

		int vcameraFinal = 0;

	};
}