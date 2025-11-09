#include "Command.h"
#include "CommandProcessor.h"

Command::Command(string command, string argument) {
    this->command = command;
    this->argument = argument;
}
Command::Command(const Command& command) {
    this->command = command.command;
    this->effect = command.effect;
    this->argument = command.argument;
}
// Not neccessary but just in case command stores pointers in future
Command& Command::operator=(const Command& otherCommand) {
    if(this != &otherCommand) {
        this->command = otherCommand.command;
        this->effect = otherCommand.effect;
        this->argument = otherCommand.argument;
    }
    return *this;
}
void Command::saveEffect(const string& effect) {
    std::cout << "Command's effect: " << effect << "\n";
    this->effect = effect;
    this->notify(*this);
}

string Command::getCommandName() const {
    return this->command;
}
string Command::getArgument() const {
    return this->argument;
}
string Command::getEffect() const {
    return this->effect;
}
StateID Command::getTransitionState(StateID currentState) {
    auto it = commandRules.find(command);
    if(it != commandRules.end()) {
        return it->second.transitionsTo;
    }
    else {
        cout << "Invalid command: " << command << std::endl;
        return currentState;
    }
}
//Implements ILoggable functions
std::string Command::stringToLog() const {
    std::string textToLog = "Command's effect: " + this->effect;
    return textToLog;
}
//Implements Subject functions
void Command::attach(std::shared_ptr<LogObserver> pObserver) {
    this->observer = pObserver;
}
void Command::detach() {
    this->observer = nullptr;
}
void Command::notify(ILoggable& loggable) const {
    this->observer->update(loggable);
}