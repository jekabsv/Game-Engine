#pragma once
#include <SFML/Graphics.hpp>


namespace game
{
	class InputManager
	{
	public:
		bool IsSpriteClicked(sf::Sprite object, sf::Mouse::Button button, sf::RenderWindow& window);
		bool IsSpriteHovered(sf::Sprite object, sf::RenderWindow& window);
		sf::Vector2i GetMousePosition(sf::RenderWindow& window);
	};
}