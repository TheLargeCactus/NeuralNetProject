#include "OthelloGame.h"

int & OthelloGame::score(bool player)
{
	if (player) {
		return score1;
	}
	return score0;
}

OthelloGame::OthelloGame()
{
	turn = false;
	played = false;
	score0 = score1 = 0;
}

OthelloGame::OthelloGame(Brain & p0, Brain & p1) : OthelloGame()
{
	player0 = p0;
	player1 = p1;

}

bool OthelloGame::takeTurn(bool player, bool perfectPlay)
{
	std::vector<int> moves;

	if (perfectPlay) {
		BitStore b = gameBoard.getValidMoves(player);

		for (int i = 0; i < b.size(); i++) {
			if (b.at(i) == true) {
				moves.push_back(i);
			}
		}

		if (!moves.size()) {
			return false;
		}

		if (player) {
			gameBoard.gamerulePlacePiece(moves[player1.answer(gameBoard, player) % moves.size()], player);
			return true;
		}

		else {
			gameBoard.gamerulePlacePiece(moves[player0.answer(gameBoard, player) % moves.size()], player);
			return true;
		}
		
	}

	if (player) {
		if (gameBoard.gamerulePlacePiece(player1.answer(gameBoard, player), player))
		{
			return true;
		}

	}
	else {
		if (gameBoard.gamerulePlacePiece(player0.answer(gameBoard, player), player))
		{
			return true;
		}
	}

	errorOut("OthelloGame",std::string("Player ") + std::string((player)? "1": "0") + " was unable to take turn", false);
	return false;
}

bool OthelloGame::loadBrain(std::string filename, std::string filename2)
{
	std::string input;
	std::ifstream file(filename);
	if (!file) {
		errorOut("OthelloGame", "Failed to open file for brain loading", true);
	}

	std::getline(file, input, '\n');
	file.close();
	player0.setChromosome(input);

	if (filename2.size()) {
		file.open(filename2);

		if (!file) {
			errorOut("OthelloGame", "Failed to open file for brain loading", true);
		}
		std::getline(file, input, '\n');
		file.close();
		player1.setChromosome(input);

	}

	return true;
}

bool OthelloGame::playGame(bool display, bool perfectPlay, bool timeDelay, int milliseconds)
{
	bool continueGame = true;
	int turnCount = 0;

	while (continueGame) {

		if (timeDelay) {
			std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
		}

		clearScreen();

		if (display) {
			displayBoard();
			consoleOut("Turn" + std::to_string(++turnCount)+ "\n");
		}

		if (takeTurn(turn,perfectPlay)) {
			turn = !turn;
			score(turn) += 1; //if a player can take their turn, add 1 to their score
		}
		else if(takeTurn(!turn, perfectPlay)) {
			score(turn) -= 1; // if the other player can take their turn, this player loses 1 point
		}
		else {
			continueGame = false;
		}
	}

	played = true;

	//return 0 if player 0 won, 1 if player 1 won
	if (pieceCount(false) > pieceCount(true)) {
		winner = false;
	}
	else {
		winner = true;
	}

	score(winner) += 25;
	return winner;
}

bool OthelloGame::playGameGUI(sf::RenderWindow & window, bool perfectPlay, bool timeDelay, int milliseconds)
{
	int turnCount = 0;
	sf::Event event;
	std::chrono::time_point<std::chrono::system_clock> dur = std::chrono::system_clock::now();

	while (window.isOpen()) {
		while (window.pollEvent(event))
		{
			switch (event.type) {
			case sf::Event::Closed: //Close Window
				window.close();
				break;
			}
		}
		
		window.clear();

		displayBoardGUI(window);
		

		window.display();

		if (!timeDelay || dur + std::chrono::milliseconds(milliseconds) < std::chrono::system_clock::now()) {
			
			if (takeTurn(turn, perfectPlay)) {
				turn = !turn;
				turnCount++;
				score(turn) += 1; //if a player can take their turn, add 1 to their score
			}
			else if (takeTurn(!turn, perfectPlay)) {
				turnCount++;
				score(turn) -= 1; // if the other player can take their turn, this player loses 1 point
			}
			else {
				window.close();
			}
			consoleOut("Turn" + std::to_string(turnCount) + "\n");
			dur = std::chrono::system_clock::now();
		}
	}

	played = true;

	//return 0 if player 0 won, 1 if player 1 won
	if (pieceCount(false) > pieceCount(true)) {
		winner = false;
	}
	else {
		winner = true;
	}

	consoleOut("Player " + std::to_string(winner) + " is the winner!\n");
	score(winner) += 25;
	return winner;
}

bool OthelloGame::playGame1HumanGUI(sf::RenderWindow & window, bool perfectPlay, bool timeDelay, int milliseconds)
{
	bool AITurn = false;
	sf::Event event;
	sf::Vector2i mousePos;
	int x, y;
	std::chrono::time_point<std::chrono::system_clock> dur;

	while (window.isOpen()) {
		while (window.pollEvent(event))
		{
			switch (event.type) {
			case sf::Event::Closed: //Close Window
				window.close();
				break;
			case sf::Event::MouseButtonPressed:
				if (!AITurn && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					mousePos = sf::Mouse::getPosition(window);
					x = mousePos.x / TILESIZE;
					y = mousePos.y / TILESIZE;
					if (gameBoard.gamerulePlacePiece(gameBoard.getLoc(x, y), 0)) {
						AITurn = true;
						dur = std::chrono::system_clock::now();
					}
				}
			}
		}

		if (!AITurn && gameBoard.getValidMoves(0) == BitStore(0)) {
			AITurn = true;
			dur = std::chrono::system_clock::now();
		}

		window.clear();

		gameBoard.displayBoardGUI(window);
		gameBoard.highlightValidMoves(window, AITurn);

		window.display();
		
		if (AITurn && (!timeDelay || dur + std::chrono::milliseconds(milliseconds) < std::chrono::system_clock::now()))
		{
			if (takeTurn(1, perfectPlay)) {
				AITurn = false;
			}
			else if (gameBoard.getValidMoves(0) == BitStore(0)) {
				window.close();
			}
		}
		
	}

	played = true;

	//return 0 if player 0 won, 1 if player 1 won
	if (pieceCount(false) > pieceCount(true)) {
		winner = false;
	}
	else {
		winner = true;
	}

	consoleOut("Player " + std::to_string(winner) + " is the winner!\n");
	score(winner) += 25;
	return winner;

	
}

Brain OthelloGame::getWinner()
{
	if (!played) {
		errorOut("OthelloGame", "Something tried to getWinner() when game hasn't been played.", true);
	}
	if (!winner) {
		return player0;
	}
	return player1;
}

int OthelloGame::getWinningScore()
{
	return score(winner);
}

Brain OthelloGame::getLoser()
{
	if (winner) {
		return player0;
	}

	return player1;
}

Board OthelloGame::getBoard()
{
	return gameBoard;
}

int OthelloGame::getScore(bool player)
{
	if (player) {
		return score1;
	}
	return score0;
}

int OthelloGame::pieceCount(bool player)
{
	return gameBoard.pieceCount(player);
}

void OthelloGame::displayBoard() const
{
	gameBoard.displayBoard();
	consoleOut("\n");
}

void OthelloGame::displayBoardGUI(sf::RenderWindow & window) const
{
	gameBoard.displayBoardGUI(window);
}
