#include "FileCommandProcessorAdapter.h"

FileReader::FileReader(const std::string& fileName) : file(fileName) {
    if (!file.is_open()) {
        std::cerr << "Error: could not open file " << fileName << std::endl;
    }
}
FileReader::~FileReader() {
    file.close();
}
void FileReader::loadNextCommandFromFile() {
    std::string line;
    if (std::getline(file, line)) {
        if (line.empty()) {
            std::cout << "Empty line, skip.\n";
            return;
        }
        size_t spaceIndex = line.find(' ');
        std::string commandName;
        std::string argument;
        if (spaceIndex != std::string::npos) {
            commandName = line.substr(0, spaceIndex);
            argument = line.substr(spaceIndex + 1);
        } else {
            commandName = line;
        }
        Command* command = new Command(commandName, argument);
        commands.push_back(command);

        std::cout << "Loaded command: " << commandName;
        if (!argument.empty())
            std::cout << " " << argument;
        std::cout << std::endl;

        return;
    }
    std::cout << "No commands left in file.\n";
}

Command* FileReader::getCommand() {
    if(commands.empty()) {
        cout << "No commands left.";
        return nullptr;
    }
    Command* command = commands.front();
    commands.erase(commands.begin());
    return command;
}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(FileReader* fileReader, GameEngine* engine) : CommandProcessor(engine) {
    this-> fileReader = fileReader;
}
void FileCommandProcessorAdapter::readCommand() {
    fileReader->loadNextCommandFromFile();
}
Command* FileCommandProcessorAdapter::getCommand() {
    return fileReader->getCommand();
}