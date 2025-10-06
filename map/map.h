#pragma once

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <memory>
#include "../player/player.h"
class Player;


class Map
{

public:

	class Territory {

	private:

		std::string name, continent;

		std::shared_ptr<Player> owner;

		int numArmies;

		std::vector<std::shared_ptr<Territory>> connectedTerritories;

	public:

		/**
		*
		* @brief Constructs a default Territory instance
		*
		*/
		Territory();
		/**
		*
		* @brief Constructs a Territory with a given name and continent
		*
		* @param name The name of the Territory
		* @param continent The name of the continent in which the Territory is located
		*/
		Territory(std::string, std::string);
		/**
		*
		* @brief Copy Constructor for Territory
		*
		* @param the Territory instance that will have its attributes copied over
		*
		*/
		Territory(const Territory&);
		/**
		*
		* @brief Copy Assignment Operator for Territory
		*
		* @param territory	the Territory instance that will have its attributes copied over to the calling instance
		*
		*/
		Territory& operator=(const Territory&);
		/**
		*
		* @brief Stream Insertion Operator for Territory
		*
		* @param os Output Stream to be returned with information about the Territory
		* @param territory Territory that prints its information to the output stream
		* 
		* @return output stream containing information on the Territory instance
		*
		*/
		friend std::ostream& operator<<(std::ostream&, const Territory&);
		/**
		*
		* @brief Function that connects a Territory instance to another Territory instance
		*
		* This function is responsible for connecting the given Territory to the calling Territory
		*
		* @param territory a pointer to the Territory instance to be connected to the calling Territory
		*
		*/
		void addConnection(std::shared_ptr<Territory>);
		/**
		*
		* @brief Getter function for the name attribute
		*
		* Basic getter function that returns the name of the Territory
		*
		* @return a string that represents the name of the calling Territory
		*/
		std::string getName();
		/**
		*
		* @brief Getter function for the continent attribute
		*
		* Basic getter function to return the continent attribute
		*
		* @return a string that represents the name of the continent the Territory belongs to
		*/
		std::string getContinent();
		/**
		*
		* @brief Getter function for the connectedTerritories attribute
		*
		* Basic getter function that returns a vector of pointers to Territory objects that the calling Territory is connected to
		*
		* @return a vector of pointers to Territory objects
		*/
		std::vector<std::shared_ptr<Territory>> getConnectedTerritories();
		/**
		*
		* @brief The function that prints information about the Territory object that called it
		*
		* This function prints the Territory's information to the console
		* This information includes the name, continent, who it's owned by as well as all the connected territories to it
		*
		*/
		void printTerritory();
		/**
		*
		* @brief Setter function that sets the owner of the territory
		*
		* Basic setter function that sets the given Player as the owner of the calling Territory
		*
		* @param owner pointer to a Player instance who will own this territory
		*
		*/
		void setOwnership(std::shared_ptr<Player>);
		/**
		*
		* @brief Getter function for the ownedBy attribute
		*
		* Basic getter function that return who owns the Territory calling the method
		*
		* @return the Player that owns the Territory calling the method
		*/
		std::shared_ptr<Player> getOwnership();
	};

	class MapLoader {

	public:

		/**
		*
		* @brief Constructs a MapLoader instance given a filePath
		* 
		* @param filePath filePath representing a map file to be loaded
		*
		*/
		MapLoader(std::string);
		/**
		*
		* @brief Copy constructor of MapLoader
		*
		* @param mapLoader MapLoader instance that will have its attributes copied
		*
		*/
		MapLoader(const MapLoader&);
		/**
		*
		* @brief Copy Assignment Operator for MapLoader
		*
		* @param mapLoader MapLoader instance that will have its attributes copied
		*
		*/
		MapLoader& operator=(const MapLoader&);
		/**
		*
		* @brief Stream Insertion Operator for MapLoader
		*
		* @param os Output stream that will be returned when MapLoader is used with a stream insertion operator
		* @param mapLoader MapLoader instance that will print its name to the output stream
		*
		* @return output stream containing information on the MapLoader instance
		*/
		friend std::ostream& operator<<(std::ostream&, const MapLoader&);
		/**
		*
		* @brief The function connects all the linked territories and returns the adjacency matrix for the map.
		*
		* This function must only be called after generateTerritories was called, as it relies on the vector that contains the territories.
		* The function will parse through the file and find all the territories connected to a current territory, and connect them together.
		* Then, it will return a map that represent an adjacency matrix that allows you to easily access each territory by name.
		*
		* @param generatedTerritories The territory nodes generated by the generateTerritories method
		*
		* @return Returns a map that represents the adjacency matrix for the map.
		*/
		std::map<std::string, std::shared_ptr<Territory>> generateConnectedTerritories(std::vector<std::shared_ptr<Territory>>);
		/**
		*
		* @brief The function generates a list of the territories used by the map.
		*
		* This function is the first step in loading our map. It parses through the entire file and creates instances
		* of each territory (our nodes). This allows us to be able to reference these instances when creating our adjacency matrix,
		* and thus physically connecting these nodes to one another.
		*
		*
		* @return Returns a vector that contains the references to all the territories (our nodes) that will be used by the map.
		*/
		std::vector<std::shared_ptr<Territory>> generateTerritories();
		/**
		*
		* @brief The function that validates the file before parsing the data
		*
		* This function is responsible for ensuring the the .map file has all the necessary sections to be considered a valid .map file
		* This means it has 3 sections: [Map], [Continents] and [Territories]
		*
		*
		* @return returns a boolean value indicating if the file is valid or invalid (true or false respectively)
		*
		*/
		bool validateFile();

	private:

		std::string filePath;

	};

	/**
	*
	* @brief Constructs a Map provided a file path
	*
	* @param filePath File path of file to be read
	*
	*/
	Map(std::string);
	/**
	*
	* @brief Copy Constructor for Map
	*
	* @param map Map instance that will have its attributes copied over
	*
	*/
	Map(const Map&);
	/**
	*
	* @brief Copy Assignment Operator for Map
	*
	* @param map Map instance that will have its attributes copied over to the calling instance
	*
	*/
	Map& operator=(const Map&);
	/**
	*
	* @brief Stream Insertion Operator for Map
	*
	* @param os output stream that will be returned with information on the Map instance
	* @param map Map that will provide information to the output stream
	* 
	* @return output stream containing information about the Map
	*
	*/
	friend std::ostream& operator<<(std::ostream&, const Map&);
	/**
	*
	* @brief Function that prints all Territory instances of the map
	*
	*/
	void printMap();
	/**
	*
	* @brief Function that returns a Territory instance from the adjacency matrix
	*
	* This function takes the name provided to it and searches for it in the adjacency matrix.
	* If found, it returns the Territory instance
	*
	* @param territoryName String with the name of a Territory to be searched for
	*
	* @return a pointer to a Territory instance
	*/
	std::shared_ptr<Territory> getTerritory(std::string);
	/**
	*
	* @brief Function that validates whether the map graph meets expected criteria
	*
	* The map graph must:
	*	1) Be a connected graph
	*	2) Each continent must be a connected subgraph
	*	3) Each Territory must belong to one and only one continent
	*
	* @return a boolean value representing whether the graph was valid (true) or invalid (false)
	*/
	bool validate();

private:

	std::map<std::string, std::shared_ptr<Territory>> adjacencyMatrix;

	std::map<std::string, std::vector<std::shared_ptr<Territory>>> continentMap;

	std::string mapName;
	/**
	*
	* @brief Function that intializes the continent map
	*
	* This function creates/initializes the continent map to later be used to validate the Map
	* The continent map pairs the continent name to a vector of all the Territories in that contiennt.
	*
	*/
	void initializeContinentMap();
	/**
	*
	* @brief Function that retrieves the starting Territory instance
	*
	* This function gets a Territory instance from the adjacency matrix
	* This will later be used as a starting point to ensure the Map graph is a connected graph
	*
	* @return pointer to a Territory instance
	*/
	std::shared_ptr<Territory> getStartingTerritory();
	/**
	*
	* @brief Function that validates whether the given continent is a connected subgraph
	*
	* This function uses depth-first search to traverse the nodes and find a path where all nodes are reached,
	* proving that the continent is a valid connected subgraph
	*
	* @param territoryVector Vector consisting of all Territories in the given continent
	* @param continentName String representing the name of the continent being validated
	*
	* @return boolean value indicating if the continent is valid (true) or invalid (false)
	*/
	bool continentIsValidSubgraph(std::vector<std::shared_ptr<Territory>>, std::string);
	/**
	*
	* @brief Function that validates if the Map is a connected subgraph
	*
	* This function uses depth-first search to traverse the nodes and find a path where all nodes are reached,
	* proving that the entire map is a connected graph
	*
	* @return boolean value indicating if the map is a valid connected graph (true) or not (false)
	*/
	bool isValidConnectedGraph();

};


