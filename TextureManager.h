#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <unordered_map>

using namespace std;

class TextureManager {
private:
	static unordered_map<string, sf::Texture> textures;
	static void LoadTexture(string name);

public:
	static sf::Texture& getTexture(string name);
	static void clear();

};