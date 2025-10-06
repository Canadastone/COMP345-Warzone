/*
* Assignment 1 - COMP 345
* Authors: Ariberto Bello Veras, Joshua Bitton, Liam Daigle, 
* Ash Stone, Cyrus Stonebanks, Kirill Vishnyakov
* Date: 2025-10-07
* Description: This file contains the implementation of the Card class.
*/

#include "cards.h"
#include <iostream>
using namespace std;

// TODO: Implement the Card object, the Deck and Hand classes and their respective methods
string cardTypeToString(CardType type) {
    switch(type) {
        case BOMB: return "Bomb";
        case REINFORCEMENT: return "Reinforcement";
        case BLOCKADE: return  "Blockade";
        case AIRLIFT: return "Airlift";
        case DIPLOMACY: return "Diplomacy";
        default: return "Unknown";
    }
}
// constructor for card
Card::Card(CardType type) {
    this -> type = new CardType(type);
}
Card::Card(const Card& otherCard) {
    this -> type = new CardType(*otherCard.type);
}
Card::~Card() {
    delete type;
}
CardType Card::getCardType() const {
    return *type;
}
void Card::play() {
    cout << "Playing " << cardTypeToString(*type) << " card" << endl;
    // after play, return to the deck
}

Deck::Deck() {
    // generate a deck
    vector<CardTypeQtyTemplate> deckConfiguration = {
        { CardType::AIRLIFT, 2 },
        { CardType::BLOCKADE, 1 },
        { CardType::BOMB, 1 },
        { CardType::DIPLOMACY, 1 },
        { CardType::REINFORCEMENT, 2 }
    };
    
    for(CardTypeQtyTemplate cardType : deckConfiguration) {
        for(int i = 0; i < cardType.qty; i++) {
            deck.push_back(make_shared<Card>(cardType.type));
        }
    }
}
// must remove the top card of the deck and add it to the hand
    int i = 1;
void Deck::draw(Hand& hand) {
    shared_ptr<Card> topCard;
    if(!deck.empty()) {
        shared_ptr<Card> top = deck.front();
        deck.erase(deck.begin());
        topCard = top;
    }
    cout << i << ". Drawing " << cardTypeToString(topCard -> getCardType()) << endl;
    hand.addCard(topCard);
    i++;
}

void Deck::returnCard(shared_ptr<Card> card) {
    deck.push_back(card);
}
int Deck::size() const {
    return deck.size();
}
Hand::Hand() { }

void Hand::addCard(shared_ptr<Card> card) {
    hand.insert(card);
}
// use card in hand, will call play and remove from the hand.
// card is also returned to the deck
void Hand::useCard(shared_ptr<Card> card, Deck& deck) {
    deck.returnCard(card);
    card -> play();
    hand.erase(card);
}
multiset<shared_ptr<Card>> Hand::getHand() {
    return hand;
}
int Hand::size() const {
    return hand.size();
}
