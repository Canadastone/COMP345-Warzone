#include <iostream>
#include "../cards/cards.h"
#include "../gameEngine/gameEngine.h"
#include "../map/map.h"
#include "../orders/orders.h"
#include "../player/player.h"
#include "../processor/CommandProcessor.h"
#include "../logObserver/LoggingObserver.h"

void testLoggingObserver() {
    /*LogObserver observer("log.txt");
    std::shared_ptr<LogObserver> pObserver = std::make_shared<LogObserver>(observer);

    orders::OrderList testList = orders::OrderList();
    testList.attach(pObserver);

    auto p1 = make_shared<Player>();
    auto p2 = make_shared<Player>();
    auto neutral = make_shared<Player>();

    auto t1 = make_shared<Map::Territory>("Russia", "Asia");
    auto t2 = make_shared<Map::Territory>("Morocco", "Africa");
    auto t3 = make_shared<Map::Territory>("Egypt", "Africa");

    p1->addTerritory(t1);
    p2->addTerritory(t2);
    t1->setUnits(5);
    t2->setUnits(3);

    auto bombCard = make_shared<Card>(CardType::BOMB);
    auto blockadeCard = make_shared<Card>(CardType::BLOCKADE);
    auto airliftCard = make_shared<Card>(CardType::AIRLIFT);
    auto diplomacyCard = make_shared<Card>(CardType::DIPLOMACY);

    auto testDeploy = make_shared<orders::Deploy>(p1, 3, t1);
    auto testAdvance = make_shared<orders::Advance>(p1, 2, t1, t2);
    auto testBomb = make_shared<orders::Bomb>(p1, t2, bombCard);
    auto testBlockade = make_shared<orders::Blockade>(p1, t1, blockadeCard, neutral);
    auto testAirlift = make_shared<orders::Airlift>(p1, 2, t1, t3, airliftCard);
    auto testNegotiate = make_shared<orders::Negotiate>(p1, p2, diplomacyCard);

    testList.add(testDeploy);
    testList.add(testAdvance);
    testList.add(testBomb);
    testList.add(testBlockade);
    testList.add(testAirlift);
    testList.add(testNegotiate);

    for (int i = 0; i < testList.size(); i++) {
        (testList)[i]->execute();
        cout << testList[i]->effect;
    }

    //Run this last for testing purposes
    GameEngine engine;
    engine.attach(pObserver);

    cout << "\n========= Welcome to Risk/Warzone (Startup phase) =========\n\n";

    CommandProcessor commandProcessor;
    commandProcessor.attach(pObserver);
    engine.startupPhase(commandProcessor);
    engine.mainGameLoop();*/
}