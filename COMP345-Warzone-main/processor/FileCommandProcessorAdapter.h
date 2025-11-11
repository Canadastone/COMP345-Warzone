#ifndef FILE_COMMAND_PROCESSOR_ADAPTER
#define FILE_COMMAND_PROCESSOR_ADAPTER
#include "CommandProcessor.h"
#include "vector"
#include <fstream>

class FileReader {
    private:
        std::ifstream file;
        std::vector<Command*> commands;
    public:
        FileReader(const string& fn);
        ~FileReader();
        void loadNextCommandFromFile(); // read first command from file and add to commands vector
        Command* getCommand(); // pop first command from commands vector and return

};

class FileCommandProcessorAdapter : public CommandProcessor {
    private:
        FileReader* fileReader;
    public:
        FileCommandProcessorAdapter(FileReader* fileReader);
        void readCommand() override;
        Command* getCommand() override;
};

#endif