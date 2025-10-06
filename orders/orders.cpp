/*
* Assignment 1 - COMP 345
* Authors: Ariberto Bello Veras, Joshua Bitton, Liam Daigle, 
* Ash Stone, Cyrus Stonebanks, Kirill Vishnyakov
* Date: 2025-10-07
* Description: This file contains the implementation of the move order logic for the Warzone game.
*/

#include "orders.h"
#include <list>
#include <string>
#include <iostream>
using namespace std;

// TODO: Implement the player order logic here

Order::Order() {
    // Default constructor logic if needed
}

Order::~Order() {
    // Destructor logic if needed
}

std::ostream& operator<<(std::ostream& os, Order& order) {
    os << "Order Type: " << order.getType();
    return os;
}

void Order::setType(OrderType t) {
    type = t;
}

OrderType Order::getType() {
    return type;
}

void deploy::execute() {
    cout << "Executing deploy order" << endl;
    if (this->validate()) {
        cout << "Deploy order is valid" << endl;
    } else {
        cout << "Deploy order is invalid" << endl;
    }
}

bool deploy::validate() {
    cout << "Validating deploy order" << endl;
    return true;  // Placeholder return value
}

deploy::deploy() {
    setType(DEPLOY);
}

deploy::~deploy() {
    // Destructor logic if needed
}

void advance::execute() {
    cout << "Executing advance order" << endl;
    if (this->validate()) {
        cout << "Advance order is valid" << endl;
    } else {
        cout << "Advance order is invalid" << endl;
    }
}

bool advance::validate() {
    cout << "Validating advance order" << endl;
    return true;  // Placeholder return value
}

advance::advance() {
    setType(ADVANCE);
}

advance::~advance() {
    // Destructor logic if needed
}

void bomb::execute() {
    cout << "Executing bomb order" << endl;
    if (this->validate()) {
        cout << "Bomb order is valid" << endl;
    } else {
        cout << "Bomb order is invalid" << endl;
    }
}

bool bomb::validate() {
    cout << "Validating bomb order" << endl;
    return true;  // Placeholder return value
}

bomb::bomb() {
    setType(BOMB);
}

bomb::~bomb() {
    // Destructor logic if needed
}

void blockade::execute() {
    cout << "Executing blockade order" << endl;
    if (this->validate()) {
        cout << "Blockade order is valid" << endl;
    } else {
        cout << "Blockade order is invalid" << endl;
    }
}

bool blockade::validate() {
    cout << "Validating blockade order" << endl;
    return true;  // Placeholder return value
}

blockade::blockade() {
    setType(BLOCKADE);
}

blockade::~blockade() {
    // Destructor logic if needed
}

void airlift::execute() {
    cout << "Executing airlift order" << endl;
    if (this->validate()) {
        cout << "Airlift order is valid" << endl;
    } else {
        cout << "Airlift order is invalid" << endl;
    }
}

bool airlift::validate() {
    cout << "Validating airlift order" << endl;
    return true;  // Placeholder return value
}

airlift::airlift() {
    setType(AIRLIFT);
}

airlift::~airlift() {
    // Destructor logic if needed
}

void negotiate::execute() {
    cout << "Executing negotiate order" << endl;
    if (this->validate()) {
        cout << "Negotiate order is valid" << endl;
    } else {
        cout << "Negotiate order is invalid" << endl;
    }
}

bool negotiate::validate() {
    cout << "Validating negotiate order" << endl;
    return true;  // Placeholder return value
}

negotiate::negotiate() {
    setType(NEGOTIATE);
}

negotiate::~negotiate() {
    // Destructor logic if needed
}
