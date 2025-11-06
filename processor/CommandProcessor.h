#ifndef COMMAND_PROCESSOR
#define COMMAND_PROCESSOR

#include "Command.h"
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
    { "addplayer", { { StateID::MapValidated, StateID::PlayersAdded }, StateID::PlayersAdded } },
    { "gamestart", { { StateID::PlayersAdded }, StateID::AssignReinforcements } },
    { "issueorder", { { StateID::AssignReinforcements, StateID::IssueOrders }, StateID::IssueOrders } },
    { "issueordersend", { { StateID::IssueOrders }, StateID::ExecuteOrders } },
    { "execorder", { { StateID::ExecuteOrders }, StateID::ExecuteOrders } },
    { "endexecorders", { { StateID::ExecuteOrders }, StateID::AssignReinforcements } },
    { "win", { { StateID::ExecuteOrders }, StateID::Win } },
    { "replay", { { StateID::Win }, StateID::Start } },
    { "quit", { { StateID::Win }, StateID::End } }
};


class CommandProcessor {
    private:
        std::vector<Command*> commands;
    public:
        CommandProcessor();
        ~CommandProcessor();
        virtual void readCommand();
        virtual Command* getCommand();
        bool validate(Command* command, StateID state);
        void saveCommand(Command* command);
};
void runProcessor();
#endif