#include "Tournament.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

//Version 0.5.0

int main()
{
	gen = std::default_random_engine(time(NULL));

	sf::RenderWindow window;
	window.setFramerateLimit(24);
	OthelloGame gam;
	Tournament * t;
	int choice;
	bool loop = true;
	std::string filename;
	std::string filename2;

	
	

	
	while (loop) {
		clearScreen();

		consoleOut("Choices:\n");
		consoleOut("1. Watch Console Game\n");
		consoleOut("2. Watch GUI AI Match\n");
		consoleOut("3. Play Game vs AI (GUI)\n");
		consoleOut("4. Play Game vs specific AI(GUI)\n");
		consoleOut("5. Play Tournament with base AI\n");
		consoleOut("6. Play Tournament with loaded AI starting brains\n");
		consoleOut("7. Quit\n");

		choice = consoleIn<int>("Enter a selection:");
		clearScreen();

		switch (choice) {
		case 1:
			gam.playGame(true, true, true, 500);
			break;
		case 2:
			window.create(sf::VideoMode(TILESIZE * BOARDSIZE, TILESIZE * BOARDSIZE), "AI Othello");
			gam.playGameGUI(window, true, true, 250);
			break;
		case 3:
			window.create(sf::VideoMode(TILESIZE * BOARDSIZE, TILESIZE * BOARDSIZE), "AI Othello");
			gam.playGame1HumanGUI(window, true, true, 1500);
			break;
		case 4:
			window.create(sf::VideoMode(TILESIZE * BOARDSIZE, TILESIZE * BOARDSIZE), "AI Othello");
			filename = consoleIn<std::string>("Enter file location: ");
			gam.loadBrain(filename,filename);
			consoleOut("Brain loaded");
			gam.playGame1HumanGUI(window, true, true, 1500);
			break;
		case 5:
			t = new Tournament;
			t->playGenerations(100, true, false);
			break;
		case 6:
			filename = consoleIn<std::string>("Enter file location for brain0: ");
			filename2 = consoleIn<std::string>("Enter file location for brain1: ");
			t = new Tournament(filename,filename2);
			t->playGenerations(100, true, false);
			break;
		case 7:
			loop = false;
			break;
		}

		system("pause");
	}
	return 0;
}