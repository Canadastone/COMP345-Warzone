#ifndef COMMAND
#define COMMAND
#include "../gameEngine/gameEngine.h"
#include <string>

using std::string;
class Command {
    private:
        string command;
        string effect;
        string argument; // for addmap  & addplayer (both have argument mapname or playername)
    public: 
        Command(string command, string argument);
        Command(const Command& otherCommand);
        Command& operator=(const Command& otherCommand);
        void saveEffect(const string& effect);
        string getCommandName() const;
        string getArgument() const;
        string getEffect() const;
        StateID getTransitionState(StateID currentState);
};


#endif