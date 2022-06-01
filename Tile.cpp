#include "Tile.h"
#include "TextureManager.h"
#include <SFML/Graphics.hpp>
#include <iostream>

Tile::Tile() {

	isHidden = true;
	hasMine = false;
	isFlagged = false;
	hasMine = false;

	tileHidden.setTexture(TextureManager::getTexture("tile_hidden"));
	tileRevealed.setTexture(TextureManager::getTexture("tile_revealed"));

	flag.setTexture(TextureManager::getTexture("flag"));
	mine.setTexture(TextureManager::getTexture("mine"));

	for (int i = 1; i <= 8; i++) {
		string numFile = "number_" + to_string(i);
		sf::Sprite num(TextureManager::getTexture(numFile));
		numbers.push_back(num);
	}
	
}

bool Tile::getIsFlagged() {
	return isFlagged;
}

bool Tile::getIsHidden() {
	return isHidden;
}

bool Tile::getHasMine() {
	return hasMine;
}

int Tile::getNearbyMines() {
	return nearbyMines;
}

int Tile::getRow() {
	return row;
}

int Tile::getColumn() {
	return column;
}

sf::Sprite Tile::getTile() {


	// If tile has no mine reveal number...
	if (!isHidden && !hasMine && nearbyMines > 0) {
		return numbers[nearbyMines - 1];
	}

	// If tile has a mine...
	if (!isHidden && hasMine) {
		return mine;
	}

	// If tile was flagged...
	if (isHidden && isFlagged) {
		return flag;
	}

	// Else just a normal tile lol...
	return isHidden ? tileHidden : tileRevealed;
	

}

sf::Sprite Tile::getMine() {
	return mine;
}

sf::Sprite Tile::getFlag() {
	return flag;
}

sf::Sprite Tile::getTileRevealed() {
	return tileRevealed;
}

sf::FloatRect Tile::getPosition() {
	return tileHidden.getGlobalBounds();
}

vector<Tile*> Tile::getNeighbors() {
	return neighbors;
}

void Tile::setNeighbors(vector<Tile*> neighbors) {
	this->neighbors = neighbors;
}

void Tile::setMines(int mines) {
	nearbyMines = mines;
}

void Tile::setColumnRow(int r, int c) {
	row = r;
	column = c;
}

void Tile::revealTile() {
	isHidden = !isHidden;
}

void Tile::unRevealTile() {
	isHidden = true;
}

void Tile::revealFlag() {
	isFlagged = true;
}

void Tile::unRevealFlag() {
	isFlagged = false;
}

void Tile::createMine() {
	hasMine = true;
}

void Tile::setPosition(float x, float y) {
	tileHidden.setPosition(x, y);
	tileRevealed.setPosition(x, y);
	flag.setPosition(x, y);
	mine.setPosition(x, y);

	for (int i = 0; i < 8; i++) {
		numbers[i].setPosition(x, y);
	}
}