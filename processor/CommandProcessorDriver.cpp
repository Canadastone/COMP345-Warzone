#include "CommandProcessor.h"
#include "../gameEngine/gameEngine.h"
#include "FileCommandProcessorAdapter.h"

void testCommandProcessor() {
    GameEngine* engine = new GameEngine();
    engine->init();  
    StateID currentState = engine->getState()->getID(); 
    string choice;
    cout << "Manual commands via console (-console) or commands from file (-file <filename>): " << std::endl;
    std::getline(cin, choice);
    if(choice == "-console") {
        CommandProcessor commandProcessor;
        auto pObserver = std::make_shared<LogObserver>("log.txt");
        commandProcessor.attach(pObserver);
        while(true) {
            commandProcessor.readCommand();   
            Command* cmd = commandProcessor.getCommand();
            if (cmd == nullptr) {
                std::cout << "No command read.\n";
                return;
            }
            if (cmd->getCommandName() == "quit" || cmd->getCommandName() == "exit") {
                std::cout << "Exiting command processor...\n";
                delete cmd;
                break;
            }

            bool validCommand = commandProcessor.validate(cmd, engine->getState()->getID());
            if(validCommand == 1) {
                StateID transitionTo = cmd->getTransitionState(currentState);
                engine->transitionState(transitionTo);
            }
            std::cout << "valid: " << validCommand << std::endl;
            
        }
    }
    else if(choice.rfind("-file", 0) == 0){
        int spaceIndex = choice.find(' ');
        if(spaceIndex == string::npos) {
            std::cerr << "Error, filename not provided" << std::endl;
        }
        else {
            string fileName = choice.substr(spaceIndex + 1);
            FileReader* fr = new FileReader("./processor/" + fileName);
            FileCommandProcessorAdapter fcp(fr);
            while(true) {
                fcp.readCommand();
                Command* cmd = fcp.getCommand();
                if(!cmd) break;
                bool isValid =  fcp.validate(cmd, engine->getState()->getID());
                cout << "Command is valid? " << isValid << std::endl;
                if(isValid) {
                    StateID transitionTo = cmd->getTransitionState(currentState);
                    engine->transitionState(transitionTo);
                }
            }
        }
    }


    delete engine;  // clean up
}
