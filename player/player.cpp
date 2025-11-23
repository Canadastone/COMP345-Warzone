/*
* Assignment 1 - COMP 345
* Authors: Ariberto Bello Veras, Joshua Bitton, Liam Daigle,
* Ash Stone, Cyrus Stonebanks, Kirill Vishnyakov
* Date: 2025-10-07
* Description: This file contains the implementation of the player and their available moves
*/

#include "player.h"
#include "../orders/orders.h"
#include "../cards/cards.h"
#include <map>
#include <iostream>
#include <list>
#include <vector>
#include <limits>

using namespace std;
using namespace orders;

//issue order helpers

static void printTerritoryList(string title, const list<std::shared_ptr<Map::Territory>>& terrList) {
    cout << title << ":" << endl;
    bool first = true;
    for (auto& t : terrList) {
        if (!first) std::cout << ", ";
        cout << t->getName() << endl;
        first = false;
    }
    cout << endl;
}

static void printHand(Player& player){
    if(player.getHand()->getHand().empty()){
        cout << "your hand is empty" << endl;
    } else {
        cout << "your hand: " << endl; 
        for(const auto& card : player.getHand()->getHand()){
            cout << card->getCardType() << " CARD" << endl;
        }
    }  
}

static shared_ptr<Map::Territory> promptPickTerritory(const string& prompt, const list<shared_ptr<Map::Territory>>& terrList) {
    cout << prompt;
    string terrName; 
    getline(cin, terrName);
    for(auto map : terrList){
        if(terrName == map->getName()){
            return map;
        }
    }
    std::cout << "Invalid territory.\n"; 
    return promptPickTerritory(prompt, terrList);
}

static int promptPickInteger(string prompt, int lowerBound, int upperBound){
    cout << prompt << "(max " << upperBound-1 <<", min " << lowerBound <<"): ";
    int picked = upperBound; 
    std::cin >> picked;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (picked < lowerBound || picked >= upperBound) {
        std::cout << "Invalid try again" << endl; 
        return promptPickInteger(prompt, lowerBound, upperBound);
    }
    return picked;
}

static std::shared_ptr<Player>promptPickPlayer(const std::string& prompt, const std::map<int, std::shared_ptr<Player>>& players, const std::shared_ptr<Player>& self){
    std::cout << "Players:\n";
    for (const auto& [id, p] : players) {
        std::cout << "  [" << id << "]"
                  << (p.get() == self.get() ? " (you)" : "")
                  << "\n";
    }

    std::cout << prompt;
    int pid = -1;
    if (!(std::cin >> pid)) {
        std::cin.clear();
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    auto it = players.find(pid);
    if (it == players.end() || it->second.get() == self.get()) {
        std::cout << "Invalid selection. Try again.\n";
        return promptPickPlayer(prompt, players, self);
    }
    return it->second;
}

static int pickOrderType(bool reinforcementLock){
    //TODO cant skip, must add functionality
    string OrderTypePrompt = "Enter order number (DEPLOY [1], ADVANCE [2], BOMB [3], AIRLIFT [4], BLOCKADE [5], NEGOTIATE [6]) or [0] to skip: " ;
    int pick =  promptPickInteger(OrderTypePrompt, 0, 7);
    if(reinforcementLock && (pick != 1 && pick != 0)){
        cout << "you must choose DEPLOY [1], since you have units in your reinforcement poool" << endl;
        return pickOrderType(reinforcementLock);
    }
    return pick;
}

static shared_ptr<orders::Deploy> promptDeployOrder(const shared_ptr<Player>& player, const list<shared_ptr<Map::Territory>>& defendList, int reinforcementPool, int committedReinforcements){
    
    
    int availableReinforcements = reinforcementPool - committedReinforcements;  
    string pickterritoryPrompt =  "Choose a territory you own to deploy units: ";
    shared_ptr<Map::Territory> target = promptPickTerritory(pickterritoryPrompt, defendList);
    
    string pickUnitsPrompt  = "How many units to deploy: ";
    int units = promptPickInteger(pickUnitsPrompt, 0, availableReinforcements+1); 
    
    auto order = make_shared<orders::Deploy>(player, units, target);
    player->commitReinforcements(units);
    return order;
}

static shared_ptr<orders::Advance> promptAdvanceOrder(const shared_ptr<Player>& player, const list<shared_ptr<Map::Territory>>& defendList, const list<shared_ptr<Map::Territory>>& attackList){

    string pickSourcePrompt = "From where will you advance your units (Source territory name): ";
    shared_ptr<Map::Territory> src = promptPickTerritory(pickSourcePrompt, defendList);

    string pickTargetPrompt = "Target territory name (must be adjacent to source): ";
    shared_ptr<Map::Territory> target = promptPickTerritory(pickTargetPrompt, attackList);

    string pickUnitsCount = "How many units to move/attack with?: ";
    int units = promptPickInteger(pickUnitsCount, 1, src->getUnits()+1);

    auto order = std::make_shared<orders::Advance>(player, units, src, target);
    return order;
}

static shared_ptr<orders::Bomb> promptBombOrder(shared_ptr<Player> player, const list<shared_ptr<Map::Territory>>& attackList, Deck* deck){

    for(const auto& card : player->getHand()->getHand()){
        if(card->getCardType() == CardType::BOMB){
            string pickTargetPrompt = "Target territory name (must be adjacent to an owned territory): ";
            shared_ptr<Map::Territory> target = promptPickTerritory(pickTargetPrompt, attackList);
            auto order = make_shared<orders::Bomb>(player, target, deck, card);
            return order;
        }
    }
    cout << "Error finding bomb card... aborting!";
    return nullptr;   
}      

static shared_ptr<orders::Airlift> promptAirliftOrder(shared_ptr<Player> player, const list<shared_ptr<Map::Territory>>& defendList, Deck* deck){
    
    for(const auto& card : player->getHand()->getHand()){
        if(card->getCardType() == CardType::AIRLIFT){
            
            string pickSourcePrompt = "From where will you airlift your units (Source territory name): ";
            shared_ptr<Map::Territory> src = promptPickTerritory(pickSourcePrompt, defendList);

            string pickTargetPrompt = "where will the airlift land (target territory name): ";
            shared_ptr<Map::Territory> target = promptPickTerritory(pickTargetPrompt, defendList);

            string pickUnitsCount = "How many units to move with?: ";
            int units = promptPickInteger(pickUnitsCount, 1, src->getUnits()+1);

            auto order = make_shared<orders::Airlift>(player, units, src, target, deck, card);
            return order;
        }
    }
    cout << "Error finding airlift card... aborting!";
    return nullptr;  
}

static shared_ptr<orders::Blockade> promptBlockadeOrder(shared_ptr<Player> issuer, const list<shared_ptr<Map::Territory>>& defendList, Deck* deck){
    for(const auto& card : issuer->getHand()->getHand()){
        if(card->getCardType() == CardType::BLOCKADE){
            
            string pickSourcePrompt = "What territory will you blockade (must belong to you): ";
            shared_ptr<Map::Territory> target = promptPickTerritory(pickSourcePrompt, defendList);

            auto order = make_shared<orders::Blockade>(issuer, target, deck, card);
            return order;
        }
    }
    cout << "Error finding blockade card... aborting!";
    return nullptr;
}

static shared_ptr<orders::Negotiate> promptNegotiateOrder(shared_ptr<Player> issuer, const map<int, shared_ptr<Player>>& players, Deck* deck){

    for(const auto& card : issuer->getHand()->getHand()){
        if(card->getCardType() == CardType::DIPLOMACY){
            
            string pickAllyPrompt = "which player would you like to avoid conflicts with this next round: ";
            shared_ptr<Player> target = promptPickPlayer(pickAllyPrompt, players, issuer);

            auto order = make_shared<orders::Negotiate>(issuer, target, deck, card);
            return order;
        }
    }
    cout << "Error finding Diplomacy card... aborting!";
    return nullptr;
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

void Player::commitReinforcements(int unitsToCommit){
    committedReinforcements+=unitsToCommit;
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

bool Player::issueOrder(const map<int, shared_ptr<Player>>& players, const std::shared_ptr<Map>& map, Deck* deck) {

    cout << "Issuing an order to player" << endl;
    std::shared_ptr<Player> self = shared_from_this();
    
    // Get the list of territories to defend (player's own territories in priority)
    list<std::shared_ptr<Map::Territory>> defendList = toDefend();
    if (defendList.empty()) {
        std::cout << "No territories to defend. Skipping.\n";
        return false;
    }
    printTerritoryList("territories you must defend (territories you own)", defendList);

    // Get the list of territories to attack (neighboring enemy territories)
    list<std::shared_ptr<Map::Territory>> attackList = toAttack();
    printTerritoryList("territories you must attack (adjacent territories)", attackList);
    
    //show the cards in the deck
    printHand(*self);

    bool reinforcementLock = false; 
    if(reinforcmentPool - committedReinforcements > 0){
        reinforcementLock = true;
        cout << "you have" << (reinforcmentPool-committedReinforcements)
            << " active reinforcements you must deploy on your territories" << endl;
    }

    shared_ptr<orders::Order> newOrder = nullptr;

    bool orderIssued = false;
    while(!orderIssued){
        switch(pickOrderType(reinforcementLock)){
            case 1: //(deploy)
                newOrder = promptDeployOrder(self, defendList, reinforcmentPool, committedReinforcements);
                orderIssued = true;
                break;
            case 2://(advance)
                newOrder = promptAdvanceOrder(self, defendList, attackList);
                orderIssued = true;
                break;
            case 3: //(bomb)
                if(!self->getHand()->contains(CardType::BOMB)){
                    cout << "Hand must contain a bomb card";
                    break;
                }
                newOrder = promptBombOrder(self, attackList, deck);
                orderIssued = true;
                break;
            case 4: //(airlift)
                if(!self->getHand()->contains(CardType::AIRLIFT)){
                    cout << "Hand must contain an airlift card";
                    break;
                }
                newOrder = promptAirliftOrder(self, defendList, deck);
                orderIssued = true;
                break;
            case 5: //(blockade)
                if(!self->getHand()->contains(CardType::BLOCKADE)){
                    cout << "Hand must contain a bomb card";
                    break;
                }
                newOrder = promptBlockadeOrder(self, defendList, deck);
                orderIssued = true;
                break;
            case 6: //(negotiate)
                if(!self->getHand()->contains(CardType::DIPLOMACY)){
                    cout << "Hand must contain a diplomacy card";
                    break;
                }
                newOrder = promptNegotiateOrder(self, players, deck);
                orderIssued = true;
                break;
            default:
                orderIssued = true;
                break;
        }
    }

    if(!newOrder) return false;

    playerOrders.add(newOrder); 
	std::cout << "Order issued successfully" << endl;
    return true;
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
