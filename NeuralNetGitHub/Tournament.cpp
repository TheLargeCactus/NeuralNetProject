#include "Tournament.h"
#include <thread>



Tournament::Tournament(int numGames)
{
	gameCount = numGames;
	for (int i = 0; i < numGames; i++) {
		games.push_back(new OthelloGame());
	}

}

Tournament::Tournament(std::string file0, std::string file1) : Tournament()
{
	games[0]->loadBrain(file0, file1);
}

Tournament::~Tournament()
{
	resetTournament();
}

bool Tournament::resetTournament()
{
	for (int i = 0; i < games.size(); i++) {
		delete games[i];
	}
	games.resize(0);

	scores.resize(0);
	return true;
}

Brain Tournament::playTournament(bool display, bool perfectPlay, bool timeDelay, int milliseconds)
{
	bestScore = INT_MIN;

	for (int i = 0; i < games.size(); i++) {
		games[i]->playGame(display, perfectPlay, timeDelay,milliseconds);
		scores.push_back(games[i]->getWinningScore());

		if (scores[i] > bestScore) {
			best = games[i]->getWinner();
			bestScore = scores[i];
		}
	}


	return best;
}

bool Tournament::playGenerations(int generations,bool perfectPlay, bool display, bool timeDelay, int milliseconds)
{
	for (int i = 0; i < generations; i++) {
		playTournament(display, perfectPlay, timeDelay,milliseconds);
		consoleOut("Played tournament: "+std::to_string(i)+ "\n");
		saveBestToFile(i);
		seedNewGeneration();
	}

	return true;
}

bool Tournament::seedNewGeneration()
{
	Brain p0, p1;

	if (!games.size()) {
		return false;
	}

	resetTournament();

	for (int i = 0; i < gameCount; i++) {
		p0 = p1 = best;
		p0.mutate(LAMBDA, PERFECT);
		p1.mutate(LAMBDA, PERFECT);
		

		games.push_back(new OthelloGame(p0, p1));
	}

	return true;
	
}

bool Tournament::saveBestToFile(int generation)
{
	std::stringstream fileName;

	fileName << ".\\Brains\\Gen" << generation << "Winner.brain";

	return best.saveToFile(fileName.str());
}
