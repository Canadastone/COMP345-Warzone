/*
* Assignment 1 - COMP 345
* Authors: Ariberto Bello Veras, Joshua Bitton, Liam Daigle, 
* Ash Stone, Cyrus Stonebanks, Kirill Vishnyakov
* Date: 2025-10-07
* Description: This file contains the implementation of the player and their available moves
*/

#include "player.h"
#include <iostream>
#include <list>

using namespace std;

// TODO: Implement the player logic here


Player::Player() {
    
    cout << "Player created" << endl;
    playerHand = new Hand();
    playerTerritories = new list<Map::Territory>();
    playerOrders = new list<Order>();
}

Player::Player(const Player& other) {
    cout << "Player copy constructor called" << endl;
    playerHand = new Hand(*other.playerHand);
    playerTerritories = new list<Map::Territory>(*other.playerTerritories);
    playerOrders = new list<Order>(*other.playerOrders);
}

Player::~Player() {
    delete playerHand;
    delete playerTerritories;
    delete playerOrders;
}
list<Map::Territory> Player::toDefend() {

    cout << "Player toDefend method called" << endl;

    list<Map::Territory> defendList;

    Map::Territory *t1 = new Map::Territory("Test Territory One", "Test Continent One");
    Map::Territory *t2 = new Map::Territory("Test Territory Two", "Test Continent One");

    defendList.push_back(*t1);
    defendList.push_back(*t2);

    return defendList;
    

}
list<Map::Territory> Player::toAttack() {

    cout << "Player toAttack method called" << endl;
    
    list<Map::Territory> attackList;

    Map::Territory *t1 = new Map::Territory("Test Territory One", "Test Continent One");
    Map::Territory *t2 = new Map::Territory("Test Territory Two", "Test Continent One");

    attackList.push_back(*t1);
    attackList.push_back(*t2);

    return attackList;

}
void Player::issueOrder() {

    cout << "Player issueOrder method called" << endl;
    
    Order *o = new Order();

    playerOrders->push_back(*o);

    cout << "Order issued" << endl;

}

void Player::addTerritory(Map::Territory t) {
    cout << "Adding territory to player" << endl;
    playerTerritories->push_back(t);
}

void Player::removeTerritory(Map::Territory t) {
    cout << "Removing territory from player" << endl;
    playerTerritories->remove(t);
}

void Player::addCard(Card c) {
    cout << "Adding card to player" << endl;
    playerHand->addCard(std::make_shared<Card>(c));
    //TODO: add card to player's hand here
}

void Player::removeCard(Card c) {
    cout << "Removing card from player" << endl;
    //TODO: remove card from player's hand here
}

void Player::addOrder(Order o) {
    cout << "Adding order to player" << endl;
    playerOrders->push_back(o);
}

void Player::removeOrder(Order o) {
    cout << "Removing order from player" << endl;
    playerOrders->remove(o);
}

Hand* Player::getHand() { 
    return playerHand; 
}

std::list<Map::Territory>* Player::getTerritories() { 
    return playerTerritories; 
}

std::list<Order>* Player::getOrders() { 
    return playerOrders; 
}

Player& Player::operator=(const Player& other) {
    if (this != &other) {
        delete playerHand;
        delete playerTerritories;
        delete playerOrders;

        playerHand = new Hand(*other.playerHand);
        playerTerritories = new list<Map::Territory>(*other.playerTerritories);
        playerOrders = new list<Order>(*other.playerOrders);
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Player& p) {
    os << "Player with " << p.playerTerritories->size() << " territories: ";
    for(auto& territory : *p.playerTerritories) {
        os << territory.getName() << " ";
    }
    os << p.playerOrders->size() << " orders, and "
       << p.playerHand->size() << " cards in hand.";
    return os;
}