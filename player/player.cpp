/*
* Assignment 1 - COMP 345
* Authors: Ariberto Bello Veras, Joshua Bitton, Liam Daigle,
* Ash Stone, Cyrus Stonebanks, Kirill Vishnyakov
* Date: 2025-10-07
* Description: This file contains the implementation of the player and their available moves
*/

#include "player.h"
#include "../orders/orders.h"
#include <iostream>
#include <list>

using namespace std;

Player::Player() {

    cout << "Player created" << endl;                               
    playerHand = make_shared<Hand>();                                   // gives the player a hand of cards by default
    reinforcmentPool = 0;
}

Player::Player(const Player& other) {
    cout << "Player copy constructor called" << endl;
	playerHand = other.playerHand;                                      // copies the player's hand of cards
	playerTerritories = other.playerTerritories;                        // copies the player's list of territories
	playerOrders = other.playerOrders;                                  // copies the player's list of orders
    reinforcmentPool = other.reinforcmentPool;
}

int Player::getReinforcementPool(){
    return reinforcmentPool;
}

void Player::assignReinforcments(int numToAdd) {
    reinforcmentPool += numToAdd;
}
list<std::shared_ptr<Map::Territory>> Player::toDefend() {

	list<std::shared_ptr<Map::Territory>> defendList;                   // list of territories to defend

    std::shared_ptr<Map::Territory> t1 = make_shared<Map::Territory>("Test Territory One", "Test Continent One");
    std::shared_ptr<Map::Territory> t2 = make_shared<Map::Territory>("Test Territory Two", "Test Continent Two");
	std::shared_ptr<Map::Territory> t3 = make_shared<Map::Territory>("Test Territory Three", "Test Continent Three");

	defendList.push_back(t1);                                           // adds arbitrary territories to the defend list
    defendList.push_back(t2);
	defendList.push_back(t3);

	return defendList;                                                  // returns list of territories to defend


}
list<std::shared_ptr<Map::Territory>> Player::toAttack() {

	list<std::shared_ptr<Map::Territory>> attackList;                   // list of territories to attack

    std::shared_ptr<Map::Territory> t1 = make_shared<Map::Territory>("Test Territory One", "Test Continent One");
    std::shared_ptr<Map::Territory> t2 = make_shared<Map::Territory>("Test Territory Two", "Test Continent One");

	attackList.push_back(t1);                                           // adds arbitrary territories to the attack list
    attackList.push_back(t2);

	return attackList;                                                  // returns list of territories to attack

}
void Player::issueOrder(shared_ptr<orders::Order> o) {

    cout << "Issuing an order to player" << endl;

	playerOrders.add(o);                            // adds the given order to the player's list of orders

}

void Player::addTerritory(shared_ptr<Map::Territory> t) {

    cout << "Assigned territory: " << t->getName() << endl;

	playerTerritories.push_back(t); 								// adds the given territory to the player's list of territories
    t->setOwnership(shared_from_this());

}

void Player::removeTerritory(shared_ptr<Map::Territory> t) {

    cout << "Removing territory:" << t->getName() << endl;

	playerTerritories.remove(t); 			        			   // removes the given territory from the player's list of territories
    t->setOwnership(nullptr);
}

void Player::addCard(shared_ptr<Card> c) {

    cout << "Adding card to player" << endl;

	playerHand->addCard(c);                                        // adds the given card to the player's hand

}

void Player::removeCard(shared_ptr<Card> c) {

    cout << "Removing card from player" << endl;

    Deck tempDeck;
	playerHand->useCard(c, tempDeck);                               // removes the given card from the player's hand

}

shared_ptr<Hand> Player::getHand() {

	return playerHand;                                              // returns the player's hand of cards

}

std::list<shared_ptr<Map::Territory>> Player::getTerritories() {

	return playerTerritories;                                       // returns the list of territories owned by the player

}

orders::OrderList& Player::getOrders() {

	return playerOrders;                                            // returns the list of orders issued by the player

}

Player& Player::operator=(const Player& other) {

    cout << "Player copy assignment operator called" << endl;

	if (this != &other) {                                           // checks if not self-assignment
		playerHand = other.playerHand;                              // fills in info for the player
        playerTerritories = other.playerTerritories;
        playerOrders = other.playerOrders;
    }

	return *this;                                                   // returns a copy of the player

}

bool Player::operator==(const Player& p) const{
    return playerTerritories == p.playerTerritories && p.reinforcmentPool == reinforcmentPool;
    //TODO compare  order list and cards aswell
}

std::ostream& operator<<(std::ostream& os, const Player& p) {

    os << "Player Territories:" << endl;

	for (const auto& territory : p.playerTerritories) {             // iterates through the player's territories
        if (territory) {
            os << "  " << *territory << endl;
        }
        else {
            os << "  None" << endl;
        }
    }

    os << "Player Orders:" << endl;
    for(int i = 0; i < p.playerOrders.size(); i++){  // iterates through the player's orders   
        auto order = p.playerOrders[i];
        if (order) {
            os << *order << endl;
        }
        else {
            os << " None" << endl;
        }
    }

    os << "Player Hand:" << endl;

	if (p.playerHand) {                                             // checks if the player has a hand of cards

		auto handCards = p.playerHand->getHand();                   // gets the cards in the player's hand

		for (const auto& card : handCards) {                        // iterates through the cards in the player's hand
            if (card) {
                os << *card << endl;
            }
            else {
                os << "  [null card]\n";
            }
        }
    }
    else {
        os << "  No Cards" << endl;
    }

	return os;                                                      // returns the output stream describing the player
}