#include <iostream>
#include "AssetManager.hpp"
#include <fstream>
#include <sstream>

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




    void AssetManager::LoadTextureMTL(std::string name, std::string FileName)
    {
        

        std::ifstream mtlFile(FileName);
        if (!mtlFile.is_open())
        {
            std::cout << "Error opening MTL file: " << FileName << std::endl;
            return;
        }

        std::string line;
        std::string currentMaterial;
        sf::Texture texture;
        while (std::getline(mtlFile, line))
        {
            std::stringstream ss(line);
            std::string token;
            ss >> token;

            if (token == "newmtl")
            {
                ss >> currentMaterial;  // Get material name
                
            }
            else if (token == "map_Kd")
            {
                
                // If we find the texture definition, load the texture from a file
                std::string textureFile;
                ss >> textureFile;

                if (currentMaterial == name)
                {
                    if (!texture.loadFromFile(textureFile))
                    {
                        std::cout << "Error loading texture for material: " << currentMaterial << " from file: " << textureFile << std::endl;
                    }
                    else
                    {
                        std::cout << "Texture loaded successfully from file: " << textureFile << std::endl;
                        _textures[name] = texture;  // Store texture with the given name
                    }
                }
            }
            else if (token == "Kd")
            {
                
                // If we find a color definition, create a solid color texture
                if (currentMaterial == name)
                {
                    float r, g, b;
                    ss >> r >> g >> b;
                    
                    // Ensure values are within the range [0, 1] and create a 1x1 texture
                    r = std::min(1.0f, std::max(0.0f, r));  // Clamping to the range [0, 1]
                    g = std::min(1.0f, std::max(0.0f, g));
                    b = std::min(1.0f, std::max(0.0f, b));

                    // Create a 1x1 image filled with the color
                    sf::Image image;
                    try {
                        image.create(1, 1, sf::Color(r * 255, g * 255, b * 255));  // Multiply by 255 to get the correct color range

                        // Debugging: Print the image size
                        sf::Vector2u imageSize = image.getSize();
                        std::cout << "Image created with size: " << imageSize.x << "x" << imageSize.y << std::endl;

                    }
                    catch (const std::exception& e) {
                        std::cout << "Error creating image for color material: " << currentMaterial << "\nException: " << e.what() << std::endl;
                        continue;  // Skip further processing if the image creation fails
                    }

                    // Load the image into the texture
                    if (!texture.loadFromImage(image))
                    {
                        std::cout << "Error loading texture from image for material: " << currentMaterial << std::endl;
                    }
                    else
                    {
                        std::cout << "Color-based texture loaded successfully for material: " << currentMaterial << std::endl;
                        _textures[name] = texture;  // Store the generated texture with the given name

                        // Debugging: Print the texture size
                        sf::Vector2u textureSize = texture.getSize();
                        std::cout << "Texture size after loading: " << textureSize.x << "x" << textureSize.y << std::endl;
                    }
                }
            }
        }
    }




}