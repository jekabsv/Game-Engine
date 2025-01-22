#include "InputManager.hpp"
#include <iostream>

namespace game
{
	bool InputManager::IsSpriteClicked(sf::Sprite object, sf::Mouse::Button button, sf::RenderWindow& window)
	{
		return sf::Mouse::isButtonPressed(button) && object.getGlobalBounds().contains(sf::Vector2f(GetMousePosition(window).x, GetMousePosition(window).y));
	}
	bool InputManager::IsSpriteHovered(sf::Sprite object, sf::RenderWindow& window)
	{
		return object.getGlobalBounds().contains(sf::Vector2f(GetMousePosition(window).x, GetMousePosition(window).y));
	}
	sf::Vector2i InputManager::GetMousePosition(sf::RenderWindow& window)
	{
		return(sf::Mouse::getPosition(window));
	}
}