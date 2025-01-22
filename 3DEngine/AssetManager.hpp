#pragma once

#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "DEFINITIONS.hpp"


namespace game
{
	class AssetManager
	{
	public:
		AssetManager() {};
		~AssetManager() {};
		void LoadTexture(std::string name, std::string FileName);
		sf::Texture& GetTexture(std::string name);
		//void UnloadTexture();
		void LoadFont(std::string name, std::string FileName);
		sf::Font& GetFont(std::string name);
		//void UnloadFont();
		void LoadSound(std::string name, std::string FileName);
		sf::SoundBuffer& GetSound(std::string name);
		//void UnloadSound();
	private:
		std::map <std::string, sf::Texture> _textures;
		std::map <std::string, sf::Font> _fonts;
		std::map <std::string, sf::SoundBuffer> _sounds;
	};
}