#include "CommandProcessor.h"

CommandProcessor::~CommandProcessor() {
    for(Command* cmd : commands) {
        delete cmd;
    }
}

string CommandProcessor::readCommand() {
    string commandName;
    cout << "Enter command: " << std::endl;
    cin >> commandName;
    Command* command = new Command(commandName);
    commands.push_back(command);
}
Command* CommandProcessor::getCommand() {
    if(commands.empty()) {
        cout << "No commands left.";
        return nullptr;
    }
    Command* command = commands.front();
    commands.erase(commands.begin());
    return command;
}

bool CommandProcessor::validate(State* state, Command* command) {

}