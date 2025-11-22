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
    committedReinforcements = 0;
    playerOrders = orders::OrderList();

    LogObserver observer("./log.txt");
	std::shared_ptr<LogObserver> obsPtr = std::make_shared<LogObserver>(observer);
    playerOrders.attach(obsPtr); 
}

Player::Player(const Player& other) {
    cout << "Player copy constructor called" << endl;
	playerHand = other.playerHand;                                      // copies the player's hand of cards
	playerTerritories = other.playerTerritories;                        // copies the player's list of territories
	playerOrders = other.playerOrders;                                  // copies the player's list of orders
    reinforcmentPool = other.reinforcmentPool;
    committedReinforcements = other.committedReinforcements;
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

void Player::resetCommittedReinforcements(){
    committedReinforcements = 0;
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
    if(reinforcmentPool - committedReinforcements > 0){
        if (defendList.empty()) {
            std::cout << "No territories to defend. Skipping.\n";
            return false;
        }

        cout << "you have active reinforcements you must deploy on your territries" << endl;
        std::cout << "Choose territory to deploy units: ";
        std::string targetName; std::getline(std::cin, targetName);
        auto target = map->getTerritory(targetName);
        if (!target) { std::cout << "Invalid territory.\n"; return false; }

        int availableReinforcements = reinforcmentPool - committedReinforcements;
        std::cout << "How many units to deploy (max " << availableReinforcements << "): ";
        int units = 0; std::cin >> units;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (units <= 0 || units > availableReinforcements) {
            std::cout << "Invalid units.\n"; return false;
        }

        auto order = std::make_shared<orders::Deploy>(self, units, target);
        committedReinforcements+=units;
        playerOrders.add(order);
        return true;
    }

    std::cout << "No reinforcements left, Enter order (ADVANCE, BOMB, AIRLIFT, BLOCKADE, NEGOTIATE) or hit Enter to skip: ";
    std::string orderType;
    std::getline(std::cin, orderType);
    if (orderType.empty()) return false;

    // --- ADVANCE ---
    if (orderType == "ADVANCE") {
        if (defendList.empty()) { std::cout << "You own no territories. cannot advance\n"; return false; }

        std::cout << "from where will you advance your units (Source territory name): ";
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
    if (orderType == "BOMB" || orderType == "AIRLIFT" || orderType == "BLOCKADE" || orderType == "NEGOTIATE"){
        if(playerHand->getHand().empty()){
            cout << "Hand is empty must have a card to call this order";
        }
    }
	std::cout << "Order issued successfully" << endl;

}

void Player::issueOrder(orders::Order* o) {

    cout << "Issuing an order to player" << endl;

	LogObserver observer("./log.txt");
	std::shared_ptr<LogObserver> obsPtr = std::make_shared<LogObserver>(observer);

    // Get the list of territories to defend (player's own territories in priority)
    list<std::shared_ptr<Map::Territory>> defendList = toDefend();

    // Get the list of territories to attack (neighboring enemy territories)
    list<std::shared_ptr<Map::Territory>> attackList = toAttack();
    
    // Priority 1: Deploy all armies from reinforcement pool first
    // As long as the player has armies in their reinforcement pool,
    // they should issue deploy orders and no other orders

    if (reinforcmentPool > 0) {

        // Prioritize Deploy orders while reinforcement pool is not empty
        if (o->getType() == orders::orderType::DEPLOY) {

            // Deploy to territories from toDefend() list
			playerOrders.attach(obsPtr); // Attach observer to the order
            playerOrders.add(std::shared_ptr<orders::Order>(o));
            
        } 
        
        else {

            // Player should deploy first, but still allow other orders
            cout << "Note: Reinforcement pool has " << reinforcmentPool << " armies. Consider deploying first." << endl;
			playerOrders.attach(obsPtr); // Attach observer to the order
            playerOrders.add(std::shared_ptr<orders::Order>(o));

        }
    } 
    
    else {
        
        // Priority 2: Once all armies are deployed, can issue other types of orders
        
        // Handle Advance orders
        if (o->getType() == orders::orderType::ADVANCE) {
            // Advance orders use toDefend() for moving armies between owned territories (defense)
            // or toAttack() for attacking enemy territories
			playerOrders.attach(obsPtr); // Attach observer to the order
            playerOrders.add(std::shared_ptr<orders::Order>(o));
        }

        // Handle card-based orders: BOMB, BLOCKADE, AIRLIFT, NEGOTIATE
        else {
			playerOrders.attach(obsPtr); // Attach observer to the order
            playerOrders.add(std::shared_ptr<orders::Order>(o));
        }
    }

	cout << "Order issued successfully" << endl;

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

void Player::setStrategy(unique_ptr<PlayerStrategy> strategy) {
    std::shared_ptr<Player> self = shared_from_this();
    this->strategy = std::move(strategy);
    this->strategy-> setPlayer(self);
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