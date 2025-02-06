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
		void ChooseCard();
	private:
		GameDataRef _data;
		sf::Clock clock, moveclock, RollCooldown;
		mat4x4 matProj;
		float fThetax, fThetay, fThetaz;
		float fYaw = 1.57079632679f;   // Yaw (left/right rotation)
		float fPitch = 0.0f; // Pitch (up/down rotation)
		float CameraYv = 0.0f;
		sf::Texture texture;
		vec3d vLookDir, vCamera;
		bool WPressed, SPressed, APressed, DPressed, SpacePressed, OnGround, hidden, DrawCard;
		bool SwitchPlayers, RollCheck, skipOtherPlayer, InfoOpen, ReadyToRoll;
		sf::Vector2i CameraPos2d;
		sf::Sprite RollButton, InfoButton, InfoScreen, Card;
		sf::Sprite icon[3];
		sf::Text rollNumber, LevelTxt, Debug;
		vec3d locationCoordinates[20];
		player players[3];
		mesh Levels[10];
		sf::Color Background[10];
		int vcameraFinal, cardRolled, PlayerToMove, nrFinished, level;
		int playersToSkip[3];
		int NextMoveTwice[3];
		int NoBonuses[3];
		int NoPunishments[3];
		std::string InfoScreenPaths[10], LevelNames[10];
		sf::RectangleShape Effects[10];
	};
}