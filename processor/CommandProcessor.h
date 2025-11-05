#ifndef COMMAND_PROCESSOR
#define COMMAND_PROCESSOR

#include "Command.h"
#include "../gameEngine/gameEngine.h"
#include <string>
#include <vector>
#include <iostream>
#include <map>

using std::cout;
using std::cin;


struct CommandRule {
    std::vector<StateID> validInStates;
    StateID transitionsTo;
};

const std::map<std::string, CommandRule> commandRules = {
    { "loadmap", { { StateID::Start, StateID::MapLoaded }, StateID::MapLoaded } },
    { "validatemap", { { StateID::MapLoaded }, StateID::MapValidated } },
    { "addplayer", { { StateID::MapValidated }, StateID::PlayersAdded } },
    { "gamestart", { { StateID::PlayersAdded }, StateID::AssignReinforcements } },
    { "replay", { { StateID::Win }, StateID::Start } },
    { "quit", { { StateID::Win }, StateID::End } }
};


class CommandProcessor {
    private:
        GameEngine* engine;
        std::vector<Command*> commands;
    public:
        CommandProcessor(GameEngine* engine);
        ~CommandProcessor();
        virtual void readCommand();
        virtual Command* getCommand();
        bool validate(Command* command);
        void saveCommand(Command* command);
};
void runProcessor();
#endif