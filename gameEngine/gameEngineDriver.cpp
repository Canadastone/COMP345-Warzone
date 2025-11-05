#include <iostream>
#include "../gameEngine/gameEngine.h"

using std::cout;
using std::cin;


void testStartupPhase() {

	GameEngine engine;

	cout << "\n========= Welcome to Risk/Warzone =========\n\n";
	
	CommandProcessor commandProcessor;
	engine.startupPhase(commandProcessor);

	

}
