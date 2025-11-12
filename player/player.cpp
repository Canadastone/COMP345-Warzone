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
#include <vector>

using namespace std;
using namespace orders;

static shared_ptr<Map::Territory> promptPick(const string& prompt, const vector<shared_ptr<Map::Territory>>& list) {
    if (list.empty()) { 
        cout << "No territories available." << endl; 
        return nullptr; 
    }

    cout << "\n" << prompt << "\n";
    for (size_t i = 0; i < list.size(); ++i) {
        auto& t = list[i];
        std::cout << "  [" << i << "] " << t->getName()
                  << " (units=" << t->getUnits() << ")\n";
    }
    cout << "Choose index : ";

    string line; 
    getline(cin, line);
    if (line.empty()) return nullptr;

    try {
        size_t idx = static_cast<size_t>(std::stoul(line));
        if (idx < list.size()) return list[idx];
    } catch (...) {}
    cout << "Invalid choice.\n";
    return nullptr;
}


static void printTerritoryList(string title, const std::list<std::shared_ptr<Map::Territory>>& terrList) {
    cout << title << ":" << endl;
    bool first = true;
    for (auto& t : terrList) {
        if (!first) std::cout << ", ";
        cout << t->getName() << endl;
        first = false;
    }
    cout << endl;
}


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

void Player::assignReinforcments(int numToAdd) {
    reinforcmentPool += numToAdd;
}

int Player::getReinforcementPool() const {
    return reinforcmentPool;
}

void Player::decrementReinforcementPool(int numToRemove) {
    reinforcmentPool -= numToRemove;
    if (reinforcmentPool < 0) {
        reinforcmentPool = 0; // Ensure it doesn't go negative
    }
}

list<std::shared_ptr<Map::Territory>> Player::toDefend() {

	list<std::shared_ptr<Map::Territory>> defendList;                   // list of territories to defend

	// Return all territories owned by the player in priority order
	// Priority: territories that are adjacent to enemy territories should be defended first
	for (auto& territory : playerTerritories) {
		defendList.push_back(territory);
	}

	return defendList;                                                  // returns list of territories to defend


}

list<std::shared_ptr<Map::Territory>> Player::toAttack() {

	list<std::shared_ptr<Map::Territory>> attackList;                   // list of territories to attack

	// Find all neighboring territories that are NOT owned by this player
	// These are potential attack targets
	for (auto& ownedTerritory : playerTerritories) {
		// Get all connected territories
		auto connectedTerritories = ownedTerritory->getConnectedTerritories();
		
		for (auto& neighbor : connectedTerritories) {
			// Check if this neighbor is NOT owned by the player
			bool isOwned = false;
			for (auto& playerTerritory : playerTerritories) {
				if (playerTerritory->getName() == neighbor->getName()) {
					isOwned = true;
					break;
				}
			}
			
			// If not owned and not already in attack list, add it
			if (!isOwned) {
				bool alreadyInList = false;
				for (auto& attackTarget : attackList) {
					if (attackTarget->getName() == neighbor->getName()) {
						alreadyInList = true;
						break;
					}
				}
				if (!alreadyInList) {
					attackList.push_back(neighbor);
				}
			}
		}
	}

	return attackList;                                                  // returns list of territories to attack

}

bool Player::issueOrder(const std::shared_ptr<Map>& map, Deck* deck) {

    cout << "Issuing an order to player" << endl;

    std::shared_ptr<Player> self = shared_from_this();


    // Get the list of territories to defend (player's own territories in priority)
    list<std::shared_ptr<Map::Territory>> defendList = toDefend();
    printTerritoryList("territories you must defend (territories you own)", defendList);

    // Get the list of territories to attack (neighboring enemy territories)
    list<std::shared_ptr<Map::Territory>> attackList = toAttack();
    printTerritoryList("territories you must attack (adjacent territories)", attackList);
    

    //priority deploy if you have reinforcements
    if(reinforcmentPool > 0){
        if (defendList.empty()) {
            std::cout << "No territories to defend. Skipping.\n";
            return false;
        }

        cout << "you have active reinforcements you must deploy" << endl;
        std::cout << "Choose target territory: ";
        std::string targetName; std::getline(std::cin, targetName);
        auto target = map->getTerritory(targetName);
        if (!target) { std::cout << "Invalid territory.\n"; return false; }

        std::cout << "How many units to deploy (max " << reinforcmentPool << "): ";
        int units = 0; std::cin >> units;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (units <= 0 || units > reinforcmentPool) {
            std::cout << "Invalid units.\n"; return false;
        }

        auto order = std::make_shared<orders::Deploy>(self, units, target);
        playerOrders.add(order);
        return true;
    }

    std::cout << "Enter order  (ADVANCE, BOMB, AIRLIFT, BLOCKADE, NEGOTIATE) or hit Enter to skip: ";
    std::string orderType;
    std::getline(std::cin, orderType);
    if (orderType.empty()) return false;

    // --- ADVANCE ---
    if (orderType == "ADVANCE") {
        if (defendList.empty()) { std::cout << "You own no territories. cannot advance\n"; return false; }

        std::cout << "Source (owned) territory name: ";
        std::string srcName; std::getline(std::cin, srcName);
        auto src = map->getTerritory(srcName);
        if (!src) { std::cout << "Invalid source.\n"; return false; }

        std::cout << "Target territory name (must be adjacent to source): ";
        std::string dstName; std::getline(std::cin, dstName);
        auto dst = map->getTerritory(dstName);
        if (!dst) { std::cout << "Invalid target.\n"; return false; }

        std::cout << "Units to move/attack with (available on " << srcName
                  << ": " << src->getUnits() << "): ";
        int units = 0; std::cin >> units;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (units <= 0) { std::cout << "Invalid units.\n"; return false; }

        auto order = std::make_shared<orders::Advance>(self, units, src, dst);
        playerOrders.add(order);
        return true;
    }
	std::cout << "Order issued successfully" << endl;

}

void Player::addTerritory(shared_ptr<Map::Territory> t) {

    cout << "Assigned territory: " << t->getName() << endl;

	playerTerritories.push_back(t); 								// adds the given territory to the player's list of territories

}

void Player::removeTerritory(shared_ptr<Map::Territory> t) {

    cout << "Removing territory from player" << endl;

	playerTerritories.remove(t); 			        			   // removes the given territory from the player's list of territories

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