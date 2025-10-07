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

// TODO: Implement the player logic here


Player::Player() {
    
    cout << "Player created" << endl;
	playerHand = make_shared<Hand>();
}

Player::Player(const Player& other) {
    cout << "Player copy constructor called" << endl;
    playerHand = other.playerHand;
    playerTerritories = other.playerTerritories;
    playerOrders = other.playerOrders;
}

list<std::shared_ptr<Map::Territory>> Player::toDefend() {

    cout << "Player toDefend method called" << endl;

    list<std::shared_ptr<Map::Territory>> defendList;

    std::shared_ptr<Map::Territory> t1 = make_shared<Map::Territory>("Test Territory One", "Test Continent One");
    std::shared_ptr<Map::Territory> t2 = make_shared<Map::Territory>("Test Territory Two", "Test Continent One");

    defendList.push_back(t1);
    defendList.push_back(t2);

    return defendList;
    

}
list<std::shared_ptr<Map::Territory>> Player::toAttack() {

    cout << "Player toAttack method called" << endl;
    
    list<std::shared_ptr<Map::Territory>> attackList;

    std::shared_ptr<Map::Territory> t1 = make_shared<Map::Territory>("Test Territory One", "Test Continent One");
    std::shared_ptr<Map::Territory> t2 = make_shared<Map::Territory>("Test Territory Two", "Test Continent One");

    attackList.push_back(t1);
    attackList.push_back(t2);

    return attackList;

}
void Player::issueOrder(shared_ptr<Order> o) {

    cout << "Player issueOrder method called" << endl;

    playerOrders.push_back(o);

    cout << "Order issued" << endl;

}

void Player::addTerritory(shared_ptr<Map::Territory> t) {
    cout << "Adding territory to player" << endl;
    playerTerritories.push_back(t);
}

void Player::removeTerritory(shared_ptr<Map::Territory> t) {
    cout << "Removing territory from player" << endl;
    playerTerritories.remove(t);
}

void Player::addCard(shared_ptr<Card> c) {
    cout << "Adding card to player" << endl;
    playerHand->addCard(c);
    
}

void Player::removeCard(shared_ptr<Card> c) {
    cout << "Removing card from player" << endl;
	Deck tempDeck;
	playerHand->useCard(c, tempDeck);
}

shared_ptr<Hand> Player::getHand() { 
    return playerHand; 
}

std::list<shared_ptr<Map::Territory>> Player::getTerritories() { 
    return playerTerritories; 
}

std::list<shared_ptr<Order>> Player::getOrders() { 
    return playerOrders; 
}

Player& Player::operator=(const Player& other) {
    
	cout << "Player copy assignment operator called" << endl;
    if (this != &other) {
        playerHand = other.playerHand;
        playerTerritories = other.playerTerritories;
        playerOrders = other.playerOrders;
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Player& p) {
    os << "Player Territories:" << endl;
    for (const auto& territory : p.playerTerritories) {
        if (territory) {
            os << "  " << territory << endl;
        }
        else {
            os << "  None" << endl;
        }
    }

    os << "Player Orders:" << endl;
    for (const auto& order : p.playerOrders) {
        if (order) {
            os << order << endl;
        }
        else {
            os << " None" << endl;
        }
    }

    os << "Player Hand:" << endl;
    if (p.playerHand) {
        auto handCards = p.playerHand->getHand();
        for (const auto& card : handCards) {
            if (card) {
                os << card << endl;
            }
            else {
                os << "  [null card]\n";
            }
        }
    }
    else {
        os << "  No Cards" << endl;
    }

    return os;
}