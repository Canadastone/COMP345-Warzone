#include <iostream>
#include "../gameEngine/gameEngine.h"

using std::cout;
using std::cin;


void testStartupPhase() {

	GameEngine engine;

	cout << "\n========= Welcome to Risk/Warzone =========\n\n";
	engine.startupPhase();
	CommandProcessor commandProcessor;

	while (true) {

		commandProcessor.readCommand();          
		Command* cmd = commandProcessor.getCommand();
		bool isValidCommand = commandProcessor.validate(cmd, engine.getState()->getID());

		if (!isValidCommand) {
			cout << "Invalid command for State " << engine.getState()->getStateName() << ".\n";
			continue;
		}
		if (cmd->getCommandName() == "quit" || cmd->getCommandName() == "exit") {
			std::cout << "Exiting command processor...\n";
			delete cmd;
			break;
		}
		string effect = engine.getState()->onCommand(cmd, engine);
		cmd->saveEffect(effect);
		delete cmd;
	}
	
	

}
