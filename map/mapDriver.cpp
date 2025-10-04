#include "map.h"
#include <iostream>

int main() {

	Map test("AsiaDisjointContinent.map");
	bool result = test.validate();

	std::cout << result << std::endl;
	return 0;
}

void testLoadMaps() {

}