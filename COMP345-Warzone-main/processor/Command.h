#ifndef COMMAND
#define COMMAND
#include <string>
#include "../logObserver/LoggingObserver.h"

using std::string;


/*
Enum representing states, used as a parameter to the template classes below.
*/
enum class StateID {
	Start,
	MapLoaded,
	MapValidated,
	PlayersAdded,
	AssignReinforcements,
	IssueOrders,
	ExecuteOrders,
	Win,
	End
};

class Command : ILoggable, Subject{
    private:
        string command;
        string effect;
        string argument; // for addmap  & addplayer (both have argument mapname or playername)
        std::shared_ptr<LogObserver> observer;
    public: 
        Command(string command, string argument);
        Command(const Command& otherCommand);
        Command& operator=(const Command& otherCommand);
        void saveEffect(const string& effect);
        string getCommandName() const;
        string getArgument() const;
        string getEffect() const;
        StateID getTransitionState(StateID currentState);

        //Implements ILoggable functions
        std::string stringToLog() const;
        //Implements Subject functions
        void attach(std::shared_ptr<LogObserver> pObserver);
        void detach();
        void notify(ILoggable& loggable) const;
};


#endif