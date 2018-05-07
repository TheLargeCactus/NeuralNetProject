#pragma once

#include <mutex>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <ctime>
#include <random>
#include <set>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <future>

extern const float TILESIZE;
extern const float PIECESIZE;

extern const uint64_t BOARDSIZE;
extern const int LAMBDA;
extern const bool PERFECT;

extern const std::pair<int, int> DIAGONAL_DIRECTIONS[4];
extern const std::pair<int, int> CARDINAL_DIRECTIONS[4];
extern const std::pair<int, int> ALL_DIRECTIONS[8];

extern std::default_random_engine gen;

extern std::mutex coutMutex;

//append \n to denote new lines in message
void consoleOut(std::string msg);

//error logging function
void errorOut(std::string file,std::string msg, bool fatal);

//get input from the console
//message should not contain a new line character
template<typename T>
T consoleIn(std::string msg) {
	T temp;
	coutMutex.lock();

	std::cout << msg;
	std::cin >> temp;
	std::cout << std::endl;

	coutMutex.unlock();

	return temp;
}

//function to clear screen
void clearScreen();

//function to add vectors
template<typename T>
std::vector<T> operator+(const std::vector<T> & lhs, const std::vector<T> & rhs) {
	std::vector<T> temp;

	temp.reserve(lhs.size() + rhs.size());
	temp.insert(temp.end(), lhs.begin(), lhs.end());
	temp.insert(temp.end(), rhs.begin(), rhs.end());

	return temp;
}

//function to combine sets
template<typename T>
std::set<T> operator+(const std::set<T> & lhs, const std::set<T> & rhs) {
	std::set<T> temp;

	temp.insert(lhs.begin(), lhs.end());
	temp.insert(rhs.begin(), rhs.end());

	return temp;
}

//function to add first element pair with counterpart and same for second element
template<typename T,typename U>
std::pair<T, U> operator+(const std::pair<T, U> & lhs, const std::pair<T, U> & rhs) {
	std::pair<T, U> temp;

	temp.first = lhs.first + rhs.first;
	temp.second = lhs.second + rhs.second;

	return temp;
}


class readWriteMutex {
private:
	std::mutex r;
	std::mutex w;

public:
	readWriteMutex();

	readWriteMutex(const readWriteMutex & rhs) = delete;

	void read();

	void freeRead();

	void write();

	void freeWrite();

	bool isReadbale();

	bool isWritable();

};