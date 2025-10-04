/*
* Assignment 1 - COMP 345
* Authors: Ariberto Bello Veras, Joshua Bitton, Liam Daigle, 
* Ash Stone, Cyrus Stonebanks, Kirill Vishnyakov
* Date: 2025-10-07
* Description: This file contains the implementation of the map
*/

#include "Map.h"
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
	MapLoader loader;
	bool validFile = loader.validateFile(filePath);
	if (!validFile) {
		std::cout << "Invalid file, cannot create map..." << std::endl;
		return;
	}
	std::vector<std::shared_ptr<Territory>> territories = loader.generateTerritories(filePath);
	this->adjacencyMatrix = loader.generateConnectedTerritories(filePath, territories); //This is a map containing all the active territories for the game
	initializeContinentMap();
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

		//std::cout << "Current Node: " << currentNode->getName() << std::endl;

		//Boolean that tells us if there are any connected nodes that have not been traversed.
		bool newPaths = false; 
		for (std::shared_ptr<Territory> connectedTerritory : currentNode->getConnectedTerritories()) {

			//If statement for if the node has already been traversed
			if (std::count(traversedNodes.begin(), traversedNodes.end(), connectedTerritory) > 0) {
				//std::cout << "\t\t\t\t\t" << connectedTerritory->getName() << " has already been traversed, searching for a new node..." << std::endl;
				continue;
			}
			//If it has not been traversed
			else {
				searchHistory.push(connectedTerritory);
				traversedNodes.push_back(connectedTerritory);
				newPaths = true;
				//std::cout << "\t" << "Selected Node: " << connectedTerritory->getName() << std::endl;
				break;
			}
		}

		//Success condition (i.e. all nodes have been traversed by some path)
		if (traversedNodes.size() == numNodes) {
			std::cout << "\nTraversed Nodes: ";
			for (auto e : traversedNodes) {
				std::cout << e->getName() << ", ";
			}
			std::cout << std::endl;
			std::cout << "\nAll nodes have been traversed, meaning the graph is connected!" << std::endl;
			return true;
		}

		//If the currentnode is out of untraversed paths, pop it from the top of the stack
		if (!newPaths) { 
			//std::cout << "\t\tNo new paths... popping stack!" << std::endl;
			searchHistory.pop();
		}
	}

	std::cout << "Not all nodes were able to be traversed, meaning the graph is disjoint!" << std::endl;
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

		//std::cout << "Current Node: " << currentNode->getName() << std::endl;

		//Boolean that tells us if there are any connected nodes that have not been traversed.
		bool newPaths = false; 
		for (std::shared_ptr<Territory> connectedTerritory : currentNode->getConnectedTerritories()) {
			//If the connected territory is not in the current continent
			if (connectedTerritory->getContinent() != continentName) { 
				continue;
			}
			//If statement for if the node has already been traversed
			if (std::count(traversedNodes.begin(), traversedNodes.end(), connectedTerritory) > 0) {
				//std::cout << "\t\t\t\t\t" << connectedTerritory->getName() << " has already been traversed, searching for a new node..." << std::endl;
				continue;
			}
			//If it has not been traversed
			else {
				searchHistory.push(connectedTerritory);
				traversedNodes.push_back(connectedTerritory);
				newPaths = true;
				//std::cout << "\t" << "Selected Node: " << connectedTerritory->getName() << std::endl;

				break;
			}
		}

		//Success condition (i.e. all nodes have been traversed by some path)
		if (traversedNodes.size() == numNodesInContinent) {
			std::cout << "Traversed Nodes: ";
			for (auto e : traversedNodes) {
				std::cout << e->getName() << ", ";
			}
			std::cout << std::endl;
			std::cout << "All nodes have been traversed, meaning the continent graph is connected!" << std::endl;
			return true;
		}

		//If the currentnode is out of untraversed paths, pop it from the top of the stack
		if (!newPaths) {
			//std::cout << "\t\tNo new paths... popping stack!" << std::endl;
			searchHistory.pop();
		}
	}

	std::cout << "Not all nodes were able to be traversed, meaning the continent graph is disjoint!" << std::endl;
	return false;
}

bool Map::validate() {

	std::cout << "\nStarting graph validation for the entire map...\n\n" << std::endl;
	bool isConnectedGraph = isValidConnectedGraph();
	std::cout << "----------------------------------------------------------------------------------\n\n" << std::endl;

	if (!isConnectedGraph) {
		return false;
	}

	//Validating that each continent are connected subgraphs
	for (auto& continentPair : this->continentMap) {
		std::string continentName = continentPair.first;
		std::vector<std::shared_ptr<Territory>> continentNodes = continentPair.second;

		std::cout << "Starting subgraph validation for " << continentName << "...\n" << std::endl;
		bool result = continentIsValidSubgraph(continentNodes, continentName);
		std::cout << "----------------------------------------------------------------------------------" << std::endl;

		if (!result) {
			return result;
		}
	}

	std::cout << "\n\nStarting validation for every territory to belong to only one continent...\n" << std::endl;
	std::vector<std::shared_ptr<Territory>> checkedTerritories;
	for (auto& continentPair : this->continentMap) {
		std::string continentName = continentPair.first;
		std::vector<std::shared_ptr<Territory>> territoryList = continentPair.second;

		for (std::shared_ptr<Territory> territory : territoryList) {
			//If statement that checks to see if this territory has belonged to another continent
			if (std::count(checkedTerritories.begin(), checkedTerritories.end(), territory) > 0) {
				std::cout << "\t\t\t\t" << territory->getName() << " already exists in another continent!" << std::endl;
				return false;
			}
			std::cout << territory->getName() << " belongs to " << continentName << std::endl;
			checkedTerritories.push_back(territory);
		}
	}

	std::cout << "\nAll territories belong to a single continent!" << std::endl;
	std::cout << "----------------------------------------------------------------------------------" << std::endl;

	return true;
}

//-----------------------------------------------
// Territory Functions
//-----------------------------------------------

Map::Territory::Territory() {
	this->numArmies = 0;
}

Map::Territory::Territory(std::string name, std::string continent) {
	this->name = name;
	this->continent = continent;
	this->numArmies = 0;
}

void Map::Territory::addConnection(std::shared_ptr<Territory> territory) {
	this->connectedTerritories.push_back(territory);
}

void Map::Territory::setOwnership(std::string owner) {
	this->ownedBy = owner;
}

std::string Map::Territory::getName() {
	return this->name;
}

std::string Map::Territory::getContinent() {
	return this->continent;
}

std::vector <std::shared_ptr<Map::Territory>> Map::Territory::getConnectedTerritories() {
	return this->connectedTerritories;
}

std::string Map::Territory::getOwnership() {
	return this->ownedBy;
}

void Map::Territory::printTerritory() {

	std::cout << "Territory Name: " << this->name << std::endl;
	std::cout << "Continent: " << this->continent << std::endl;
	std::cout << "Owned by: " << this->ownedBy << std::endl;
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

bool Map::MapLoader::validateFile(std::string filePath) {

	std::ifstream file(filePath);

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

/**
*
* @brief The function generates a list of the territories used by the map.
*
* This function is the first step in loading our map. It parses through the entire file and creates instances
* of each territory (our nodes). This allows us to be able to reference these instances when creating our adjacency matrix,
* and thus physically connecting these nodes to one another.
*
* @param filePath The path to the file to be read
*
* @return Returns a vector that contains the references to all the territories (our nodes) that will be used by the map.
*/
std::vector<std::shared_ptr<Map::Territory>> Map::MapLoader::generateTerritories(std::string filePath) {

	std::vector<std::shared_ptr<Territory>> territoriesVector;

	std::ifstream file(filePath);

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

/**
*
* @brief The function connects all the linked territories and returns the adjacency matrix for the map.
*
* This function must only be called after generateTerritories was called, as it relies on the vector that contains the territories.
* The function will parse through the file and find all the territories connected to a current territory, and connect them together.
* Then, it will return a map that represent an adjacency matrix that allows you to easily access each territory by name.
*
* @param filePath The path to the file to be read
* @param generatedTerritories The territory nodes generated by the generateTerritories method
*
* @return Returns a map that represents the adjacency matrix for the map.
*/
std::map<std::string, std::shared_ptr<Map::Territory>> Map::MapLoader::generateConnectedTerritories(std::string filePath, std::vector<std::shared_ptr<Map::Territory>> generatedTerritories) {

	std::map<std::string, std::shared_ptr<Map::Territory>> map;

	std::ifstream file(filePath);

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