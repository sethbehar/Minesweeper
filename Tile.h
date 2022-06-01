#include <SFML/Graphics.hpp>
#include "TextureManager.h"
using namespace std;


class Tile {

private:
	int row;
	int column;
	bool hasMine;
	bool isHidden;
	bool isFlagged;
	int nearbyMines;
	int tilesClicked;

	sf::Sprite tileHidden;
	sf::Sprite tileRevealed;
	sf::Sprite mine;
	sf::Sprite flag;

public:

	Tile();
	vector<Tile*> neighbors;
	vector<sf::Sprite> numbers;

	bool getIsFlagged();
	bool getIsHidden();
	bool getHasMine();
	int getNearbyMines(); 
	int getRow();
	int getColumn();

	sf::Sprite getTile();
	sf::Sprite getMine();
	sf::Sprite getFlag();
	sf::Sprite getTileRevealed();
	sf::FloatRect getPosition();
	vector<Tile*> getNeighbors();

	void setNeighbors(vector<Tile*> neighbors);
	void setMines(int mines);
	void setColumnRow(int r, int c);
	void revealTile();
	void unRevealTile();
	void revealFlag();
	void unRevealFlag();
	void createMine();
	void setPosition(float x, float y);
	
};