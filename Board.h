#include <iostream>
#include "Tile.h"
#include <SFML/Graphics.hpp>
using namespace std;

class Board {
private:
	// Will get from "config.cfg" file...
	int columns;
	int rows; 
	int mines; 
	int totalFlagged;
	int testMines;
	int tilesRevealed;
	int totalTilesWithoutMines;

	int width;
	int height;

	bool gameWon;
	bool gameLost;
	bool isDebug;
	bool isTestBoard;
	bool isNegativeMines;

	// Board sprites
	sf::Sprite smiley;
	sf::Sprite mineDigits;
	sf::Sprite faceWin;
	sf::Sprite faceLose;
	sf::Sprite debug;
	sf::Sprite test1;
	sf::Sprite test2;
	sf::Sprite test3;

	sf::Sprite firstDigitSprite;
	sf::Sprite secondDigitSprite;
	sf::Sprite thirdDigitSprite;
	sf::Sprite negative;

	

public:
	vector< vector<Tile> > tiles;
	vector< vector<int> > intTiles;
	vector<sf::Sprite> digits;

	// Constructor
	Board();

	// Getter functions
	int getColumns();
	int getRows();
	int getMines();
	int getWidth();
	int getHeight();

	// Game functionality
	bool getGameWon();
	bool getGameLost();
	bool isValidBoardPos(int row, int col);
	void setDimensions();
	void drawBoardSprites(sf::RenderWindow& window);
	void drawBoard(sf::RenderWindow& window);
	void checkTileClicked(sf::RenderWindow& window);
	void checkTestClicked(sf::RenderWindow& window);
	void flagATile(sf::RenderWindow& window);
	void addEachNeighbor();
	void createRandomIntBoard();
	void createBoardBasedOffInts();
	void readInTestBoard(int n);
	void resetBoard();
	void countSurroundingMines();
	void updateMineCounter(sf::RenderWindow& window);
	void recursiveClick(Tile* tile);
	void endGame(sf::RenderWindow & window);

};