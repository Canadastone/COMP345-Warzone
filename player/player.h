#pragma once
#include <list>
#include "../orders/orders.h"
#include "../map/map.h"
#include "../cards/cards.h"
using namespace std;

class Player {

private:
    list<std::shared_ptr<Map::Territory>> playerTerritories;
    list<std::shared_ptr<orders::Order>> playerOrders;
    std::shared_ptr<Hand> playerHand;

public:
    Player();
    Player(const Player& other);
    void addTerritory(std::shared_ptr<Map::Territory> t);
    void removeTerritory(std::shared_ptr<Map::Territory> t);
    void addCard(std::shared_ptr<Card> c);
    void removeCard(std::shared_ptr<Card> c);
    list<std::shared_ptr<Map::Territory>> toDefend();
    list<std::shared_ptr<Map::Territory>> toAttack();
    void issueOrder(shared_ptr<orders::Order> o);
    list<std::shared_ptr<Map::Territory>> getTerritories();
    list<std::shared_ptr<orders::Order>> getOrders();
    std::shared_ptr<Hand> getHand();
    Player& operator=(const Player& p);
    friend ostream& operator<<(ostream& os, const Player& p);


};
void testPlayers();