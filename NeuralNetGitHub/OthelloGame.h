#pragma once
#include "Brain.h"
#include "Board.h"

class OthelloGame {
private:
	Brain player0, player1;
	int score0, score1;
	Board gameBoard;
	bool turn;
	bool winner;
	bool played;

	int & score(bool player);
public:
	//default constructor
	OthelloGame();

	//call constructor, uses 2 existing Brains as player
	OthelloGame(Brain & p0, Brain & p1);

	//returns true if game can continue, false if game ended
	bool takeTurn(bool player,bool perfectPlay);

	bool loadBrain(std::string filename,std::string filename2 = "");

	//play game, returns false if player 0 won, true if player 1 won
	//updates score0 and score1
	bool playGame(bool display, bool perfectPlay,bool timeDelay, int milliseconds);

	bool playGameGUI(sf::RenderWindow & window, bool perfectPlay, bool timeDelay, int milliseconds);

	bool playGame1HumanGUI(sf::RenderWindow & window, bool perfectPlay, bool timeDelay, int milliseconds);

	//returns the winning brain
	Brain getWinner();
	
	//get winner's score
	int getWinningScore();

	//returns losing brain
	Brain getLoser();

	//returns the current board state
	Board getBoard();

	//gets the score of the player
	int getScore(bool player);

	//count number of pieces for player
	int pieceCount(bool player);

	//display the board
	void displayBoard() const;

	void displayBoardGUI(sf::RenderWindow & window) const;
};