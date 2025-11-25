#include "PlayerStrategies.h"
#include "../player/player.h"
#include "../cards/cards.h"
#include "../map/map.h"
#include <memory>
#include <iostream>
#include <map>

void testPlayerStrategies() {
    // Create two players
    auto p1 = std::make_shared<Player>();
    auto p2 = std::make_shared<Player>();

    // Create two territories and connect them so they are adjacent.
    auto t1 = std::make_shared<Map::Territory>("Territory1", "ContinentA");
    auto t2 = std::make_shared<Map::Territory>("Territory2", "ContinentA");

    // connect each other (bidirectional)
    t1->addConnection(t2);
    t2->addConnection(t1);

    // assign ownership: t1 -> p1, t2 -> p2
    t1->setOwnership(p1);
    p1->addTerritory(t1);

    t2->setOwnership(p2);
    p2->addTerritory(t2);

    // Attach strategies: p1 is Human (simulated), p2 is Cheater (automatic)
    p1->setStrategy(std::make_unique<HumanPlayer>(p1));
    p2->setStrategy(std::make_unique<CheaterPlayer>(p2));

    // Show initial ownership
    std::cout << "Before cheater issueOrder:\n";
    std::cout << "  " << t1->getName() << " owner: " << (t1->getOwnership() == p1 ? "Player1" : "other") << "\n";
    std::cout << "  " << t2->getName() << " owner: " << (t2->getOwnership() == p2 ? "Player2" : "other") << "\n";

    // Prepare players map and auxiliary objects required by Player::issueOrder
    std::map<int, std::shared_ptr<Player>> players;
    players[1] = p1;
    players[2] = p2;
    std::shared_ptr<Map> map = nullptr;
    Deck deck;
    Deck* p_deck = &deck;

    // Call p2->issueOrder -- expected to trigger CheaterPlayer behavior and conquer adjacent enemy territories
    bool changed = p2->issueOrder(players, map, p_deck);

    std::cout << "\nCheater issued orders -> changed something: " << (changed ? "yes" : "no") << "\n";

    // Show ownership after cheater
    std::cout << "After cheater issueOrder:\n";
    std::cout << "  " << t1->getName() << " owner: "
              << (t1->getOwnership() == p2 ? "Player2 (cheater)" : (t1->getOwnership() == p1 ? "Player1" : "other")) << "\n";
    std::cout << "  " << t2->getName() << " owner: "
              << (t2->getOwnership() == p2 ? "Player2 (cheater)" : (t2->getOwnership() == p1 ? "Player1" : "other")) << "\n";

    // Demonstrate dynamic strategy change: switch p1 to Cheater at runtime and run again
    p1->setStrategy(std::make_unique<CheaterPlayer>(p1));
    bool changed2 = p1->issueOrder(players, map, p_deck);

    std::cout << "\nAfter switching Player1 to Cheater and issuing orders -> changed something: " << (changed2 ? "yes" : "no") << "\n";
    std::cout << "Final ownership:\n";
    std::cout << "  " << t1->getName() << " owner: "
              << (t1->getOwnership() == p1 ? "Player1" : (t1->getOwnership() == p2 ? "Player2" : "other")) << "\n";
    std::cout << "  " << t2->getName() << " owner: "
              << (t2->getOwnership() == p1 ? "Player1" : (t2->getOwnership() == p2 ? "Player2" : "other")) << "\n";
}