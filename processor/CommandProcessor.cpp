#include "CommandProcessor.h"

CommandProcessor::CommandProcessor() {}

CommandProcessor::~CommandProcessor() {
    for(Command* cmd : commands) {
        delete cmd;
    }
}

void CommandProcessor::readCommand() {
    string line;
    cout << "Enter command: " ;
    std::getline(cin, line);
    if(line.empty()) {
        cout << "No command entered.\n";
        return;
    }
    size_t spaceIndex = line.find(" ");
    string commandName;
    string argument;
    if(spaceIndex != string::npos) {
        commandName = line.substr(0, spaceIndex);
        argument = line.substr(spaceIndex + 1);
    }
    else {
        commandName = line;
    }
    Command* command = new Command(commandName, argument);
    saveCommand(command);
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

bool CommandProcessor::validate(Command* command, StateID currentState) {
    const string& commandName = command->getCommandName();
    auto iterator = commandRules.find(commandName);
    if(iterator == commandRules.end()) {
        // Not too sure about "a corresponding error message should be saved"
        command->saveEffect("Invalid command...");
        return false;
    }
    const CommandRule& rule = iterator->second;
    // if (!currentState) {
    //     std::cout << "Error: currentState is null.\n";
    //     return false;
    // }
    for(auto validState : rule.validInStates) {
        if(currentState == validState) {
            return true;
        }
    }
    // cout << "Command " << commandName << " not valid in current state." << std::endl;
    // command->saveEffect("Command not valid in current state.");
    return false;
}

void CommandProcessor::saveCommand(Command* command) {
    command->attach(this->observer);
    commands.push_back(command);
    this->notify(*this);
}

//Implements ILoggable functions
std::string CommandProcessor::stringToLog() const {
    Command& latestCommand = *this->commands.back();
    std::string latestCommandString = latestCommand.getCommandName();
    std::string textToLog = "Command saved: " + latestCommandString;

    return textToLog;
}
//Implements Subject functions
void CommandProcessor::attach(std::shared_ptr<LogObserver> pObserver) {
    this->observer = pObserver;
}
void CommandProcessor::detach() {
    this->observer = nullptr;
}
void CommandProcessor::notify(ILoggable& loggable) const {
    this->observer->update(loggable);
}