#pragma once

#include "DEFINITIONS.hpp"
#include "AssetManager.hpp"
#include "InputManager.hpp"
#include "StateMachine.hpp"
#include "Tools.hpp"
#include <SFML/Graphics.hpp>

#include <memory>
#include <string>

namespace game
{
	struct GameData
	{
		InputManager inputManager;
		StateMachine Machine;
		AssetManager AssetManager;
		sf::RenderWindow window;
		Tools tools;
	};

	typedef std::shared_ptr<GameData> GameDataRef;
	class Game
	{
	public:
		Game(int width, int height, std::string title);
	private:
		const float dt = 1.0f / 60.0f;
		sf::Clock _clock;
		GameDataRef _data = std::make_shared<GameData>();

		void Run();
	};
}