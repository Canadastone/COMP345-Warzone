#pragma once
#include <list>
#include "../orders/orders.h"
#include "../map/map.h"
#include "../cards/cards.h"
using namespace std;

class Player{

    private:
        list<Map::Territory> *playerTerritories;
        list<Order> *playerOrders;
        Hand *playerHand;
    
    public:
        Player();
        Player(const Player& other);
        ~Player();
        void addTerritory(Map::Territory t);
        void removeTerritory(Map::Territory t);
        void addOrder(Order o);
        void removeOrder(Order o);
        void addCard(Card c);
        void removeCard(Card c);
        list<Map::Territory> toDefend();
        list<Map::Territory> toAttack();
        void issueOrder();
        list<Map::Territory>* getTerritories();
        list<Order>* getOrders();
        Hand* getHand();
        Player& operator=(const Player& p);
        friend ostream& operator<<(ostream& os, const Player& p);


};