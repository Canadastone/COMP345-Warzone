#pragma once

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <memory>


class Map
{

public:

	class Territory {

	private:

		std::string name, continent, ownedBy;

		int numArmies;

		std::vector<std::shared_ptr<Territory>> connectedTerritories;

	public:

		Territory();

		Territory(std::string, std::string);

		void addConnection(std::shared_ptr<Territory>);

		std::string getName();

		std::string getContinent();

		std::vector<std::shared_ptr<Territory>> getConnectedTerritories();

		void printTerritory();

		void setOwnership(std::string);

		std::string getOwnership();
	};

	class MapLoader {

	public:

		std::map<std::string, std::shared_ptr<Territory>> generateConnectedTerritories(std::string, std::vector<std::shared_ptr<Territory>>);

		std::vector<std::shared_ptr<Map::Territory>> generateTerritories(std::string);

	};

	Map(std::string);

	void printMap();

	std::shared_ptr<Territory> getTerritory(std::string);

	std::shared_ptr<Territory> getStartingTerritory();

	bool validate();

private:

	std::map<std::string, std::shared_ptr<Territory>> adjacencyMatrix;

	std::map<std::string, std::vector<std::shared_ptr<Territory>>> continentMap;

	void initializeContinentMap();

	bool continentIsValidSubgraph(std::vector<std::shared_ptr<Territory>>, std::string);

	bool Map::isValidConnectedGraph();

};


