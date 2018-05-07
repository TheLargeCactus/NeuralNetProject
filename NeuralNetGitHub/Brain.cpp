#include "Brain.h"

double Brain::skewRatio(const int idealRatio, int num, int den)
{
	return 0.5;
	//return idealRatio * double(den) / double(num);
}

int Brain::getNextLayer()
{
	int i = 0;
	for (std::set<int>::iterator iter = layerList.begin(); iter != layerList.end(); iter++) {
		if (i != *iter) {
			layerList.insert(i);
			return i;
		}
		else {
			i++;
		}
	}
	layerList.insert(i);
	return i;
}

int Brain::getNextID()
{
	int i = 0;
	for (std::set<int>::iterator iter = idList.begin(); iter != idList.end(); iter++) {
		if (i != *iter) {
			idList.insert(i);
			return i;
		}
		else {
			i++;
		}
	}
	idList.insert(i);
	return i;
}

int Brain::getHighestLayer() const
{
	if (!layerList.size()) {
		return 0;
	}
	return *(layerList.rbegin());
}

bool Brain::freeID(int tID)
{
	std::set<int>::iterator iter = idList.find(tID);

	if (iter != idList.end()) {
		idList.erase(iter);
		return true;
	}

	return false;
}

bool Brain::freeLayer(int layer)
{
	std::set<int>::iterator iter = layerList.find(layer);

	if (iter != layerList.end()) {
		layerList.erase(iter);
		return true;
	}

	return false;
}

Brain::Brain(int boardSize)
{
	int newID;
	layerList.insert(getNextLayer());
	for (int i = 0; i < boardSize * boardSize; i++) {
		newID = getNextID();
		web[newID] = std::make_pair(0,(Cell*) new InputCell());
	}


}

Brain::Brain(std::string chrom) 
{
	setChromosome(chrom);
}

Brain::Brain(const Brain & rhs)
{
	*this = rhs;
}

Brain::~Brain()
{
	resetBrain();
}

bool Brain::resetBrain()
{
	std::map<int, std::pair<int, Cell*>>::iterator iter;
	for (iter = web.begin(); iter != web.end(); iter++) {
		delete (*iter).second.second;
	}

	web.erase(web.begin(), web.end());
	layerList.erase(layerList.begin(), layerList.end());
	idList.erase(idList.begin(), idList.end());
	links.resize(0);


	return true;
}

const bool Brain::operator<(const Brain & rhs) const
{
	return size() < rhs.size();
}

const bool Brain::operator==(const Brain & rhs) const
{
	return size() == rhs.size();
}

const bool Brain::operator<=(const Brain & rhs) const
{
	return *this < rhs || *this == rhs;
}

const bool Brain::operator>(const Brain & rhs) const
{
	return !(*this <= rhs);
}

const bool Brain::operator>=(const Brain & rhs) const
{
	return !(*this < rhs);
}

const bool Brain::operator!=(const Brain & rhs) const
{
	return !(*this == rhs);
}

Brain & Brain::operator=(const Brain & rhs)
{
	setChromosome(rhs.getChromosome());

	return *this;
}

bool Brain::growCell()
{

	std::set<int>::iterator iter = layerList.begin();
	int layer;

	if (layerList.size() < 2) {
		return false;
	}

	std::uniform_int_distribution<int> dist(1, layerList.size()-1);
	layer = dist(gen);

	for (int i = 0; i < layer; i++) {
		if (iter == layerList.end()) {
			errorOut("Brain", "Iterator reached the end of set before finding element, this shouldnt happen", true);
			return false;
		}
		iter++;
	}

	return makeCell(*iter);

}

bool Brain::killCell()
{
	std::uniform_int_distribution<int> dist;
	std::vector<int> deleteTargets;
	std::set<int>::iterator iter;
	int layer;

	if (layerList.size() < 2) {
		return false;
	}

	if (!web.size()) {
		return false;
	}
	
	dist = std::uniform_int_distribution<int>(1, layerList.size() - 1);

	layer = dist(gen);

	iter = layerList.begin();

	for (int i = 0; i < layer; i++) {
		iter++;
	}

	layer = *iter;

	for (std::map<int, std::pair<int, Cell*>>::iterator it = web.begin(); it != web.end(); it++) {
		if ((*it).second.first == layer) {
			deleteTargets.push_back((*it).first);
		}
	}

	if (!deleteTargets.size()) {
		return false;
	}

	dist = std::uniform_int_distribution<int>(0, deleteTargets.size()-1);


	return removeCell(deleteTargets[dist(gen)]);
}

bool Brain::growLink()
{
	int pID;
	int cID;
	int pLayer;
	int cLayer;
	std::vector<int> possibleParents, possibleChildren;
	std::map<int, std::pair<int, Cell*>>::iterator iter;

	if (layerList.size() < 2) {
		return false;
	}

	std::uniform_int_distribution<int> dist(0, layerList.size() - 2);
	pLayer = dist(gen);

	dist = std::uniform_int_distribution<int>(pLayer+1,layerList.size()-1);
	cLayer = dist(gen);

	
	for (iter = web.begin(); iter != web.end(); iter++) {
		if ((*iter).second.first == pLayer) {
			possibleParents.push_back((*iter).first);
		}

		if ((*iter).second.first == cLayer) {
			possibleChildren.push_back((*iter).first);
		}
	}
	

	if (!possibleParents.size() || !possibleChildren.size()) {
		return false;
	}


	dist = std::uniform_int_distribution<int>(0, possibleParents.size() - 1);
	pID = possibleParents[dist(gen)];

	dist = std::uniform_int_distribution<int>(0, possibleChildren.size() - 1);
	cID = possibleChildren[dist(gen)];

	return makeLink(pID, cID);
}

bool Brain::killLink()
{

	if (!links.size()) {
		return false;
	}

	std::uniform_int_distribution<int> dist(0, links.size() - 1);

	int link = dist(gen);

	return removeLink(links[link].first, links[link].second);

}

bool Brain::growLayer()
{

	return makeLayer(getNextLayer());

}

bool Brain::killLayer()
{
	std::uniform_int_distribution<int> dist;
	if (layerList.size() < 2) {
		return false;
	}

	dist = std::uniform_int_distribution<int>(1, layerList.size() - 1);

	return removeLayer(dist(gen));
}

bool Brain::mutate(int growthRate, bool perfectMutate)
{
	bool succeeded;
	double choice;
	double chances[] = { 0.52,0.1,0.55,0.2,0.51,0.24 };
	double total;
	std::uniform_real_distribution<double> dist;
	int choiceCase;
	std::stringstream out;

	//index 0 is layer chance, 2 is cell chance, 4 is link chance
	for (int i = 0; i < growthRate; i++) {
		//chances[2] = skewRatio(RATIO_CELL_TO_LAYER, cellCount(), layerCount());
		//chances[4] = skewRatio(RATIO_LINK_TO_CELL, linkCount(), cellCount());

		total = 0.0;

		for (int j = 0; j < 6; j++) {
			total += chances[j];
		}

		dist = std::uniform_real_distribution<double>(0.0,total);

		choice = dist(gen);
		choiceCase = -1;
		while (choice > 0) {
			choice -= chances[++choiceCase];
		}

		succeeded = true;

		switch (choiceCase) {
		case 0:
			succeeded = growLayer();
			break;
		case 1:
			succeeded = killLayer();
			break;
		case 2:
			succeeded = growCell();
			break;
		case 3:
			succeeded = killCell();
			break;
		case 4:
			succeeded = growLink();
			break;
		case 5:
			succeeded = killLink();
			break;
		default:
			errorOut("Brain","Switch inside mutate function of Brain received an impossible answer", true);
			return false;
			break;
		}

		if (perfectMutate && !succeeded) {
			i--;
		}

		
	}
	
	return true;
}

std::string Brain::getChromosome() const
{
	
	std::stringstream out;

	for (std::map<int, std::pair<int, Cell*>>::const_iterator iter = web.begin(); iter != web.end(); iter++) {
		out << 'c' << ' '
			<< (*iter).first << ' ' //ID
			<< (*iter).second.first << ' ' //Layer
			<< (*iter).second.second->getSlope() << ' ' //Slope
			<< (*iter).second.second->getOffset() << ' '; //offset
	}

	for (int i = 0; i < links.size(); i++) {
		out << 'x' << links[i].first << ' ' << links[i].second << ' ';
	}
	
	return out.str();
}

bool Brain::setChromosome(std::string chrom)
{

	std::stringstream tokens(chrom);
	std::string token;
	int layer;
	int pID;
	int cID;
	int nOffset, nSlope;
	int count = -1;

	//delete previous contents
	resetBrain();

	while (std::getline(tokens, token, ' ')) {


		if (token == "c") {
			std::getline(tokens, token, ' ');
			pID = stoi(token);

			std::getline(tokens, token, ' ');
			layer = stoi(token);

			std::getline(tokens, token, ' ');
			nSlope = stoi(token);

			std::getline(tokens, token, ' ');
			nOffset = stoi(token);

			layerList.insert(layer);
			idList.insert(pID);

			if (nSlope == -11 && nOffset == -11) {
				web[pID] = std::make_pair(layer,  (Cell*) new InputCell());
			}
			else {
				web[pID] = std::make_pair(layer, new Cell(nSlope,nOffset));
			}
			
		}

		if (token == "x") {
			std::getline(tokens, token, ' ');
			pID = stoi(token);
			std::getline(tokens, token, ' ');
			cID = stoi(token);

			makeLink(pID, cID);
		}
	}

	return true;



	
}


Cell * Brain::findCell(int cID) const
{
	std::map<int, std::pair<int, Cell*>>::const_iterator iter = web.find(cID);

	if (iter == web.end()) {
		return NULL;
	}

	return iter->second.second;
}

int Brain::size() const
{
	return cellCount();
}

bool Brain::saveToFile(std::string fName) const
{
	std::ofstream outFile;

	outFile.open(fName);

	if (!outFile) {
		errorOut("Brain","Could not open file to output brain chromosome", true);
		return false;
	}

	outFile << getChromosome();

	outFile.close();

	return true;
}

bool Brain::removeAllLinks(int tID)
{
	for (int i = links.size() -1; i >=0; i--) {
		if (links[i].first == tID || links[i].second == tID) {
			removeLink(links[i].first, links[i].second);
		}
	}

	return true;
}

bool Brain::makeCell(int layer)
{
	int newID = getNextID();

	web[newID] = std::make_pair(layer, new Cell());

	if (web.find(newID) == web.end()) {
		freeID(newID);
		return false;
	}

	return true;
}

bool Brain::makeCell( int nSlope, int nOffset, int layer)
{
	int newID = getNextID();

	web[newID] = std::make_pair(layer, new Cell(nSlope,nOffset));

	if (web.find(newID) == web.end()) {
		return false;
	}

	return true;
}

bool Brain::removeCell(int cID)
{

	std::map<int, std::pair<int, Cell*>>::iterator iter = web.find(cID);
	
	if (iter != web.end()) {
		removeAllLinks(cID);
		freeID(cID);
		delete iter->second.second;
		web.erase(iter);
		return true;
	}
	

	return false;
}

bool Brain::makeLink(int pID, int cID)
{

	Cell * tgtParent = findCell(pID);
	Cell * tgtChild = findCell(cID);

	if (tgtParent == NULL || tgtChild == NULL) {
		return false;
	}

	//check if link already exists
	for (int i = 0; i < links.size(); i++) {
		if (std::make_pair(pID, cID) == links[i]) {
			return false;
		}
	}

	if (tgtChild->pushCell(tgtParent)) {
		links.push_back(std::make_pair(pID, cID));
		return true;
	}

	return false;
}

bool Brain::removeLink(int pID, int cID)
{

	//check if link exists
	for (int i = links.size()-1; i > -1; i--) {
		if (pID == links[i].first && cID == links[i].second) {

			if (findCell(cID)->popCell(findCell(pID))) {
				links.erase(links.begin() + i);
				return true;
			}
			else {
				errorOut("Brain","Tried to remove a link that didn't exist", true);
				return false;
			}
		}
	}

	return false;
}

bool Brain::makeLayer(int pos)
{
	layerList.insert(pos);

	if (layerList.find(pos) != layerList.end()) {
		return true;
	}

	return false;
 }

bool Brain::removeLayer(int pos)
{
	std::map<int, std::pair<int, Cell*>>::iterator temp;
	for (std::map<int, std::pair<int, Cell*>>::iterator iter = web.begin(); iter != web.end();) {
		if (iter->second.first == pos) {
			temp = iter;
			temp++;
			removeCell(iter->first);
			iter = temp;
		}
		else {
			iter++;
		}
	}

	return freeLayer(pos);

}

int Brain::layerCount() const
{
	return layerList.size();
}

int Brain::cellCount() const
{
	return web.size();
}

int Brain::linkCount() const
{
	return links.size();
}

std::uint64_t Brain::answer(Board & question, bool player)
{
	int i = 0;
	uint64_t total = 0;
	int lastLayer = getHighestLayer();

	std::vector<int> states = question.getBoardContext(player);
	std::vector<int> layer0;
	std::vector<int> finalLayer;



	for (std::map<int, std::pair<int, Cell*>>::iterator iter = web.begin(); iter != web.end(); iter++) {
		if (iter->second.first == 0) {
			layer0.push_back(iter->first);
		}

		if (iter->second.first == lastLayer) {
			finalLayer.push_back(iter->first);
		}
	}

	for (int i = 0; i < layer0.size(); i++) {
		web[layer0[i]].second->state = states[i];
	}

	for (int i = 0; i < finalLayer.size(); i++) {
		total += web[finalLayer[i]].second->transform();
	}

	return total;
}
