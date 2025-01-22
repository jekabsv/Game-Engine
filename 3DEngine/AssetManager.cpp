#include <iostream>
#include "AssetManager.hpp"

namespace game
{
	void AssetManager::LoadTexture(std::string name, std::string FileName)
	{
		sf::Texture texture;
		if (!texture.loadFromFile(FileName))
		{
			std::cout << "Error Loading texture: " << name << " from File: " << FileName;
		}
		_textures[name] = texture;
	}

	sf::Texture& AssetManager::GetTexture(std::string name)
	{
		return _textures[name];
	}
	void AssetManager::LoadFont(std::string name, std::string FileName)
	{
		sf::Font font;
		if (!font.loadFromFile(FileName))
		{
			std::cout << "Error Loading font: " << name << " from File: " << FileName;
		}
		_fonts[name] = font;
	}
	sf::Font& AssetManager::GetFont(std::string name)
	{
		return _fonts[name];
	}
	void AssetManager::LoadSound(std::string name, std::string FileName)
	{
		sf::SoundBuffer sound;
		if (!sound.loadFromFile(FileName))
		{
			std::cout << "Error Loading sound: " << name << " from File: " << FileName;
		}
		_sounds[name] = sound;
	}
	sf::SoundBuffer& AssetManager::GetSound(std::string name)
	{
		return _sounds[name];
	}
}