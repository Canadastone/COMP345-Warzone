#include "cards.h"
#include <iostream>
using namespace std;

void testCards() {

    cout << "Drawing from deck: \n" << endl;
    Deck deck;
    Hand hand;
    // draw every card from the deck into hand
    while ( deck.size() != 0 ) {
        deck.draw(hand);
    }
    // confirm deck is empty
    cout << "Deck size after drawing is: " << deck.size() << endl;
    cout << "Playing cards in hand: \n" << endl;
    // play all cards in the hand
    while ( hand.size() != 0 ) {
        shared_ptr<Card> card = *(hand.getHand().begin());
        hand.useCard(card, deck);
    }
    // confirm deck is replenished
    cout << "Deck size after playing is: " << deck.size() << endl;
};