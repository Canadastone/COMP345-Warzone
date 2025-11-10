/*
* Assignment 1 - COMP 345
* Authors: Ariberto Bello Veras, Joshua Bitton, Liam Daigle, 
* Ash Stone, Cyrus Stonebanks, Kirill Vishnyakov
* Date: 2025-10-07
* Description: This file contains the implementation of the map
*/

#include "map.h"
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <stack>
#include <fstream>
#include <sstream>
#include <memory>

//-----------------------------------------------
// Map Functions
//-----------------------------------------------

Map::Map(std::string filePath) {
	MapLoader loader(filePath);
	validFile = loader.validateFile();
	if (!validFile) {
		std::cout << "Invalid file, cannot create map..." << std::endl;
		return;
	}
	std::vector<std::shared_ptr<Territory>> territories = loader.generateTerritories();
	this->adjacencyMatrix = loader.generateConnectedTerritories(territories); //This is a map containing all the active territories for the game
	initializeContinentMap();
	int indexOfLastSlash = filePath.find_last_of("/");
	this->mapName = filePath.substr(indexOfLastSlash + 1);
	std::cout << "Successfully created map..." << std::endl;

}

Map::Map(const Map& map) {
	adjacencyMatrix = map.adjacencyMatrix;
	continentMap = map.continentMap;
	mapName = map.mapName;
	std::cout << "Copy constructor called!" << std::endl;
}

Map& Map::operator=(const Map& map) {

	if (this == &map)
		return *this;

	adjacencyMatrix = map.adjacencyMatrix;
	continentMap = map.continentMap;
	mapName = map.mapName;

	std::cout << "Copy Assignment Operator Called!" << std::endl;
	return *this;
}

std::ostream& operator<<(std::ostream& os, const Map& map){

	os << "Map: " << map.mapName << std::endl;
	return os;
}

void Map::printMap() {

	for (std::pair<std::string, std::shared_ptr<Territory>> pair : this->adjacencyMatrix) {
		pair.second->printTerritory();
	}
}

std::shared_ptr<Map::Territory> Map::getTerritory(std::string territoryName) {
	if (this->adjacencyMatrix.find(territoryName) != this->adjacencyMatrix.end()) {
		std::shared_ptr<Territory> territory = this->adjacencyMatrix.at(territoryName);
		return territory;
	}
	else {
		std::cout << "Error: \"" << territoryName << "\" is not a valid territory." << std::endl;
	}
}

std::shared_ptr<Map::Territory> Map::getStartingTerritory() {
	auto iter = this->adjacencyMatrix.begin();
	std::string startingTerritoryName = iter->first;
	std::shared_ptr<Territory> startingTerritory = this->adjacencyMatrix.at(startingTerritoryName);

	return startingTerritory;
}

void Map::initializeContinentMap() {
	for (auto& pair : this->adjacencyMatrix) {
		std::string continent = pair.second->getContinent();
		std::shared_ptr<Territory> territoryPtr = pair.second;

		if (this->continentMap.count(continent) > 0) {
			this->continentMap.at(continent).push_back(territoryPtr);
		}
		else { //Continent does not yet exist as a key
			std::vector<std::shared_ptr<Territory>> continentVector;
			continentVector.push_back(territoryPtr);
			this->continentMap.insert(std::make_pair(continent, continentVector));
		}
	}
}

bool Map::isValidConnectedGraph() {

	//Vector that keeps track of which nodes have already been traversed
	std::vector<std::shared_ptr<Territory>> traversedNodes;

	//Stack that contains the history of currently traversed nodes
	std::stack<std::shared_ptr<Territory>> searchHistory;

	//Number representing the size traversedNodes should be in order to ensure that the graph has been fully traversed
	int numNodes = this->adjacencyMatrix.size();

	//Setup the algorithm
	std::shared_ptr<Territory> startingNode = getStartingTerritory();
	searchHistory.push(startingNode);
	traversedNodes.push_back(startingNode);

	while (!searchHistory.empty()) {
		//The top most node of the stack is always the current node that we are looking for paths from
		std::shared_ptr<Territory> currentNode = searchHistory.top();

		//Boolean that tells us if there are any connected nodes that have not been traversed.
		bool newPaths = false; 
		for (std::shared_ptr<Territory> connectedTerritory : currentNode->getConnectedTerritories()) {

			//If statement for if the node has already been traversed
			if (std::count(traversedNodes.begin(), traversedNodes.end(), connectedTerritory) > 0) {
				continue;
			}
			//If it has not been traversed
			else {
				searchHistory.push(connectedTerritory);
				traversedNodes.push_back(connectedTerritory);
				newPaths = true;
				break;
			}
		}

		//Success condition (i.e. all nodes have been traversed by some path)
		if (traversedNodes.size() == numNodes) {
			return true;
		}

		//If the currentnode is out of untraversed paths, pop it from the top of the stack
		if (!newPaths) { 
			searchHistory.pop();
		}
	}

	std::cout << "\t\tNot all nodes were able to be traversed, meaning the graph is disjoint!" << std::endl;
	return false;
}


bool Map::continentIsValidSubgraph(std::vector<std::shared_ptr<Map::Territory>> territoryVector, std::string continentName) {

	//Vector that keeps track of which nodes have already been traversed
	std::vector<std::shared_ptr<Territory>> traversedNodes;

	//Stack that contains the history of currently traversed nodes
	std::stack<std::shared_ptr<Territory>> searchHistory;

	//Number representing the size traversedNodes should be in order to ensure that the graph has been fully traversed
	int numNodesInContinent = territoryVector.size();

	//Setup the algorithm
	std::shared_ptr<Territory> startingNode = territoryVector.at(0);
	searchHistory.push(startingNode);
	traversedNodes.push_back(startingNode);

	while (!searchHistory.empty()) {
		//The top most node of the stack is always the current node that we are looking for paths from
		std::shared_ptr<Territory> currentNode = searchHistory.top();

		//Boolean that tells us if there are any connected nodes that have not been traversed.
		bool newPaths = false; 
		for (std::shared_ptr<Territory> connectedTerritory : currentNode->getConnectedTerritories()) {
			//If the connected territory is not in the current continent
			if (connectedTerritory->getContinent() != continentName) { 
				continue;
			}
			//If statement for if the node has already been traversed
			if (std::count(traversedNodes.begin(), traversedNodes.end(), connectedTerritory) > 0) {
				continue;
			}
			//If it has not been traversed
			else {
				searchHistory.push(connectedTerritory);
				traversedNodes.push_back(connectedTerritory);
				newPaths = true;
				break;
			}
		}

		//Success condition (i.e. all nodes have been traversed by some path)
		if (traversedNodes.size() == numNodesInContinent) {
			return true;
		}

		//If the currentnode is out of untraversed paths, pop it from the top of the stack
		if (!newPaths) {
			searchHistory.pop();
		}
	}

	std::cout << "\t\tNot all nodes were able to be traversed, meaning the continent graph is disjoint!" << std::endl;
	return false;
}

bool Map::validate() {

	std::cout << "Starting graph validation for the entire map...\n" << std::endl;
	bool isConnectedGraph = isValidConnectedGraph();

	if (!isConnectedGraph) {
		std::cout << "----------------------------------------------------------------------------------" << std::endl;
		return false;
	}
	std::cout << "\t\tMap is a valid connected graph!" << std::endl;
	std::cout << "----------------------------------------------------------------------------------" << std::endl;

	//Validating that each continent are connected subgraphs
	for (auto& continentPair : this->continentMap) {
		std::string continentName = continentPair.first;
		std::vector<std::shared_ptr<Territory>> continentNodes = continentPair.second;

		std::cout << "Starting subgraph validation for " << continentName << "...\n" << std::endl;
		bool result = continentIsValidSubgraph(continentNodes, continentName);

		if (!result) {
			std::cout << "----------------------------------------------------------------------------------" << std::endl;
			return result;
		}
		std::cout << "\t\t" << continentName << " is a valid subgraph!" << std::endl;
		std::cout << "----------------------------------------------------------------------------------" << std::endl;

	}

	std::cout << "Starting validation for every territory to belong to only one continent...\n" << std::endl;
	std::vector<std::shared_ptr<Territory>> checkedTerritories;
	for (auto& continentPair : this->continentMap) {
		std::string continentName = continentPair.first;
		std::vector<std::shared_ptr<Territory>> territoryList = continentPair.second;

		for (std::shared_ptr<Territory> territory : territoryList) {
			//If statement that checks to see if this territory has belonged to another continent
			if (std::count(checkedTerritories.begin(), checkedTerritories.end(), territory) > 0) {
				return false;
			}
			checkedTerritories.push_back(territory);
		}
	}

	std::cout << "\t\tAll territories belong to a single continent!" << std::endl;
	std::cout << "----------------------------------------------------------------------------------" << std::endl;

	return true;
}

//-----------------------------------------------
// Territory Functions
//-----------------------------------------------

Map::Territory::Territory() {

}

Map::Territory::Territory(std::string name, std::string continent) {
	this->name = name;
	this->continent = continent;
	this->units = 0;
}

Map::Territory::Territory(const Territory& territory) {

	this->name = territory.name;
	this->continent = territory.continent;
	this->owner = territory.owner;
	this->units = territory.units;
	this->connectedTerritories = connectedTerritories;
	
	std::cout << "Territory Copy Constructor Called!" << std::endl;
}

Map::Territory& Map::Territory::operator=(const Territory& territory) {

	if (this == &territory)
		return *this;

	this->name = territory.name;
	this->continent = territory.continent;
	this->owner = territory.owner;
	this->units = territory.units;
	this->connectedTerritories = territory.connectedTerritories;
	
	std::cout << "Territory Copy Assignment Operator Called!" << std::endl;
	return *this;
}

bool Map::Territory::operator==(const Territory& other) const {
    return units == other.units && name == other.name && continent == other.continent;
}

std::ostream& operator<<(std::ostream& os, const Map::Territory& territory) {
	os << "Territory:" << territory.name << std::endl;
	return os;
}

void Map::Territory::addConnection(std::shared_ptr<Territory> territory) {
	this->connectedTerritories.push_back(territory);
}


std::string Map::Territory::getName() {
	return this->name;
}

int Map::Territory::getUnits(){
	return units;
}

void Map::Territory::addUnits(int n){
	units += n;
}

void Map::Territory::removeUnits(int n){
	units -= n;
}

void Map::Territory::setUnits(int n){
	units = n;
}

std::string Map::Territory::getContinent() {
	return this->continent;
}

std::shared_ptr<Player> Map::Territory::getOwnership(){
	return this->owner.lock();
}

void Map::Territory::setOwnership(std::shared_ptr<Player> owner){
	this->owner = owner;
}

std::vector <std::shared_ptr<Map::Territory>> Map::Territory::getConnectedTerritories() {
	return this->connectedTerritories;
}


void Map::Territory::printTerritory() {

	std::cout << "Territory Name: " << this->name << std::endl;
	std::cout << "Continent: " << this->continent << std::endl;
	std::cout << "Owned by: " << *(this->owner) << std::endl;
	std::cout << "Connected Territories: ";

	for (std::shared_ptr<Territory> territory : this->connectedTerritories) {
		std::cout << territory->getName() << ", ";
	}
	std::cout << std::endl;
	std::cout << "---------------------------------------------------------------" << std::endl;
}

//-----------------------------------------------
// MapLoader Functions
//-----------------------------------------------

Map::MapLoader::MapLoader(std::string filePath) {
	this->filePath = filePath;
}

Map::MapLoader::MapLoader(const MapLoader& mapLoader) {
	this->filePath = mapLoader.filePath;
	std::cout << "Map Loader Copy Constructor Called!" << std::endl;

}

Map::MapLoader& Map::MapLoader::operator=(const MapLoader& mapLoader) {

	if (this == &mapLoader)
		return *this;

	this->filePath = mapLoader.filePath;
	std::cout << "Map Loader Copy Assignment Operator Called!" << std::endl;
	return *this;
}

std::ostream& operator<<(std::ostream& os, const Map::MapLoader& mapLoader) {
	os << "Map Loader for :" << mapLoader.filePath << std::endl;
	return os;
}
bool Map::MapLoader::validateFile() {

	std::ifstream file(this->filePath);

	std::string text;
	bool hasMap = false, hasContinents = false, hasTerritories = false;

	while (std::getline(file, text)) {

		//If file contains a Map section
		if (text == "[Map]") {
			hasMap = true;
			continue;
		}

		//If file contains a Continents section
		if (text == "[Continents]") {
			hasContinents = true;
			continue;
		}

		//If file contains a Territories section
		if (text == "[Territories]") {
			hasTerritories = true;
			continue;
		}
	}

	file.close();

	return hasMap && hasContinents && hasTerritories;
}

std::vector<std::shared_ptr<Map::Territory>> Map::MapLoader::generateTerritories() {

	std::vector<std::shared_ptr<Territory>> territoriesVector;

	std::ifstream file(this->filePath);

	std::string text;
	bool startReading = false;

	while (std::getline(file, text)) {

		if (text == "[Territories]") {
			startReading = true;
			continue;
		}

		//If statement that only gets hits if the file has read up to the "[Territories]" string in the file
		if (startReading == true) {

			if (text == "") { //Skip if there is an empty line in the file
				continue;
			}

			std::stringstream textStream(text);
			std::string item;
			char delimiter = ',';
			std::vector<std::string> items;//Indexes each value delimited by a comma into a vector list

			//Adds all values in a row into the items vector
			while (std::getline(textStream, item, delimiter)) {
				items.push_back(item);
			}

			std::string territoryName = items.at(0);
			std::string continentName = items.at(3);

			std::shared_ptr<Territory> newTerritory = std::make_shared<Territory>(territoryName, continentName);

			territoriesVector.push_back(newTerritory);
		}
	}

	file.close();

	return territoriesVector;
}

std::map<std::string, std::shared_ptr<Map::Territory>> Map::MapLoader::generateConnectedTerritories(std::vector<std::shared_ptr<Map::Territory>> generatedTerritories) {

	std::map<std::string, std::shared_ptr<Map::Territory>> map;

	std::ifstream file(this->filePath);

	std::string text;
	bool startReading = false;

	while (std::getline(file, text)) {

		if (text == "[Territories]") {
			startReading = true;
			continue;
		}

		//If statement that only gets hits if the file has read up to the "[Territories]" string in the file
		if (startReading == true) {

			if (text == "") { //Skip if there is an empty line in the file
				continue;
			}

			std::stringstream textStream(text);
			std::string item;
			char delimiter = ',';
			std::vector<std::string> items;//Indexes each value delimited by a comma into a vector of strings

			//Adds all values in a row into the items vector
			while (std::getline(textStream, item, delimiter)) {
				items.push_back(item);
			}

			//Retrieve all connected territories and store them in the connectedTerritories vector
			std::vector<std::shared_ptr<Territory>> connectedTerritories;
			for (int i = 4; i < items.size(); i++) {
				std::string territoryName = items.at(i);
				for (std::shared_ptr<Territory> territory : generatedTerritories) {
					if (territory->getName() == territoryName) {
						connectedTerritories.push_back(territory);
					}
				}
			}

			//Retrieve the territory name and continent from the row
			std::string territoryName = items.at(0);

			//Retrieve the reference to the current Territory from the already generated list of Territories
			std::shared_ptr<Territory> currentTerritory;
			for (std::shared_ptr<Territory> territory : generatedTerritories) {
				if (territory->getName() != territoryName) {
					continue;
				}
				currentTerritory = territory;
			}

			//Add the connected territories to the current territory
			for (std::shared_ptr<Territory> connectedTerritory : connectedTerritories) {
				currentTerritory->addConnection(connectedTerritory);
			}

			//Insert the territory into the adjacency matrix
			map.insert(std::make_pair(territoryName, currentTerritory));
		}

	}

	file.close();

	return map;
}
