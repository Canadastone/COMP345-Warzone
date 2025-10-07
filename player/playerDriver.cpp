#include "player.h"
using namespace std;
#include <iostream>
#include <memory>

void testPlayers(){

	cout << "Testing Player class" << endl;

  
    Player p1;

    // Add territories
    std::shared_ptr<Map::Territory> t1 = std::make_shared<Map::Territory>("Territory1", "Continent1");
    std::shared_ptr<Map::Territory> t2 = std::make_shared<Map::Territory>("Territory2", "Continent1");
    p1.addTerritory(t1);
    p1.addTerritory(t2);

    // Add cards
    std::shared_ptr<Card> c1 = std::make_shared<Card>(CardType::BOMB);
    std::shared_ptr<Card> c2 = std::make_shared<Card>(CardType::REINFORCEMENT);

    p1.addCard(c1);
    p1.addCard(c2);

	shared_ptr<Order> o1 = make_shared<Order>(orders::orderType::BOMB);
	shared_ptr<Order> o2 = make_shared<Order>(orders::orderType::AIRLIFT);
	shared_ptr<Order> o3 = make_shared<Order>(orders::orderType::NEGOTIATE);

    // Demonstrate issueOrder
    cout << "Calling issueOrder:" << endl;
    p1.issueOrder(o1);
	p1.issueOrder(o2);
	p1.issueOrder(o3);

    // Demonstrate toAttack and toDefend
    cout << "Player 1 toAttack and toDefend:" << endl;
    p1.toAttack();
    p1.toDefend();

    // Print number of orders
    cout << "Player 1 has " << p1.getOrders().size() << " orders" << endl;

    // Print cards in hand
    cout << "Player 1 has " << p1.getHand()->size() << " cards in hand" << endl;

    // Print territories
    cout << "Player 1 has " << p1.getTerritories().size() << " territories" << endl;

	Player p2; // Test copy assignment

    p2 = p1;

	Player p3(p1); // Test copy constructor

	// Print number of orders
    cout << "Player 2 has " << p2.getOrders().size() << " orders" << endl;

    // Print cards in hand
    cout << "Player 2 has " << p2.getHand()->size() << " cards in hand" << endl;

    // Print territories
    cout << "Player 2 has " << p2.getTerritories().size() << " territories" << endl;

	// Print number of orders
    cout << "Player 3 has " << p3.getOrders().size() << " orders" << endl;

    // Print cards in hand
    cout << "Player 3 has " << p3.getHand()->size() << " cards in hand" << endl;

    // Print territories
    cout << "Player 3 has " << p3.getTerritories().size() << " territories" << endl;

	cout << p1 << endl;

};