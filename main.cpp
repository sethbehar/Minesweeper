#include <SFML/Graphics.hpp>
#include "Board.h"

using namespace std;

int main() {
	Board board;
	sf::RenderWindow window(sf::VideoMode(board.getWidth(), board.getHeight()), "Minesweeper");

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
		board.drawBoard(window);
		board.drawBoardSprites(window);

		if (event.type == sf::Event::MouseButtonPressed) {
			// Check which tile is clicked...
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				board.checkTileClicked(window);
				board.checkTestClicked(window);
				continue;
			}
		}

		if (event.type == sf::Event::MouseButtonPressed) {
			// Check which tile we are flagging...
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
				board.updateMineCounter(window);
				board.flagATile(window);
				continue;
			}
		}



		window.display();
	}

	
}