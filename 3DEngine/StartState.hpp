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
		mat4x4 matProj;
		float fThetax, fThetay, fThetaz;
		float fYaw = 1.57079632679f;   // Yaw (left/right rotation)
		float fPitch = 0.0f; // Pitch (up/down rotation)
		float CameraYv = 0.0f;
		vec3d vLookDir, vCamera;
		bool WPressed, SPressed, APressed, DPressed, SpacePressed, OnGround, hidden, LShiftPressed;
		sf::Vector2i CameraPos2d;
		int vcameraFinal, nrOfMeshes;
		mesh Cube, Cube2;
		vec3d vLight;
		mat4x4 matView;
		std::vector <mesh> MeshesToRender, MeshesToTranform, MeshesTransformed;





	};
}