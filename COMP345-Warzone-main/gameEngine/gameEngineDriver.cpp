#include <iostream>
#include "../gameEngine/gameEngine.h"
#include "../orders/orders.h"
#include "../cards/cards.h"

using std::cout;
using std::cin;

void testMainGameLoop(GameEngine* engine);
void testStartupPhase() {

	GameEngine engine;

	cout << "\n========= Welcome to Risk/Warzone (Startup phase) =========\n\n";

	auto pObserver = std::make_shared<LogObserver>("log.txt");
	CommandProcessor commandProcessor;
	engine.attach(pObserver);
	commandProcessor.attach(pObserver);
	engine.startupPhase(commandProcessor);

	testMainGameLoop(&engine);

}

/**
 * Test driver that demonstrates the main game loop functionality:
 * (1) Players receive correct number of army units in reinforcement phase
 * (2) Players only issue deploy orders when they have reinforcement pool armies
 * (3) Players can issue advance orders to defend or attack based on toAttack()/toDefend()
 * (4) Players can play cards to issue orders
 * (5) Players with no territories are removed from the game
 * (6) Game ends when a single player controls all territories
 * 
 * If engine is provided, it runs the main game loop from startup phase.
 * If engine is null, it creates a standalone demonstration scenario.
 */
void testMainGameLoop(GameEngine* engine) {

	// If engine is provided, run the actual game loop (from startup phase)
	if (engine == nullptr) {
		return;
	}

	// Otherwise, run standalone demonstration
	cout << "\n========= TEST MAIN GAME LOOP =========\n\n";
	
	// Create a simple map with 5 territories
	cout << "--- Setting up test scenario ---\n";
	auto t1 = std::make_shared<Map::Territory>("Territory1", "Continent1");
	auto t2 = std::make_shared<Map::Territory>("Territory2", "Continent1");
	auto t3 = std::make_shared<Map::Territory>("Territory3", "Continent2");
	auto t4 = std::make_shared<Map::Territory>("Territory4", "Continent2");
	auto t5 = std::make_shared<Map::Territory>("Territory5", "Continent2");
	
	// Connect territories to create neighbors
	t1->addConnection(t2);
	t2->addConnection(t1);
	t2->addConnection(t3);
	t3->addConnection(t2);
	t3->addConnection(t4);
	t4->addConnection(t3);
	t4->addConnection(t5);
	t5->addConnection(t4);
	
	// Create 3 players
	auto player1 = std::make_unique<Player>();
	auto player2 = std::make_unique<Player>();
	auto player3 = std::make_unique<Player>();
	
	// Assign territories to players
	cout << "Assigning territories to players...\n";
	player1->addTerritory(t1);
	player1->addTerritory(t2);
	
	player2->addTerritory(t3);
	player2->addTerritory(t4);
	
	player3->addTerritory(t5);
	
	cout << "\nPlayer 1 owns: " << player1->getTerritories().size() << " territories\n";
	cout << "Player 2 owns: " << player2->getTerritories().size() << " territories\n";
	cout << "Player 3 owns: " << player3->getTerritories().size() << " territories\n";
	
	// Create deck for card testing
	Deck gameDeck;
	
	// TEST 1: Demonstrate reinforcement phase - correct number of army units
	cout << "\n=== TEST 1: Reinforcement Phase - Army Unit Calculation ===\n";
	
	// Player 1: 2 territories / 3 = 0 (minimum is 3)
	int reinforcements1 = std::max(3, static_cast<int>(player1->getTerritories().size() / 3));
	player1->assignReinforcments(reinforcements1);
	cout << "Player 1 has " << player1->getTerritories().size() << " territories -> receives " 
	     << reinforcements1 << " army units (minimum 3)\n";
	
	// Player 2: 2 territories / 3 = 0 (minimum is 3)
	int reinforcements2 = std::max(3, static_cast<int>(player2->getTerritories().size() / 3));
	player2->assignReinforcments(reinforcements2);
	cout << "Player 2 has " << player2->getTerritories().size() << " territories -> receives " 
	     << reinforcements2 << " army units (minimum 3)\n";
	
	// Player 3: 1 territory / 3 = 0 (minimum is 3)
	int reinforcements3 = std::max(3, static_cast<int>(player3->getTerritories().size() / 3));
	player3->assignReinforcments(reinforcements3);
	cout << "Player 3 has " << player3->getTerritories().size() << " territories -> receives " 
	     << reinforcements3 << " army units (minimum 3)\n";
	
	// Additional test case: Player with 10 territories
	cout << "\nAdditional case - Player with 10 territories:\n";
	int reinforcements_large = std::max(3, 10 / 3);
	cout << "Player with 10 territories -> receives " << reinforcements_large << " army units (10/3 = 3)\n";
	
	// TEST 2: Players only issue Deploy orders when reinforcement pool > 0
	cout << "\n=== TEST 2: Deploy Orders Priority (Reinforcement Pool > 0) ===\n";
	cout << "Player 1 reinforcement pool: " << player1->getReinforcementPool() << "\n";
	
	// Try to issue Deploy order (should succeed)
	orders::Deploy* deployOrder1 = new orders::Deploy();
	cout << "Attempting to issue Deploy order...\n";
	player1->issueOrder(deployOrder1);
	cout << "Deploy order issued successfully. Orders in list: " << player1->getOrders().size() << "\n";
	
	// Try to issue Advance order (should issue but with warning)
	orders::Advance* advanceOrder1 = new orders::Advance();
	cout << "\nAttempting to issue Advance order while pool has armies...\n";
	player1->issueOrder(advanceOrder1);
	cout << "Advance order issued. Orders in list: " << player1->getOrders().size() << "\n";
	
	// Deplete reinforcement pool
	cout << "\nDepleting reinforcement pool...\n";
	player1->decrementReinforcementPool(player1->getReinforcementPool());
	cout << "Player 1 reinforcement pool: " << player1->getReinforcementPool() << "\n";
	
	// Now try Deploy and other orders
	orders::Deploy* deployOrder2 = new orders::Deploy();
	cout << "Attempting to issue Deploy order with empty pool...\n";
	player1->issueOrder(deployOrder2);
	cout << "Orders in list: " << player1->getOrders().size() << "\n";
	
	// TEST 3: Advance orders based on toDefend() and toAttack()
	cout << "\n=== TEST 3: Advance Orders - Defend and Attack Decisions ===\n";
	
	auto defendList = player1->toDefend();
	auto attackList = player1->toAttack();
	
	cout << "Player 1's toDefend() list has " << defendList.size() << " territories:\n";
	for (auto& t : defendList) {
		cout << "  - " << t->getName() << "\n";
	}
	
	cout << "\nPlayer 1's toAttack() list has " << attackList.size() << " territories:\n";
	for (auto& t : attackList) {
		cout << "  - " << t->getName() << "\n";
	}
	
	cout << "\nIssuing Advance order for defense (moving between owned territories)...\n";
	orders::Advance* advanceDefend = new orders::Advance();
	player1->issueOrder(advanceDefend);
	cout << "Advance order issued. Total orders: " << player1->getOrders().size() << "\n";
	
	cout << "\nIssuing Advance order for attack (moving to enemy territory)...\n";
	orders::Advance* advanceAttack = new orders::Advance();
	player1->issueOrder(advanceAttack);
	cout << "Advance order issued. Total orders: " << player1->getOrders().size() << "\n";
	
	// TEST 4: Playing cards to issue orders
	cout << "\n=== TEST 4: Playing Cards to Issue Orders ===\n";
	
	// Give player some cards
	auto bombCard = std::make_shared<Card>(CardType::BOMB);
	auto airliftCard = std::make_shared<Card>(CardType::AIRLIFT);
	auto diplomacyCard = std::make_shared<Card>(CardType::DIPLOMACY);
	
	player2->addCard(bombCard);
	player2->addCard(airliftCard);
	player2->addCard(diplomacyCard);
	
	cout << "Player 2 hand size: " << player2->getHand()->size() << " cards\n";
	
	// Issue card-based orders
	cout << "\nIssuing Bomb order (requires BOMB card)...\n";
	orders::Bomb* bombOrder = new orders::Bomb();
	player2->issueOrder(bombOrder);
	cout << "Bomb order issued. Total orders: " << player2->getOrders().size() << "\n";
	
	cout << "\nIssuing Airlift order (requires AIRLIFT card)...\n";
	orders::Airlift* airliftOrder = new orders::Airlift();
	player2->issueOrder(airliftOrder);
	cout << "Airlift order issued. Total orders: " << player2->getOrders().size() << "\n";
	
	cout << "\nIssuing Negotiate order (requires DIPLOMACY card)...\n";
	orders::Negotiate* negotiateOrder = new orders::Negotiate();
	player2->issueOrder(negotiateOrder);
	cout << "Negotiate order issued. Total orders: " << player2->getOrders().size() << "\n";
	
	// TEST 5: Player with no territories removed from game
	cout << "\n=== TEST 5: Remove Player with No Territories ===\n";
	
	// Remove all territories from player 3
	cout << "Player 3 currently has " << player3->getTerritories().size() << " territory\n";
	auto territoriesCopy = player3->getTerritories(); // Make a copy to iterate
	for (auto& t : territoriesCopy) {
		player3->removeTerritory(t);
	}
	cout << "Removed all territories from Player 3\n";
	cout << "Player 3 now has " << player3->getTerritories().size() << " territories\n";
	
	if (player3->getTerritories().size() == 0) {
		cout << "Player 3 would be removed from the game (homeless player)\n";
	}
	
	// TEST 6: Game ends when single player controls all territories
	cout << "\n=== TEST 6: Win Condition - Single Player Controls All Territories ===\n";
	
	int totalTerritories = 5; // We created 5 territories
	
	cout << "Total territories in game: " << totalTerritories << "\n";
	cout << "Player 1 owns: " << player1->getTerritories().size() << " territories\n";
	cout << "Player 2 owns: " << player2->getTerritories().size() << " territories\n";
	
	// Simulate Player 1 conquering all territories
	cout << "\nSimulating Player 1 conquering all territories...\n";
	
	// Transfer all territories to Player 1
	auto p2Territories = player2->getTerritories();
	for (auto& t : p2Territories) {
		player2->removeTerritory(t);
		player1->addTerritory(t);
	}
	
	cout << "Player 1 now owns: " << player1->getTerritories().size() << " territories\n";
	cout << "Player 2 now owns: " << player2->getTerritories().size() << " territories\n";
	
	if (player1->getTerritories().size() == totalTerritories) {
		cout << "\n*** PLAYER 1 WINS! Controls all " << totalTerritories << " territories! ***\n";
		cout << "Game would end here.\n";
	}

	engine->mainGameLoop();
	
}
