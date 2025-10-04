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

}
Player::~Player() {
    // Destructor logic if needed
}
void Player::toDefend() {
    cout << "Player toDefend method called" << endl;
    // Placeholder implementation
}
void Player::toAttack() {
    cout << "Player toAttack method called" << endl;
    // Placeholder implementation
}
void Player::issueOrder() {
    cout << "Player issueOrder method called" << endl;
    // Placeholder implementation
}

void Player::addTerritory(Territory t) {
    cout << "Adding territory to player" << endl;
    playerTerritories->push_back(t);
}

void Player::removeTerritory(Territory t) {
    cout << "Removing territory from player" << endl;
    playerTerritories->remove(t);
}

void Player::addCard(Card c) {
    cout << "Adding card to player" << endl;
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