#include "map.h"
#include <iostream>
#include <memory>

void testLoadMaps();
//int main() {
//
//	testLoadMaps();
//
//	return 0;
//}

void testLoadMaps() {
	std::string map_directory = "./map_files/";

	//Valid File Structure
	std::shared_ptr<Map> asia = std::make_shared<Map>(map_directory + "Asia.map");
	std::shared_ptr<Map> asiaDisjoint = std::make_shared<Map>(map_directory + "AsiaDisjoint.map");
	std::shared_ptr<Map> asiaDisjointContinent = std::make_shared<Map>(map_directory + "AsiaDisjointContinent.map");
	std::shared_ptr<Map> atlantis = std::make_shared<Map>(map_directory + "Atlantis.map");
	std::shared_ptr<Map> africa = std::make_shared<Map>(map_directory + "Africa.map");

	//Create map with files that have a valid file structure
	std::map<std::string, std::shared_ptr<Map>> validFileMap = {
		{"Asia", asia},
		{"AsiaDisjoint", asiaDisjoint},
		{"AsiaDisjointContinent", asiaDisjointContinent},
		{"Atlantis", atlantis},
		{"Africa", africa},
	};

	//Non Valid File Structure
	std::shared_ptr<Map> asiaNoContinents = std::make_shared<Map>(map_directory + "AsiaNoContinents.map");
	std::shared_ptr<Map> asiaNoMap = std::make_shared<Map>(map_directory + "AsiaNoMap.map");
	std::shared_ptr<Map> asiaNoTerritories = std::make_shared<Map>(map_directory + "AsiaNoTerritories.map");

	//Validate that the valid structured files are connected graphs with connected continent subgraphs
	for (auto& pair : validFileMap) {
		std::string fileName = pair.first;
		std::cout << "Starting map validation: " << fileName << std::endl;
		bool result = pair.second->validate();
		if (result == true)
			std::cout << fileName << " is valid" << std::endl;
		else
			std::cout << fileName << " is invalid" << std::endl;
		std::cout << "----------------------------------------------------------------------------------" << std::endl;
	}
}