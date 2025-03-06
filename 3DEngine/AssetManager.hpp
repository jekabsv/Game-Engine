#pragma once

#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "DEFINITIONS.hpp"
#include "Tools.hpp"

namespace game
{
	class AssetManager
	{
	public:
		std::map <std::string, sf::Texture> _textures;
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
		void LoadTextureMTL(std::string name, std::string FileName);

		void LoadObj(std::string ObjPath, std::string MtlPath, std::string name);
		mesh GetObj(std::string name);
		

	private:
		std::map <std::string, mesh> _meshes;
		std::map <std::string, sf::Font> _fonts;
		std::map <std::string, sf::SoundBuffer> _sounds;
	};
}