#pragma once
#include "Cell.h"
#include "InputCell.h"
#include <random>
#include <sstream>
#include <fstream>
#include <cmath>
#include "Board.h"
#include "Lib.h"
#include <map>

const int RATIO_CELL_TO_LAYER = 1; //ideal ratio 5 cells to 1 layer
const int RATIO_LINK_TO_CELL = 1; //ideal ratio 2 links per 1 cell

class Brain {
private:
	//the key is the ID, and the pair is the layer and a pointer to the object
	std::map<int,std::pair<int,Cell *>> web;
	std::set<int>layerList; //layers in use
	std::set<int>idList; //ID's in use
	std::vector<std::pair<int, int>> links;
	

	//return the skewed ratio of numerator/denominator
	double skewRatio(const int idealRatio, int num, int den);

	int getNextLayer();
	int getNextID();

	int getHighestLayer() const;

	bool freeID(int tID);
	bool freeLayer(int layer);

	
public:
	//constructors
	//call constructor/default constructor with default value
	Brain(int boardSize = BOARDSIZE);

	//call constructor for chromosome import
	Brain(std::string chrom);

	//copy
	Brain(const Brain & rhs);

	//destructor
	~Brain();

	//destructor proxy
	bool resetBrain();

	//operator functions
	const bool operator<(const Brain & rhs) const;

	const bool operator==(const Brain & rhs) const;

	const bool operator<=(const Brain & rhs) const;

	const bool operator>(const Brain & rhs) const;

	const bool operator>=(const Brain & rhs) const;

	const bool operator!=(const Brain & rhs) const;

	Brain & operator=(const Brain & rhs);

	//member functions
	//add new cell to brain
	bool growCell();

	//delete cell from brain, removing all links
	bool killCell();

	//create link between two cells
	bool growLink();

	//remove link between two random cells
	bool killLink();

	//add new layer to layers
	bool growLayer();

	//remove layer from layers
	bool killLayer();

	//perform grow/kill actions randomly
	bool mutate(int growthRate, bool perfectMutate);

	//return string representing chromosome of brain
	std::string getChromosome() const;

	//change cell to match chromosome of input
	bool setChromosome(std::string chrom);

	//find address of Cell in brain, if not exist return NULL
	Cell * findCell(int cID) const;

	//return number of cells in brain
	int size() const;

	//output chromosome to file
	bool saveToFile(std::string fName) const;

	//remove all links that include id of tID
	bool removeAllLinks(int tID);

	bool makeCell(int layer);

	//make cell with specific ID
	bool makeCell(int nSlope, int nOffset, int layer);

	//remove cell with specific ID
	bool removeCell(int cID);

	//make link between two id's
	bool makeLink(int pID, int cID);

	//remove link between parent pID and child cID
	bool removeLink(int pID, int cID);

	//make a new layer
	bool makeLayer(int pos);

	//remove a layer at position
	bool removeLayer(int pos);

	//returns number of layers in brain
	int layerCount() const;

	//return number of cells in brain
	int cellCount() const;

	//return number of links in brain
	int linkCount() const;

	//get integer answer to integer input
	std::uint64_t answer(Board & question, bool player);
};