#include "player.h"
using namespace std;
#include <iostream>
#include <memory>

void testPlayers();

//int main(){
//    cout << "Testing Player class" << endl;
//    testPlayers();
//};

void testPlayers(){
    
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

    // Add orders
    std::shared_ptr<Order> o1, o2;
    p1.addOrder(o1);
    p1.addOrder(o2);

    // Demonstrate toAttack and toDefend
    cout << "Player 1 toAttack and toDefend:" << endl;
    p1.toAttack();
    p1.toDefend();

    // Demonstrate issueOrder
    cout << "Calling issueOrder:" << endl;
    p1.issueOrder();

    // Print number of orders
    cout << "Player 1 has " << p1.getOrders().size() << " orders" << endl;

    // Print cards in hand
    cout << "Player 1 has " << p1.getHand()->size() << " cards in hand" << endl;

    // Print territories
    cout << "Player 1 has " << p1.getTerritories().size() << " territories" << endl;

};