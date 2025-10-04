#pragma once

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>


class Map
{

public:

	class Territory {

	private:

		std::string name, continent, ownedBy;

		int numArmies;

		std::vector<Territory> connectedTerritories;

	public:

		Territory();

		Territory(std::string, std::string);

		void addConnection(Territory);

		std::string getName();

		void printTerritory();

		void setOwnership(std::string);

		std::string getOwnership();
	};

	class MapLoader {

	public:

		std::map<std::string, Territory> generateConnectedTerritories(std::string, std::vector<Territory>);

		std::vector<Territory> generateTerritories(std::string);

	};

	Map(std::string);

	void printMap();

	Territory getTerritory(std::string);

	void validateMap();

private:

	std::map<std::string, Territory> adjacencyMatrix;

	std::vector<Territory> territories;
};


