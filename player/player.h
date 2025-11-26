#pragma once
#include <list>
#include "../orders/orders.h"
#include "../map/map.h"
#include "../cards/cards.h"
#include "../playerStrategies/PlayerStrategies.h"
using namespace std;

class PlayerStrategy;
class Player : public std::enable_shared_from_this<Player> {

private:
	int committedReinforcements;
	list<std::shared_ptr<Map::Territory>> playerTerritories;        // territories owned by the player
	orders::OrderList playerOrders;              					// orders issued by the player
	std::shared_ptr<Hand> playerHand;                               // the player's hand of cards

	//temporary reinforcmentPool (number of armies a player has in their pool)
	int reinforcmentPool;
	unique_ptr<PlayerStrategy> strategy;

public:
	Player();                                                       // default constructor
	Player(const Player& other);                                    // copy constructor
	void addTerritory(std::shared_ptr<Map::Territory> t);           // adds a territory to the player's list of territories
	void removeTerritory(std::shared_ptr<Map::Territory> t);        // removes a territory from the player's list of territories
	void addCard(std::shared_ptr<Card> c);                          // adds a card to the player's hand
	void removeCard(std::shared_ptr<Card> c);                       // removes a card from the player's hand
	list<std::shared_ptr<Map::Territory>> toDefend();				// returns a list of territories the player has to defend
	list<std::shared_ptr<Map::Territory>> toAttack();				// returns a list of territories the player can attack
	bool issueOrder(const map<int, shared_ptr<Player>>& players, const std::shared_ptr<Map>& map, Deck* deck);
	void issueOrder(orders::Order* o);				// adds an order to the player's list of orders
	list<std::shared_ptr<Map::Territory>> getTerritories();			// returns the list of territories owned by the player
	orders::OrderList& getOrders();									// returns the list of orders issued by the player
	std::shared_ptr<Hand> getHand();								// returns the player's hand of cards
	Player& operator=(const Player& p);								// copy assignment operator
	friend ostream& operator<<(ostream& os, const Player& p);		// stream insertion operator
	void assignReinforcments(int numToAdd);							// assigns reinforcments to the reinforcmentPool
	void commitReinforcements(int unitsToCommit);
	int getReinforcementPool() const;								// returns the number of armies in the reinforcement pool
	int getCommitedReinforcements() const;
	void decrementReinforcementPool(int numToRemove);				// removes armies from the reinforcement pool
	void resetCommittedReinforcements();
	void setStrategy(unique_ptr<PlayerStrategy> strategy);
	void addOrder(std::shared_ptr<orders::Order> order);

};
void testPlayers();													// function to test the Player class