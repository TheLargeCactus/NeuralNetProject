
#include "Cell.h"



Cell::Cell()
{

	std::uniform_int_distribution<int> dist(-10, 10);
	slope = dist(gen);
	offset = dist(gen);

}

Cell::Cell(const int inSlope, const int inOffset)
{
	slope = inSlope;
	offset = inOffset;
}

Cell::Cell(const Cell & rhs)
{
	*this = rhs;
}



const Cell & Cell::operator=(const Cell & rhs)
{
	offset = rhs.getOffset();
	slope = rhs.getSlope();
	state = rhs.state;
	in = rhs.in;

	return *this;
}

uint64_t Cell::transform() const
{
	uint64_t total = 0;

	for (std::set<Cell*>::iterator i = in.begin(); i != in.end(); i++) {
		total += ((*i)->transform());
	}

	return slope * total + offset;
}

const int Cell::getOffset() const
{
	return offset;
}

const int Cell::getSlope() const
{
	return slope;
}

bool Cell::pushCell(Cell * src)
{
	if (in.insert(src).second) {
		return true;
	}

	errorOut("Cell","Failed to insert Cell * into 'in' list in instance of Cell", true);
	return false;
}

bool Cell::popCell(Cell * tgt)
{
	for (std::set<Cell*>::iterator i = in.begin(); i != in.end(); i++) {
		if ((*i) == tgt) {
			in.erase(i);
			return true;
		}
	}

	errorOut("Cell", "Failed to find Cell * in 'in' list so item could not be erased", true);
	return false;
}


