#include "./gameEngine.h"

void testTournament(){
    
    //Initialize the processor, engine and observer
    auto pObserver = std::make_shared<LogObserver>("C:\\Users\\liamd\\Documents\\COMP345_A3\\log.txt");
    CommandProcessor cmdProcessor;
    GameEngine* engine = new GameEngine();
    engine->attach(pObserver);
    engine->init();

    //Validate the command exists:
    cmdProcessor.attach(pObserver);
    cmdProcessor.readCommand();
    Command* cmd = cmdProcessor.getCommand();
    std::cout << "Is the command valid:" << std::endl;
    bool isValid = cmdProcessor.validate(cmd, engine->getState()->getID());
    std::cout << std::boolalpha << isValid << std::endl;

    //Execute the tournament:
    std::string cmdArguments = cmd->getArgument();
    engine->tournamentMode(cmdArguments, *engine);
}