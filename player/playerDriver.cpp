#include "player.h"
using namespace std;
#include <iostream>

void testPlayers();

int main(){
    cout << "Testing Player class" << endl;
    testPlayers();
};

void testPlayers(){
    
    Player p1, p2;

    Card c1, c2;
    Territory t1, t2;
    Order o1, o2;

    p1.addCard(Card());
    p1.addOrder(Order());
    p1.addTerritory(Territory());

    p2.addCard(Card());
    p2.addOrder(Order());
    p2.addTerritory(Territory());

};