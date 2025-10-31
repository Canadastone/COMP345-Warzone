#ifndef COMMAND
#define COMMAND

#include <string>
using std::string;
class Command {
    private:
        string command;
        string effect;
    public: 
        Command(string command);
        Command(const Command& otherCommand);
        Command& operator=(const Command& otherCommand);
        void saveEffect(const string& effect);
        string getCommandName() const;
        string getEffect() const;
};


#endif