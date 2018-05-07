
#include "Board.h"


uint64_t BitStore::powerOfTwo(uint64_t pow) const
{
	if (pow > 1) {
		return 4 * powerOfTwo(pow - 2);
	}

	if (pow == 1) {
		return 2;
	}

	if (pow == 0) {
		return 1;
	}


	errorOut("Board","Negative exponent given to power of 2 function", true);
	return -1;

}

BitStore::BitStore()
{

	bits = 0;
	

}

bool BitStore::operator==(const BitStore & rhs) const
{
	return bits == rhs.bits;
}

bool BitStore::at(uint64_t pos) const
{
	return bits & powerOfTwo(pos);
}

uint64_t BitStore::set(uint64_t pos)
{
	return bits |= powerOfTwo(pos);
}

uint64_t BitStore::clear(uint64_t pos)
{
	return bits &= ~powerOfTwo(pos);
}

uint64_t BitStore::overwrite(uint64_t pos, bool val)
{
	if (val) {
		return set(pos);
	}

	return clear(pos);
}

uint64_t BitStore::toggle(uint64_t pos)
{
	if (at(pos)) {
		return clear(pos);
	}

	return set(pos);
}

int BitStore::size() const
{
	return 8 * sizeof(bits);
}

bool BitStore::display() const
{
	consoleOut(std::to_string(size()));

	for (int i = 0; i < size(); i++) {
		consoleOut(std::to_string(at(i)));
	}

	consoleOut("\n");
	return true;
}

std::pair<int, int> Board::getXY(int loc) const
{
	int x = loc % BOARDSIZE;
	int y = loc / BOARDSIZE;

	return std::make_pair(x, y);
}

int Board::getLoc(int x, int y) const
{
	return y * BOARDSIZE + x;
}

int Board::getLoc(std::pair<int, int> coord) const
{
	return getLoc(coord.first, coord.second);
}

std::set<int> Board::rowBounds(int loc) const
{
	std::set<int> values;
	int value = loc - loc % BOARDSIZE;

	for (int i = 0; i < BOARDSIZE; i++) {
		values.insert(value + i);
	}

	return values;
}

std::set<int> Board::colBounds(int loc) const
{
	std::set<int> values;
	int lowerBound = loc % BOARDSIZE;

	for (int i = 0; i < BOARDSIZE; i++) {
		values.insert(lowerBound + BOARDSIZE * i);
	}

	return values;
}

std::set<int> Board::diagonalBounds(int loc) const
{
	std::set<int> temp;
	std::pair<int, int> coord;
	
	for (int i = 0; i < 4; i++) {
		coord = getXY(loc);
		while (!outOfBounds(coord)) {
			temp.insert(getLoc(coord));
			coord = coord + ALL_DIRECTIONS[2 * i + 1];
		}
	}

	return temp;
}

std::set<int> Board::allBounds(int loc) const
{
	return rowBounds(loc) + colBounds(loc) + diagonalBounds(loc);
}


int Board::getLocAt(int pos, int radius, int direction) const
{
	//first value is row, second is col
	return pos + radius * (BOARDSIZE * ALL_DIRECTIONS[direction].second/*y*/ + ALL_DIRECTIONS[direction].first/*x*/);
}

bool Board::outOfBounds(int loc) const
{
	return loc < 0 || loc > BOARDSIZE * BOARDSIZE - 1;
}

bool Board::outOfBounds(std::pair<int, int> coord) const
{
	if (coord.first < 0 || coord.first >= BOARDSIZE) {
		return true;
	}

	if (coord.second < 0 || coord.second >= BOARDSIZE) {
		return true;
	}

	return false;
}

bool Board::outOfValidPath(int loc, int potentiallyOnPath,int direction) const
{
	std::set<int> validLocations;
	switch (direction) {
	case 0:
	case 4:
		validLocations = rowBounds(loc);
		break;
	case 2:
	case 6:
		validLocations = colBounds(loc);
		break;
	case 1:
	case 3:
	case 5:
	case 7:
		validLocations = diagonalBounds(loc);
		break;
	default:
		errorOut("Board", "Case statement received a direction value that doesnt correspond to a direction", true);

	}
	
	if (validLocations.find(potentiallyOnPath) != validLocations.end()) {
		return false;
	}

	return true;
}

std::vector<int> Board::lineComp(int pos, int direction, bool player) const
{
	std::vector<int>temp;
	int rad = 1;
	int play;
	int opposite;
	std::pair<int, int> loc;

	if (player) {
		play = 1;
		opposite = 0;
	}
	else {
		play = 0;
		opposite = 1;
	}

	loc = getXY(pos);
	temp.push_back(at(pos));
	loc = loc + ALL_DIRECTIONS[direction];
	

	while (!outOfBounds(loc) && at(getLoc(loc))!= 2) {
		temp.push_back(at(getLoc(loc)));
		if (at(getLoc(loc)) == play) {
			break;
		}
		loc = loc + ALL_DIRECTIONS[direction];
	}

	return temp;
}

bool Board::validLineComp(std::vector<int> line, bool player) const
{
	//using an integer clears up problems with bool/int conversion
	int play;

	if (player) {
		play = 1;
	}
	else {
		play = 0;
	}

	//valid lines take the shape <empty><opposite><same>
	//<empty> = an empty space
	//<opposite> = <opposite><o> | <o>
	//<o> = other players piece
	// <same> = this players piece

	//if line is not long enough to form a valid one, return false
	if (line.size() < 3) {
		return false;
	}

	//if first position of a line is not empty, return false
	if (line[0] != 2) {
		return false;
	}

	//if last piece is not the same as player return false
	if (line[line.size() - 1] != play) {
		return false;
	}


	//for each middle piece, check if they are the opposite players piece, and not an empty spot
	for (int i = 1; i < line.size() - 1; i++) {
		if (line[i] == play || line[i] == 2) {
			return false;
		}
	}

	return true;
}



Board::Board()
{
	lastPiecePlaced = -1;
	placePiece(27, 0);
	placePiece(28, 1);
	placePiece(35, 1);
	placePiece(36, 0);
}

bool Board::clearBoard()
{
	isPlaced = BitStore(0);
	pieceColor = BitStore(0);

	return true;
}

std::pair<BitStore, BitStore> Board::getState() const
{
	return std::pair<BitStore, BitStore>(isPlaced, pieceColor);
}

std::vector<int> Board::getBoardContext(bool player) const
{
	std::vector<int> temp;
	int piece;

	for (int i = 0; i < BOARDSIZE * BOARDSIZE; i++) {
		if (player) {
			//if player 1, this is just indicative of board state
			piece = (isPlaced.at(i)) ? pieceColor.at(i) : 2;
		}
		else {
			//if player 0, reverse values except for empty spots
			piece = (isPlaced.at(i)) ? ((pieceColor.at(i)) ? 0 : 1) : 2;
		}

		temp.push_back(piece);
	}

	return temp;
}

bool Board::placePiece(uint64_t pos, bool player)
{
	if (pos > BOARDSIZE * BOARDSIZE - 1) {
		errorOut("Board","Tried to place pieece in game board that existed outside bounds of board", false);
		return false;
	}

	isPlaced.set(pos);

	//if white player, place white piece
	if (!player) {
		pieceColor.clear(pos);
		return true;
	}

	pieceColor.set(pos);
	return true;
}

bool Board::protectedPlacePiece(uint64_t pos, bool player)
{
	if (isPlaced.at(pos)) {
		errorOut("Board","Player tried to place a piece in a spot that was already occupied.", false);
		return false;
	}

	return placePiece(pos, player);
}

bool Board::gamerulePlacePiece(uint64_t pos, bool player)
{
	//vector to store path information, first value is a valid direction
	//second is the length of the line
	std::vector<int> paths;
	int play;
	int rad;
	int changePiece;
	int opposite;

	if (player) {
		play = 1;
		opposite = 0;
	}
	else {
		play = 0;
		opposite = 1;
	}

	if (outOfBounds(pos)) {
		return false;
	}

	//push back valid changing directions
	for (int i = 0; i < 8; i++) {
		if (validLineComp(lineComp(pos, i, player), player)) {
			paths.push_back(i);
		}
	}

	//return false if this does not change anything
	if (!paths.size()) {
		return false;
	}

	placePiece(pos, player);
	lastPiecePlaced = pos;
	piecesChanged.clear();

	for (int i = 0; i < paths.size(); i++) {
		rad = 1;
		changePiece = getLocAt(pos, rad, paths[i]);
		while (!outOfValidPath(pos, changePiece, paths[i]) && at(changePiece) == opposite) {
			placePiece(changePiece, player);
			piecesChanged.insert(changePiece);
			rad++;
			changePiece = getLocAt(pos, rad, paths[i]);
		}
	}

	return true;
}

int Board::operator[](int pos)
{
	return at(pos);
}

int Board::at(int pos) const
{
	if (!outOfBounds(pos)) {
		return (isPlaced.at(pos)) ? pieceColor.at(pos) : 2;
	}

	errorOut("Board","attempted to get the value of an out of bound index", true);
	return -1;
}

BitStore Board::getValidMoves(bool player) const
{
	BitStore temp;

	for (int i = 0; i < BOARDSIZE * BOARDSIZE; i++) {
		if (!isPlaced.at(i)) {
			for (int j = 0; j < 8; j++) {
				if (validLineComp(lineComp(i, j, player), player)) {
					temp.set(i);
					j = 8; //break inner loop, work is done
				}
			}
		}
	}

	return temp;
}

int Board::pieceCount(bool player)
{
	int play;

	if (player) {
		play = 1;
	}
	else {
		play = 0;
	}

	int total = 0;
	for (int i = 0; i < BOARDSIZE * BOARDSIZE; i++) {
		if (at(i) == play) {
			total++;
		}
	}

	return total;
}

void Board::displayBoard() const
{
	int loc;
	std::stringstream board;

	for (int i = 0; i < BOARDSIZE; i++) {
		for (int j = 0; j < BOARDSIZE; j++) {
			loc = BOARDSIZE * i + j;
			if (isPlaced.at(loc)) {
				if (pieceColor.at(loc)) {
					board << 'B';
				}
				else {
					board << 'W';
				}
			}
			else {
				board << 'O';
			}
		}
		board << '\n';
	}


	consoleOut(board.str());
}

void Board::displayBoardGUI(sf::RenderWindow & window) const
{
	sf::RectangleShape tempTile(sf::Vector2f(PIECESIZE,PIECESIZE));
	sf::CircleShape tempPiece(PIECESIZE / 2);

	bool checkered = false;

	/*tempTile.setOutlineColor(sf::Color(0x000000FF));
	tempTile.setOutlineThickness(2);
	tempPiece.setOutlineColor(sf::Color(0x000000FF));
	tempPiece.setOutlineThickness(2);*/

	for (int i = 0; i < BOARDSIZE; i++) {
		for (int j = 0; j < BOARDSIZE; j++) {
			if (checkered) {
				tempTile.setFillColor(sf::Color(0xBEBEBEFF));
			}
			else {
				tempTile.setFillColor(sf::Color(0x787878FF));
			}
			if (i*BOARDSIZE + j == lastPiecePlaced) {
				tempTile.setFillColor(sf::Color(0x00FF00FF));
			}
			if (piecesChanged.find(i*BOARDSIZE + j) != piecesChanged.end()) {
				tempTile.setFillColor(sf::Color(0x0000FFFF));
			}
			checkered = !checkered;

			tempTile.setPosition(sf::Vector2f(j*PIECESIZE, i*PIECESIZE));
			tempPiece.setPosition(sf::Vector2f(j*PIECESIZE, i*PIECESIZE));

			if (at(i * BOARDSIZE + j) == 1) {
				tempPiece.setFillColor(sf::Color(0x000000FF));
			}
			else if(at(i*BOARDSIZE+j)== 0){
				tempPiece.setFillColor(sf::Color(0xFFFFFFFF));
			}

			
			window.draw(tempTile);

			if (at(i * BOARDSIZE + j) != 2) {
				window.draw(tempPiece);
			}
		}
		checkered = !checkered;
	}
}

void Board::highlightValidMoves(sf::RenderWindow & window, bool player) const
{
	sf::RectangleShape tempTile(sf::Vector2f(PIECESIZE, PIECESIZE));
	tempTile.setFillColor(sf::Color(0xFFFF00FF));

	BitStore moves = getValidMoves(player);
	for (int i = 0; i < BOARDSIZE; i++) {
		for (int j = 0; j < BOARDSIZE; j++) {
			if (moves.at(i*BOARDSIZE+j)) {
				tempTile.setPosition(sf::Vector2f(j*PIECESIZE, i*PIECESIZE));
				window.draw(tempTile);
			}
		}
	}
}
