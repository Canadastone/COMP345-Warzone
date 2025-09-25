#include <iostream>
#include "GameEngine.h"
#include <crtdbg.h>

#define _CRTDBG_MAP_ALLOC
//& 

using std::cout;
using std::cin;
void testGameStates(GameEngine&);

int main() {

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	{ 
		//States are classes, transitions are triggered by void transitionState(StateID id), and commands are strings.
		GameEngine GameEngine; 
	
		testGameStates(GameEngine);
		
	}
	
	_CrtDumpMemoryLeaks();
	return 0;
	
}


void testGameStates(GameEngine& engine) {
	
	cout << "\n========= Welcome to Risk/Warzone =========\n\n";
	engine.init();
	
	std::string userCommand{};

	while (true) {
		cout << "your command: ";
		cin >> userCommand;

		bool commandHandledSuccessfully = engine.getState()->onCommand(userCommand, engine);

		if (!commandHandledSuccessfully) {
			cout << "Invalid command for State " << engine.getState()->getStateName() << ".\n";
			continue;
		}
		if (userCommand == "end") {
			break;
		}
	}
	
	

}
