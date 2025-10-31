#include "Command.h"

Command::Command(string command) {
    this->command = command;
}
Command::Command(const Command& command) {
    this->command = command.command;
    this->effect = command.effect;
}
// Not neccessary but just in case command stores pointers in future
Command& Command::operator=(const Command& otherCommand) {
    if(this != &otherCommand) {
        this->command = otherCommand.command;
        this->effect = otherCommand.effect;
    }
    return *this;
}

void Command::saveEffect(const string& effect) {
    this->effect = effect;
}

string Command::getCommandName() const {
    return this->command;
}
string Command::getEffect() const {
    return this->effect;
}
