#include "TextureManager.h"
#include <string>

using namespace std;

unordered_map<string, sf::Texture> TextureManager::textures;

sf::Texture& TextureManager::getTexture(string name) {
	if (textures.find(name) == textures.end()) {
		LoadTexture(name);
	}
	return textures[name];
}

void TextureManager::clear() {
	textures.clear();
}

void TextureManager::LoadTexture(string name) {
	string path = "images/";
	path += name + ".png";

	textures[name].loadFromFile(path);
}