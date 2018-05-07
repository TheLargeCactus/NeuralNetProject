#pragma once


#include <stdint.h>
#include <vector>
#include <sstream>

#include "Lib.h"




class BitStore {
private:
	uint64_t bits;
	uint64_t powerOfTwo(uint64_t pow) const;
public:
	//default constructor
	BitStore();

	template <typename T>
	BitStore(T in);

	bool operator==(const BitStore & rhs) const;

	//retrieve value at position pos
	bool at(uint64_t pos) const;

	//set bit at position pos
	uint64_t set(uint64_t pos);

	//clear bit at position pos
	uint64_t clear(uint64_t pos);

	//overwrite bit at position pos with value val
	uint64_t overwrite(uint64_t pos, bool val);

	//toggle bit at position pos
	uint64_t toggle(uint64_t pos);

	//return size of BitStore
	int size() const;

	//display contents of bitstore
	bool display() const;
};

class Board {
private:
	//if isPlaced is 1, piece exists at location
	//pieceColor is white if 0, black if 1, undefined if isPlaced is 0
	BitStore isPlaced, pieceColor;
	int lastPiecePlaced;
	std::set<int> piecesChanged;
public:
	//convert location to x,y coords
	std::pair<int, int> getXY(int loc) const;

	//convert x,y coords into location
	int getLoc(int x, int y) const;

	int getLoc(std::pair<int, int> coord) const;

	//get bounds of row or column respectively
	//returns all valid line values of either row or col
	std::set<int> rowBounds(int loc) const;
	std::set<int> colBounds(int loc) const;
	//get diagonals
	std::set<int> diagonalBounds(int loc) const;

	std::set<int> allBounds(int loc) const;

	//get location at direction of radius from pos
	//directions
	//0 East, 1 North East, 2 North, 3 North West, 4 West, 5 South West, 6 South, 7 South East
	//returns 0 if piece is white, 1 if piece is black, 2 if piece is empty
	int getLocAt(int pos, int radius, int direction) const;

	//returns true if the location exists outside array
	bool outOfBounds(int loc) const;

	//returns true if location exists outside of bounds of x or y
	bool outOfBounds(std::pair<int, int> coord) const;

	//returns true if piece at index potentiallyOnPath is not in the valid paths of loc
	bool outOfValidPath(int loc, int potentiallyOnPath, int direction) const;

	//get a representation of direction in the path of a location
	std::vector<int> lineComp(int pos, int direction, bool player) const;

	bool validLineComp(std::vector<int> line, bool player) const;


	//default constructor
	//create board in beginning state
	//WB
	//BW
	Board();

	//reset the board
	bool clearBoard();

	std::pair<BitStore, BitStore> getState() const;

	//get the current board state, 0 for opponents piece, 1 for owners piece, 2 for empty space
	//player denotes which player is currently asking for the board state
	std::vector<int> getBoardContext(bool player) const;

	//place piece for player 0 or 1
	bool placePiece(uint64_t pos, bool player);

	//place piece, checking to see if placement is valid first
	bool protectedPlacePiece(uint64_t pos, bool player);

	//place piece, changing other pieces as if player had placed it
	bool gamerulePlacePiece(uint64_t pos, bool player);

	//operator overload for at
	int operator[](int pos);
	//return value at position pos
	int at(int pos) const;

	//get valid moves for player, all bits returned with value 1 are valid
	BitStore getValidMoves(bool player) const;

	//return count of players pieces
	int pieceCount(bool player);

	//display board to output
	void displayBoard() const;

	void displayBoardGUI(sf::RenderWindow & window) const;

	void highlightValidMoves(sf::RenderWindow & window,bool player) const;

};

template<typename T>
inline BitStore::BitStore(T in)
{
	bits = in;
}
