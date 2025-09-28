#include "cards.h"
#include <iostream>
using namespace std;

void testCards() {

    cout << "Drawing from deck: \n" << endl;
    Deck deck;
    Hand hand;
    while ( deck.size() != 0 ) {
        deck.draw(hand);
    }
    cout << "Deck size after drawing is: " << deck.size() << endl;
    cout << "Playing cards in hand: \n" << endl;
    while ( hand.size() != 0 ) {
        shared_ptr<Card> card = *(hand.getHand().begin());
        hand.useCard(card, deck);
    }
    cout << "Deck size after playing is: " << deck.size() << endl;
};