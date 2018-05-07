#pragma once
#include <vector>
#include <set>

#include <algorithm>
#include <random>
#include <stdint.h>

#include "Lib.h"

class Cell {
protected:
	int offset;
	int slope;
	std::set<Cell *> in;
	
public:
	std::uint8_t state;
	//constructors
	//default
	Cell();

	//other call constructor
	Cell(const int inSlope, const int inOffset);


	//copy constructor
	Cell(const Cell & rhs);

	//operator functions

	const Cell & operator=(const Cell & rhs);

	//member functions
	//function to transform input into output
	virtual uint64_t transform() const;

	//get offset from cell
	const int getOffset() const;

	//get slope from cell
	const int getSlope() const;

	//function to add cell to input vector
	bool pushCell(Cell * src);

	//function to remove cell from input vector
	bool popCell(Cell * tgt);


};