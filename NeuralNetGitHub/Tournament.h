#pragma once

#include "OthelloGame.h"

class Tournament {
private:
	std::vector<OthelloGame*> games;
	std::vector<int>scores;
	Brain best;
	int bestScore;
	int gameCount;
public:

	//makes argument number of games, default 10
	Tournament(int numGames = 10);

	Tournament(std::string file0, std::string file1);

	~Tournament();

	//reset tournament for re-use
	bool resetTournament();

	//play the current tournament, may be async
	Brain playTournament(bool display, bool perfectPlay,bool timeDelay = false,int milliseconds = 1000);

	//play tournament for argument number of generations
	bool playGenerations(int generations, bool perfectPlay, bool display, bool timeDelay = false, int milliseconds = 1000);

	//uses results of previous generation to create a new one
	bool seedNewGeneration();

	//save the best of this generation to a file
	bool saveBestToFile(int generation);
};