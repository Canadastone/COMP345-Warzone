#ifndef COMMAND_PROCESSOR
#define COMMAND_PROCESSOR

#include "Command.h"
#include <string>
#include <vector>
#include <iostream>
using std::cout;
using std::cin;

class CommandProcessor {
    private:
        std::vector<Command*> commands;
    public:
        ~CommandProcessor();
        string readCommand();
        Command* getCommand();
        bool validate(State* state, Command* command);
        void saveCommand(Command* command);
};

#endif