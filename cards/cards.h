#ifndef CARDS_H
#define CARDS_H

#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <memory>

using namespace std;

enum CardType { BOMB, REINFORCEMENT, BLOCKADE, AIRLIFT, DIPLOMACY };
string cardTypeToString(CardType type);



// used for generating a deck
struct CardTypeQtyTemplate {
    CardType type;
    int qty;
};
class Deck;
class Hand;

class Card : public enable_shared_from_this<Card> {
    private: 
        CardType type;
    public: 
        Card(CardType type);
        CardType getCardType() const;
        void play();
};

class Deck {
    private:
        vector<shared_ptr<Card>> deck;
    public:
        Deck(); // used to intialize the deck
        shared_ptr<Card> popCardFromDeck(); // after a card is draw from the deck, it must be removed
        void draw(Hand& hand); // when draw is called, a card from cards is assigned to given hand
        void returnCard(shared_ptr<Card> card); // after card is played, it is returned into the deck
        int size() const;
        
};
 
class Hand {
    private:
        multiset<shared_ptr<Card>> hand;
    public:
        Hand();
        void addCard(shared_ptr<Card> card);
        void useCard(shared_ptr<Card> card, Deck& deck);
        multiset<shared_ptr<Card>> getHand();
        int size() const;
};
void testCards();
#endif