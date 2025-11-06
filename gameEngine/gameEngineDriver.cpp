#include <iostream>
#include "../gameEngine/gameEngine.h"

using std::cout;
using std::cin;

void testMainGameLoop(GameEngine& engine);
void testStartupPhase() {

	GameEngine engine;

	cout << "\n========= Welcome to Risk/Warzone (Startup phase) =========\n\n";
	
	CommandProcessor commandProcessor;
	engine.startupPhase(commandProcessor);

	testMainGameLoop(engine);

}
// I have to pass engine here because mainGame loop starts follows from the startup phase.
void testMainGameLoop(GameEngine& engine){

	engine.mainGameLoop();
}
