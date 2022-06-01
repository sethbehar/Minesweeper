#include "Board.h"
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <random>

using namespace std;

Board::Board() {

	columns = 0;
	rows = 0;
	mines = 0;

	gameWon = false;
	gameLost = false;
	isDebug = false;
	isTestBoard = false;
	isNegativeMines = false;

	setDimensions();
	
	// Add tiles to board and calculate mines near each tile...
	createRandomIntBoard();

	/*for (int i = 0; i < intTiles.size(); i++) {
		for (int k = 0; k < intTiles[0].size(); k++) {
			cout << intTiles[i][k] << " ";
		}
		cout << endl;
	}*/


	createBoardBasedOffInts();

	addEachNeighbor();
	countSurroundingMines();

	// Window dimensions...
	width = columns * 32;
	height = (rows * 32) + 100;

	// Set textures...
	smiley.setTexture(TextureManager::getTexture("face_happy"));
	faceWin.setTexture(TextureManager::getTexture("face_win"));
	faceLose.setTexture(TextureManager::getTexture("face_lose"));
	debug.setTexture(TextureManager::getTexture("debug"));
	test1.setTexture(TextureManager::getTexture("test_1"));
	test2.setTexture(TextureManager::getTexture("test_2"));
	test3.setTexture(TextureManager::getTexture("test_3"));
	mineDigits.setTexture(TextureManager::getTexture("digits"));

	// Set game sprites...
	smiley.setPosition(width / 2.0 - 50, rows * 32);
	faceWin.setPosition(width / 2.0 - 50, rows * 32);
	faceLose.setPosition(width / 2.0 - 50, rows * 32);
	debug.setPosition((width / 2.0) + 100, rows * 32);
	test1.setPosition((width / 2.0) + 165, rows * 32);
	test2.setPosition((width / 2.0) + 225, rows * 32);
	test3.setPosition((width / 2.0) + 285, rows * 32);

	firstDigitSprite.setPosition(21, height - 100);
	secondDigitSprite.setPosition(42, height - 100);
	thirdDigitSprite.setPosition(63, height - 100);

	totalTilesWithoutMines = (rows * columns) - mines;
}

int Board::getColumns() {
	return columns;
}

int Board::getRows() {
	return rows;
}

int Board::getMines() {
	return mines;
}

int Board::getWidth() {
	return width;
}

int Board::getHeight() {
	return height;
}

bool Board::getGameWon() {
	return gameWon;
}

bool Board::getGameLost() {
	return gameLost;
}

bool Board::isValidBoardPos(int row, int col) {
	return (row >= 0 && row < rows && col >= 0 && col < columns);
}

void Board::drawBoardSprites(sf::RenderWindow& window) {

	if (!gameWon && !gameLost)
		window.draw(smiley);
	else if (gameLost)
		window.draw(faceLose);
	else
		window.draw(faceWin);

	window.draw(test1);
	window.draw(test2);
	window.draw(test3);
	window.draw(debug);
	updateMineCounter(window);
}


void Board::drawBoard(sf::RenderWindow& window) {
	for (int i = 0; i < rows; i++) {
		for (int k = 0; k < columns; k++) {

			if (gameLost) {
				for (int i = 0; i < rows; i++) 
					for (int k = 0; k < columns; k++) 
						if (tiles[i][k].getHasMine())
							window.draw(tiles[i][k].getMine());
			}

			if (gameWon) {
				for (int i = 0; i < rows; i++)
					for (int k = 0; k < columns; k++)
						if (tiles[i][k].getHasMine() && !tiles[i][k].getIsFlagged())
							window.draw(tiles[i][k].getFlag());
			}

			tiles[i][k].setPosition(k * 32, i * 32);
			tiles[i][k].setColumnRow(k, i);

			if (!tiles[i][k].getIsHidden() && (tiles[i][k].getNearbyMines() > 0 || tiles[i][k].getHasMine()))
				window.draw(tiles[i][k].getTileRevealed());

			window.draw(tiles[i][k].getTile());

			if (isDebug)
				if (tiles[i][k].getHasMine())
					window.draw(tiles[i][k].getMine());

		}
	}
}

void Board::setDimensions() {
	ifstream file("boards/config.cfg");
	string tempNumber;

	int tempColumns;
	int tempRows;
	int tempMines;

	if (file.is_open()) {
		getline(file, tempNumber);
		tempColumns = stoi(tempNumber);

		getline(file, tempNumber);
		tempRows = stoi(tempNumber);

		getline(file, tempNumber);
		tempMines = stoi(tempNumber);

		columns = tempColumns;
		rows = tempRows;
		mines = tempMines;
	}
	else {
		cout << "file not opened...";
	}
}
	
void Board::checkTileClicked(sf::RenderWindow& window) {
	if (!gameLost || gameWon) {
		auto mousePos = sf::Mouse::getPosition(window);
		auto actualMousePos = window.mapPixelToCoords(mousePos);
		for (int i = 0; i < rows; i++) {
			for (int k = 0; k < columns; k++) {
				if (tiles[i][k].getPosition().contains(actualMousePos)) {
					/*if (tiles[i][k].getIsHidden()) {
						tiles[i][k].revealTile();
					}*/

					if (gameWon) {
						break;
					}

					if (tiles[i][k].getHasMine()) {
						gameLost = true;
						endGame(window);
					}

					recursiveClick(&tiles[i][k]);

				}
			}
		}
	}
}

void Board::checkTestClicked(sf::RenderWindow& window) {
	auto mousePos = sf::Mouse::getPosition(window);
	auto actualMousePos = window.mapPixelToCoords(mousePos);

	if (test1.getGlobalBounds().contains(actualMousePos)) {
		gameLost = false;
		gameWon = false;
		isNegativeMines = false;
		readInTestBoard(1);
		negative.setPosition(width * height * 2, width * height * 2);
		isTestBoard = true;
		testMines = 1;
		updateMineCounter(window);
	}
	else if(test2.getGlobalBounds().contains(actualMousePos)) {
		gameLost = false;
		gameWon = false;
		isNegativeMines = false;
		negative.setPosition(width * height * 2, width * height * 2);
		readInTestBoard(2);
		isTestBoard = true;
		updateMineCounter(window);
	}

	else if(test3.getGlobalBounds().contains(actualMousePos)) {
		gameLost = false;
		gameWon = false;
		isNegativeMines = false;
		negative.setPosition(width * height * 2, width * height * 2);
		readInTestBoard(3);
		isTestBoard = true;
		updateMineCounter(window);
	}
	else if(smiley.getGlobalBounds().contains(actualMousePos)) {
		gameLost = false;
		gameWon = false;
		isNegativeMines = false;
		negative.setPosition(width * height * 2, width * height * 2);
		isTestBoard = false;
		resetBoard();
		updateMineCounter(window);
	}
	else if (debug.getGlobalBounds().contains(actualMousePos)) {
		gameLost = false;
		gameWon = false;
		isDebug = !isDebug;
	}

}

void Board::flagATile(sf::RenderWindow& window) {
	auto mousePos = sf::Mouse::getPosition(window);
	auto actualMousePos = window.mapPixelToCoords(mousePos);

	if (!gameWon && !gameLost && !isDebug) {
		for (int i = 0; i < rows; i++) {
			for (int k = 0; k < columns; k++) {
				if (tiles[i][k].getPosition().contains(actualMousePos)) {
					// Toggle flag...
					if (!tiles[i][k].getIsFlagged()) {
						tiles[i][k].revealFlag();
						totalFlagged++;
					}
					else {
						tiles[i][k].unRevealFlag();
						totalFlagged--;
					}

				}
			}
		}
	}
}

void Board::addEachNeighbor() {
	int neighbors;

	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < columns; c++) {
			vector<Tile*> nbs;

			// top left
			if (isValidBoardPos(r - 1, c + 1)) {
				Tile* temp(&tiles[r - 1][c + 1]);
				nbs.push_back(temp);
			}
			else {
				nbs.push_back(nullptr);
			}

			// top right
			if (isValidBoardPos(r + 1, c + 1)) {
				Tile* temp(&tiles[r + 1][c + 1]);
				nbs.push_back(temp);
			}
			else {
				nbs.push_back(nullptr);
			}

			// top middle
			if (isValidBoardPos(r, c + 1)) {
				Tile* temp(&tiles[r][c + 1]);
				nbs.push_back(temp);
			}
			else {
				nbs.push_back(nullptr);
			}

			// bottom left
			if (isValidBoardPos(r - 1, c - 1)) {
				Tile* temp(&tiles[r - 1][c - 1]);
				nbs.push_back(temp);
			}
			else {
				nbs.push_back(nullptr);
			}

			// bottom right
			if (isValidBoardPos(r + 1, c - 1)) {
				Tile* temp(&tiles[r + 1][c - 1]);
				nbs.push_back(temp);
			}
			else {
				nbs.push_back(nullptr);
			}

			// bottom middle
			if (isValidBoardPos(r, c - 1)) {
				Tile* temp(&tiles[r][c - 1]);
				nbs.push_back(temp);
			}
			else {
				nbs.push_back(nullptr);
			}

			// middle left
			if (isValidBoardPos(r - 1, c)) {
				Tile* temp(&tiles[r - 1][c]);
				nbs.push_back(temp);
			}
			else {
				nbs.push_back(nullptr);
			}

			// middle right
			if (isValidBoardPos(r + 1, c)) {
				Tile* temp(&tiles[r + 1][c]);
				nbs.push_back(temp);
			}
			else {
				nbs.push_back(nullptr);
			}

			tiles[r][c].setNeighbors(nbs);
		}
	}
}

void Board::createRandomIntBoard() {
	
	// not in test board
	isTestBoard = false;

	intTiles.clear();
	vector<int> vals(columns * rows);
	generate(vals.begin(), vals.end(), [val = 0]() mutable { return val++; });
	shuffle(vals.begin(), vals.end(), mt19937{ random_device{}() });
	vals.resize(mines);

	// create 2d int vector based off random vals...
	int c = 0;
	for (int i = 0; i < rows; i++) {
		vector<int> tempArr;
		for (int k = 0; k < columns; k++) {
			if (find(vals.begin(), vals.end(), c) != vals.end()) {
				tempArr.push_back(1);
			}
			else {
				tempArr.push_back(0);
			}
			c++;
		}
		intTiles.push_back(tempArr);
		tempArr.clear();
	}

	totalTilesWithoutMines = (columns * rows) - mines;

}

void Board::createBoardBasedOffInts() {
	tiles.clear();
	for (int i = 0; i < rows; i++) {
		vector<Tile> tempTiles;
		for (int k = 0; k < columns; k++) {
			Tile t;
			if (intTiles[i][k] == 1) {
				t.createMine();
			}
			tempTiles.push_back(t);
		}
		tiles.push_back(tempTiles);
		tempTiles.clear();
	}
	
	gameLost = false;
	gameWon = false;
	isNegativeMines = false;
	tilesRevealed = 0;
	totalFlagged = 0;
	addEachNeighbor();
	countSurroundingMines();
}

void Board::readInTestBoard(int n) {
	intTiles.clear();
	string filename = "boards/testboard" + to_string(n) + ".brd";

	fstream file(filename);
	string lineFromFile;

	int temp = 0;

	if (file.is_open()) {
		while (getline(file, lineFromFile)) {
			vector<int> tempInts;
			for (int i = 0; i < lineFromFile.size(); i++) {
				tempInts.push_back(((int)lineFromFile[i]) - 48);

				if (((int)lineFromFile[i]) - 48 == 1) {
					temp++; // increase testMine counter
				}

			}

			intTiles.push_back(tempInts);
			tempInts.clear();
		}

		columns = intTiles[0].size();
		rows = intTiles.size();
	}
	else {
		cout << "File " << filename << " could not be opened..." << endl;
	}

	tilesRevealed = 0;

	if (n != 1)
		testMines = temp;
	else
		testMines = 1;

	if (isTestBoard)
		totalTilesWithoutMines = (rows * columns) - testMines;
	
	createBoardBasedOffInts();
	file.close();
}

void Board::resetBoard() {
	createRandomIntBoard();
	createBoardBasedOffInts();
}

void Board::countSurroundingMines() {
	int count = 0;
	for (int i = 0; i < rows; i++) {
		vector<Tile*> nbs;
		for (int k = 0; k < columns; k++) {
			nbs = tiles[i][k].getNeighbors();
			count = 0;

			for (int j = 0; j < 8; j++) 
				if (nbs.at(j) != nullptr && nbs.at(j)->getHasMine())
					count++;

			tiles[i][k].setMines(count);
		}
	}
}

void Board::updateMineCounter(sf::RenderWindow& window) {

	int minesLeft = 0;

	if (gameWon) {
		minesLeft = 0;
	}
	else if (isTestBoard) {
		minesLeft = testMines - totalFlagged;
	}
	else {
		minesLeft = mines - totalFlagged;
	}
	
	int ones = 0, tens = 0, hundreds = 0;

	firstDigitSprite.setTexture(TextureManager::getTexture("digits"));
	secondDigitSprite.setTexture(TextureManager::getTexture("digits"));
	thirdDigitSprite.setTexture(TextureManager::getTexture("digits"));
	negative.setTexture(TextureManager::getTexture("digits"));

	if (minesLeft < 0) {
		minesLeft *= -1;
		isNegativeMines = true;
	}
	else {
		isNegativeMines = false;
	}

	if (minesLeft >= 100) {
		ones = minesLeft % 10;
		minesLeft = minesLeft / 10;

		tens = minesLeft % 10;
		minesLeft = minesLeft / 10;

		hundreds = minesLeft % 10;
	}
	else if (minesLeft >= 10 && minesLeft <= 99) {
		ones = minesLeft % 10;
		minesLeft = minesLeft / 10;

		tens = minesLeft;
	}
	else if (minesLeft < 10 && minesLeft >= 0) {
		ones = minesLeft;
	}

	firstDigitSprite.setTextureRect(sf::IntRect(21 * hundreds, 0, 21, 32));
	secondDigitSprite.setTextureRect(sf::IntRect(21 * tens, 0, 21, 32));
	thirdDigitSprite.setTextureRect(sf::IntRect(21 * ones, 0, 21, 32));
	negative.setTextureRect(sf::IntRect(210, 0, 21, 32));

	window.draw(firstDigitSprite);
	window.draw(secondDigitSprite);
	window.draw(thirdDigitSprite);
	
	if (isNegativeMines) {
		negative.setPosition(0, height - 100);
		window.draw(negative);
	}
	else {
		negative.setPosition(-100, -100);
		window.draw(negative);
	}
	
}

void Board::recursiveClick(Tile* t) {
	cout << "Tiles without mines: " << totalTilesWithoutMines << endl;
	cout << "Tiles revealed: " << tilesRevealed << endl;

	if (isTestBoard && totalTilesWithoutMines == tilesRevealed) {
		gameWon = true;
		cout << "Game Won!" << endl;
		return;
	}

	if (totalTilesWithoutMines == tilesRevealed) {
		gameWon = true;
	}

	if (t == nullptr) {
		return;
	}
	
	if (!t->getIsHidden() || t->getIsFlagged()) {
		return;
	}

	if (t->getNearbyMines() == 0) {
		t->revealTile();
		tilesRevealed++;

		vector<Tile*> nbs = t->getNeighbors();

		for (int i = 0; i < nbs.size(); i++)
			recursiveClick(nbs[i]);
		
	}
	else {
		t->revealTile();
		tilesRevealed++;
	}

}

void Board::endGame(sf::RenderWindow& window) {
	if (gameLost) {
		window.draw(faceLose);
		return;
	}
	if (gameWon) {
		window.draw(faceWin);
		return;
	}
}