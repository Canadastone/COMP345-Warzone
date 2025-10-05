#include <list>
#include "orders/orders.h"
#include "map/map.h"
#include "cards/cards.h"

class Player{

    private:
        list<Territory> *playerTerritories;
        list<Order> *playerOrders;
        Hand *playerCards;
    
    public:
        Player();
        ~Player();
        void addTerritory(Territory t);
        void removeTerritory(Territory t);
        void addOrder(Order o);
        void removeOrder(Order o);
        void addCard(Card c);
        void removeCard(Card c);
        void toDefend();
        void toAttack();
        void issueOrder();

};