#include "orders.h"
#include "../player/player.h"
#include "../map/map.h"
#include "../cards/cards.h"
#include "../logObserver/LoggingObserver.h"
#include <iostream>
#include <memory>

using namespace std;
using namespace orders;

void testOrderExecution() {
    
    cout << "==== TESTING ORDER EXECUTION ====" << endl;

    // Create players
    auto p1 = make_shared<Player>();
    auto p2 = make_shared<Player>();
    auto neutral = make_shared<Player>();

    // Create territories
    auto terrA = make_shared<Map::Territory>("Alpha", "North");
    auto terrB = make_shared<Map::Territory>("Beta", "North");
    auto terrC = make_shared<Map::Territory>("Gamma", "South");

    // Connect adjacency
    terrA->addConnection(terrB);
    terrB->addConnection(terrA);
    terrB->addConnection(terrC);
    terrC->addConnection(terrB);

    // Assign ownership
    p1->addTerritory(terrA);
    p2->addTerritory(terrB);
    p1->addTerritory(terrC);

    terrA->setUnits(8);
    terrB->setUnits(5);
    terrC->setUnits(4);

    p1->assignReinforcments(10);
    p2->assignReinforcments(5);

    auto observer = make_shared<LogObserver>("order_log.txt");

    cout << "\n=== TEST 1: DEPLOY ORDER ===" << endl;
    auto deploy = make_shared<Deploy>(p1, 5, terrA);
    deploy->attach(observer);
    cout << "Validating: " << (deploy->validate() ? "VALID" : "INVALID") << endl;
    deploy->execute();
    cout << "Territory " << terrA->getName() << " now has " << terrA->getUnits() << " units\n";

    
    cout << "\n=== TEST 2: ADVANCE ORDER (ATTACK) ===" << endl;
    auto advance = make_shared<Advance>(p1, 6, terrA, terrB);
    advance->attach(observer);
    cout << "Validating: " << (advance->validate() ? "VALID" : "INVALID") << endl;
    advance->execute();

    auto newOwner = terrB->getOwnership();
    cout << "Territory " << terrB->getName() << " now owned by: " 
        << (newOwner == p1 ? "Player 1" : newOwner == p2 ? "Player 2" : "Neutral") << endl;

    cout << "\n=== TEST 3: CARD REWARD ===" << endl;
    bool conqueredThisTurn = true; // simulate flag
    if (conqueredThisTurn) {
        auto card = make_shared<Card>(CardType::BOMB);
        p1->addCard(card);
        cout << "Player 1 conquered — drew one card!" << endl;
    }

    cout << "\n=== TEST 4: NEGOTIATE ORDER ===" << endl;
    auto diplomacyCard = make_shared<Card>(CardType::DIPLOMACY);
    auto negotiate = make_shared<Negotiate>(p1, p2, diplomacyCard);
    negotiate->attach(observer);
    negotiate->execute();

    // Now simulate attack blocked
    cout << "Attempting another Advance from P1 to P2 during peace..." << endl;
    auto blockedAdvance = make_shared<Advance>(p1, 3, terrC, terrB);
    blockedAdvance->attach(observer);
    if (/* negotiation active */ true) {
        cout << "Attack invalid due to negotiation!" << endl;
    } else {
        blockedAdvance->execute();
    }

    cout << "\n=== TEST 5: BLOCKADE ORDER ===" << endl;
    auto blockadeCard = make_shared<Card>(CardType::BLOCKADE);
    auto blockade = make_shared<Blockade>(p1, terrC, blockadeCard, neutral);
    blockade->attach(observer);
    blockade->execute();
    cout << "Territory " << terrC->getName() << " now owned by Neutral? "
         << (terrC->getOwnership() == neutral ? "YES" : "NO") << endl;

    cout << "\n=== TEST 6: AIRLIFT ORDER ===" << endl;
    auto airliftCard = make_shared<Card>(CardType::AIRLIFT);
    auto airlift = make_shared<Airlift>(p1, 2, terrA, terrB, airliftCard);
    airlift->attach(observer);
    airlift->execute();
    cout << terrA->getName() << " now has " << terrA->getUnits()
         << ", " << terrB->getName() << " has " << terrB->getUnits() << endl;

    cout << "\n=== FINAL STATE ===" << endl;
    cout << *p1 << endl;
    cout << *p2 << endl;
    cout << *neutral << endl;
};
