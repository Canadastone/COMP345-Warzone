#include <iostream>
#include "../cards/cards.h"
#include "../gameEngine/gameEngine.h"
#include "../map/map.h"
#include "../orders/orders.h"
#include "../player/player.h"
#include "../processor/CommandProcessor.h"
#include "../logObserver/LoggingObserver.h"

void testLoggingObserver() {
    LogObserver observer("C:\\Users\\liamd\\Documents\\A2_Part5_Log\\log.txt");
    std::shared_ptr<LogObserver> pObserver = std::make_shared<LogObserver>(observer);

    orders::OrderList* testList = new orders::OrderList();
    testList->attach(pObserver);

    orders::Deploy* testDeploy = new orders::Deploy();
    orders::Advance* testAdvance = new orders::Advance();
    orders::Bomb* testBomb = new orders::Bomb();
    orders::Blockade* testBlockade = new orders::Blockade();
    orders::Airlift* testAirlift = new orders::Airlift();
    orders::Negotiate* testNegotiate = new orders::Negotiate();

    testList->add(testDeploy);
    testList->add(testAdvance);
    testList->add(testBomb);
    testList->add(testBlockade);
    testList->add(testAirlift);
    testList->add(testNegotiate);

    for (int i = 0; i < testList->size(); i++) {
        (*testList)[i]->execute();
    }

    //Run this last for testing purposes
    GameEngine engine;
    engine.attach(pObserver);

    cout << "\n========= Welcome to Risk/Warzone (Startup phase) =========\n\n";

    CommandProcessor commandProcessor;
    commandProcessor.attach(pObserver);
    engine.startupPhase(commandProcessor);
    engine.mainGameLoop();
}