#include <iostream>
#include "../gameEngine/gameEngine.h"

using std::cout;
using std::cin;

void testMainGameLoop(GameEngine& engine);
void testStartupPhase() {
    LogObserver observer("log.txt");
    std::shared_ptr<LogObserver> pObserver = std::make_shared<LogObserver>(observer);

	orders::OrderList* testList = new orders::OrderList();
    testList->attach(pObserver);

	CommandProcessor commandProcessor;
	commandProcessor.attach(pObserver);

	GameEngine engine;
	engine.attach(pObserver);

	cout << "\n========= Welcome to Risk/Warzone (Startup phase) =========\n\n";
	

	engine.startupPhase(commandProcessor);

	testMainGameLoop(engine);

}
// I have to pass engine here because mainGame loop starts follows from the startup phase.
void testMainGameLoop(GameEngine& engine){

	engine.mainGameLoop();
}
