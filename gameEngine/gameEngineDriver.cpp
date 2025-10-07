#include <iostream>
#include "GameEngine.h"

using std::cout;
using std::cin;

/*
creates a console-driven interface that allows
the user to navigate through all the states by typing commands.
*/ 
void testGameStates() {
	//States are classes, transitions are triggered by void transitionState(StateID id), and commands are strings.
	GameEngine engine;

	cout << "\n========= Welcome to Risk/Warzone =========\n\n";
	engine.init();
	
	string userCommand{};

	while (true) {
		cout << "your command: ";
		cin >> userCommand;
		cout << "\n";
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
