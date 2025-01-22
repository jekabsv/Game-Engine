#pragma once

#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "Game.hpp"
#include <array>


namespace game
{

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
		mesh _mesh;
		float fThetax;
		float fThetay;
		float fThetaz;
		mat4x4 matProj;
	};
}