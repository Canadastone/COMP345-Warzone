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
	auto p3 = std::make_shared<Player>();
	auto p4 = std::make_shared<Player>();
	auto p5 = std::make_shared<Player>();

    // Create two territories and connect them so they are adjacent.
    auto t1 = std::make_shared<Map::Territory>("Territory1", "ContinentA");
    auto t2 = std::make_shared<Map::Territory>("Territory2", "ContinentA");
	auto t3 = std::make_shared<Map::Territory>("Territory3", "ContinentB");
	auto t4 = std::make_shared<Map::Territory>("Territory4", "ContinentB");
	auto t5 = std::make_shared<Map::Territory>("Territory5", "ContinentC");
	auto t6 = std::make_shared<Map::Territory>("Territory6", "ContinentC");
	auto t7 = std::make_shared<Map::Territory>("Territory7", "ContinentD");

    // connect each other (bidirectional)
    t1->addConnection(t2);
    t2->addConnection(t1);
	t2->addConnection(t3);
	t3->addConnection(t2);
	t3->addConnection(t4);
	t4->addConnection(t3);
	t4->addConnection(t6);
	t6->addConnection(t4);
	t5->addConnection(t6);
	t6->addConnection(t5);
	t5->addConnection(t7);
	t7->addConnection(t5);

    // assign ownership: t1 -> p1, t2 -> p2
    t1->setOwnership(p1);
    p1->addTerritory(t1);

    t2->setOwnership(p2);
    p2->addTerritory(t2);

	t3->setOwnership(p3);
	p3->addTerritory(t3);

	t4->setOwnership(p4);
	p4->addTerritory(t4);
	t6->setOwnership(p4);
	p4->addTerritory(t6);

	t5->setOwnership(p5);
	p5->addTerritory(t5);
	t7->setOwnership(p5);
	p5->addTerritory(t7);

    // Attach strategies: p1 is Human (simulated), p2 is Cheater (automatic)
    p1->setStrategy(std::make_unique<HumanPlayer>(p1));
    p2->setStrategy(std::make_unique<CheaterPlayer>(p2));
	p3->setStrategy(std::make_unique<NeutralPlayer>(p3));
	p4->setStrategy(std::make_unique<AggressivePlayer>(p4));
	p5->setStrategy(std::make_unique<BenevolentPlayer>(p5));

    // Show initial ownership
    std::cout << "Before cheater issueOrder:\n";
    std::cout << "  " << t1->getName() << " owner: " << (t1->getOwnership() == p1 ? "Player1 " : "other") << '(' << p1->getStrategyName() << ")\n";
    std::cout << "  " << t2->getName() << " owner: " << (t2->getOwnership() == p2 ? "Player2 " : "other") << '(' << p2->getStrategyName() << ")\n";
	std::cout << "  " << t3->getName() << " owner: " << (t3->getOwnership() == p3 ? "Player3 " : "other") << '(' << p3->getStrategyName() << ")\n";
	std::cout << "  " << t4->getName() << " owner: " << (t4->getOwnership() == p4 ? "Player4 " : "other") << '(' << p4->getStrategyName() << ")\n";
	std::cout << "  " << t5->getName() << " owner: " << (t5->getOwnership() == p5 ? "Player5 " : "other") << '(' << p5->getStrategyName() << ")\n";
	std::cout << "  " << t6->getName() << " owner: " << (t6->getOwnership() == p4 ? "Player4 " : "other") << '(' << p4->getStrategyName() << ")\n";
	std::cout << "  " << t7->getName() << " owner: " << (t7->getOwnership() == p5 ? "Player5 " : "other") << '(' << p5->getStrategyName() << ")\n";



    // Prepare players map and auxiliary objects required by Player::issueOrder
    std::map<int, std::shared_ptr<Player>> players;
    players[1] = p1;
    players[2] = p2;
	players[3] = p3;
	players[4] = p4;
	players[5] = p5;
    std::shared_ptr<Map> map = nullptr;
    Deck deck;
    Deck* p_deck = &deck;

    // Call p2->issueOrder -- expected to trigger CheaterPlayer behavior and conquer adjacent enemy territories
    bool changed = p2->issueOrder(players, map, p_deck);

    std::cout << "\nCheater issued orders -> changed something: " << (changed ? "yes" : "no") << "\n";

   //Show ownership before switch
	std::cout << "Ownership before switch\n";
	std::cout << "	Player 4 is using the " << p4->getStrategyName() << " strategy.\n";

	p4->setStrategy(std::make_unique<BenevolentPlayer>(p4));


	std::cout << "Ownership after switch\n";
	std::cout << "	Player 4 is now using the " << p4->getStrategyName() << " strategy.\n";
}