#include "Lib.h"

const float TILESIZE = 50;
const float PIECESIZE = 50;


const uint64_t BOARDSIZE = 8;
const int LAMBDA = 20;
const bool PERFECT = true;

const std::pair<int, int> DIAGONAL_DIRECTIONS[4] = { { 1,1 },{ 1, -1 },{ -1,1 },{ -1,-1 } };
const std::pair<int, int> CARDINAL_DIRECTIONS[4] = { { 1,0 },{ 0,1 },{ -1,0 },{ 0,-1 } };
const std::pair<int, int> ALL_DIRECTIONS[8] = { { 1,0 },{ 1,1 },{ 0,1 },{ -1,1 },{ -1,0 },{ -1,-1 },{ 0,-1 },{ 1,-1 } };

std::mutex coutMutex;

//initialize random time
std::default_random_engine gen(time(NULL));

//use \n to place endLine characters
void consoleOut(std::string msg) {
	coutMutex.lock();

	std::cout << msg;

	std::cout.flush();

	coutMutex.unlock();
}

//console logging function
void errorOut(std::string file, std::string msg, bool fatal) {
	std::stringstream error;
	int a;
	error << file << ": " << "Error: " << ((fatal) ? "FATAL: " : "") << msg << '\n';

	consoleOut(error.str());

	if (fatal) {
		std::cin >> a;
		exit(-1);
	}

}

void clearScreen()
{
	coutMutex.lock();
	system("cls");
	coutMutex.unlock();
}

readWriteMutex::readWriteMutex()
{
}

void readWriteMutex::read()
{
	w.lock();
}

void readWriteMutex::freeRead()
{
	w.unlock();
}

void readWriteMutex::write()
{
}

void readWriteMutex::freeWrite()
{
}

bool readWriteMutex::isReadbale()
{
	return false;
}

bool readWriteMutex::isWritable()
{
	return false;
}
