#include "PlayerStrategies.h"
#include "../player/player.h"
#include "../map/map.h"
#include <memory>
#include <stdio.h>
#include <limits>

void PlayerStrategy::setPlayer(std::shared_ptr<Player> player) {
    this->player = player;
}

PlayerStrategy& PlayerStrategy::operator=(const PlayerStrategy& ps) {

    if (this != &ps) {
        this->player = ps.player;
    }

    return *this;
}

static list<shared_ptr<Map::Territory>> getalladjacentTerritories(const shared_ptr<Player> player){
    list<std::shared_ptr<Map::Territory>> attackList;
    // Find all neighboring territories that are NOT owned by this player
    // These are potential attack targets
    for (auto& ownedTerritory : player->getTerritories()) {
        // Get all connected territories
        auto connectedTerritories = ownedTerritory->getConnectedTerritories();

        for (auto& neighbor : connectedTerritories) {
            // Check if this neighbor is NOT owned by the player
            bool isOwned = false;
            for (auto& playerTerritory : player->getTerritories()) {
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
    return attackList;  
}

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

static void printHand(Player& player) {
    if (player.getHand()->getHand().empty()) {
        cout << "your hand is empty" << endl;
    }
    else {
        cout << "your hand: " << endl;
        for (const auto& card : player.getHand()->getHand()) {
            cout << card->getCardType() << " CARD" << endl;
        }
    }
}

static int promptPickInteger(string prompt, int lowerBound, int upperBound) {
    cout << prompt << "(max " << upperBound - 1 << ", min " << lowerBound << "): ";
    int picked = upperBound;
    std::cin >> picked;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (picked < lowerBound || picked >= upperBound) {
        std::cout << "Invalid try again" << endl;
        return promptPickInteger(prompt, lowerBound, upperBound);
    }
    return picked;
}

static shared_ptr<Map::Territory> promptPickTerritory(const string& prompt, const list<shared_ptr<Map::Territory>>& terrList) {
    cout << prompt;
    string terrName;
    getline(cin, terrName);
    for (auto map : terrList) {
        if (terrName == map->getName()) {
            return map;
        }
    }
    std::cout << "Invalid territory.\n";
    return promptPickTerritory(prompt, terrList);
}

static std::shared_ptr<Player>promptPickPlayer(const std::string& prompt, const std::map<int, std::shared_ptr<Player>>& players, const std::shared_ptr<Player>& self) {
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

static int pickOrderType(bool reinforcementLock) {
    //TODO cant skip, must add functionality
    string OrderTypePrompt = "Enter order number (DEPLOY [1], ADVANCE [2], BOMB [3], AIRLIFT [4], BLOCKADE [5], NEGOTIATE [6]) or [0] to skip: ";
    int pick = promptPickInteger(OrderTypePrompt, 0, 7);
    if (reinforcementLock && (pick != 1 && pick != 0)) {
        cout << "you must choose DEPLOY [1], since you have units in your reinforcement poool" << endl;
        return pickOrderType(reinforcementLock);
    }
    return pick;
}

static shared_ptr<orders::Deploy> promptDeployOrder(const shared_ptr<Player>& player, const list<shared_ptr<Map::Territory>>& defendList, int reinforcementPool, int committedReinforcements) {


    int availableReinforcements = reinforcementPool - committedReinforcements;
    string pickterritoryPrompt = "Choose a territory you own to deploy units: ";
    shared_ptr<Map::Territory> target = promptPickTerritory(pickterritoryPrompt, defendList);

    string pickUnitsPrompt = "How many units to deploy: ";
    int units = promptPickInteger(pickUnitsPrompt, 0, availableReinforcements + 1);

    auto order = make_shared<orders::Deploy>(player, units, target);
    player->commitReinforcements(units);
    return order;
}

static shared_ptr<orders::Advance> promptAdvanceOrder(const shared_ptr<Player>& player, const list<shared_ptr<Map::Territory>>& defendList, const list<shared_ptr<Map::Territory>>& attackList) {

    string pickSourcePrompt = "From where will you advance your units (Source territory name): ";
    shared_ptr<Map::Territory> src = promptPickTerritory(pickSourcePrompt, defendList);

    string pickTargetPrompt = "Target territory name (must be adjacent to source): ";
    shared_ptr<Map::Territory> target = promptPickTerritory(pickTargetPrompt, attackList);

    string pickUnitsCount = "How many units to move/attack with?: ";
    int units = promptPickInteger(pickUnitsCount, 1, src->getUnits() + 1);

    auto order = std::make_shared<orders::Advance>(player, units, src, target);
    return order;
}

static shared_ptr<orders::Bomb> promptBombOrder(shared_ptr<Player> player, const list<shared_ptr<Map::Territory>>& attackList, Deck* deck) {

    for (const auto& card : player->getHand()->getHand()) {
        if (card->getCardType() == CardType::BOMB) {
            string pickTargetPrompt = "Target territory name (must be adjacent to an owned territory): ";
            shared_ptr<Map::Territory> target = promptPickTerritory(pickTargetPrompt, attackList);
            auto order = make_shared<orders::Bomb>(player, target, deck, card);
            return order;
        }
    }
    cout << "Error finding bomb card... aborting!";
    return nullptr;
}

static shared_ptr<orders::Airlift> promptAirliftOrder(shared_ptr<Player> player, const list<shared_ptr<Map::Territory>>& defendList, Deck* deck) {

    for (const auto& card : player->getHand()->getHand()) {
        if (card->getCardType() == CardType::AIRLIFT) {

            string pickSourcePrompt = "From where will you airlift your units (Source territory name): ";
            shared_ptr<Map::Territory> src = promptPickTerritory(pickSourcePrompt, defendList);

            string pickTargetPrompt = "where will the airlift land (target territory name): ";
            shared_ptr<Map::Territory> target = promptPickTerritory(pickTargetPrompt, defendList);

            string pickUnitsCount = "How many units to move with?: ";
            int units = promptPickInteger(pickUnitsCount, 1, src->getUnits() + 1);

            auto order = make_shared<orders::Airlift>(player, units, src, target, deck, card);
            return order;
        }
    }
    cout << "Error finding airlift card... aborting!";
    return nullptr;
}

static shared_ptr<orders::Blockade> promptBlockadeOrder(shared_ptr<Player> issuer, const list<shared_ptr<Map::Territory>>& defendList, Deck* deck) {
    for (const auto& card : issuer->getHand()->getHand()) {
        if (card->getCardType() == CardType::BLOCKADE) {

            string pickSourcePrompt = "What territory will you blockade (must belong to you): ";
            shared_ptr<Map::Territory> target = promptPickTerritory(pickSourcePrompt, defendList);

            auto order = make_shared<orders::Blockade>(issuer, target, deck, card);
            return order;
        }
    }
    cout << "Error finding blockade card... aborting!";
    return nullptr;
}

static shared_ptr<orders::Negotiate> promptNegotiateOrder(shared_ptr<Player> issuer, const map<int, shared_ptr<Player>>& players, Deck* deck) {

    for (const auto& card : issuer->getHand()->getHand()) {
        if (card->getCardType() == CardType::DIPLOMACY) {

            string pickAllyPrompt = "which player would you like to avoid conflicts with this next round: ";
            shared_ptr<Player> target = promptPickPlayer(pickAllyPrompt, players, issuer);

            auto order = make_shared<orders::Negotiate>(issuer, target, deck, card);
            return order;
        }
    }
    cout << "Error finding Diplomacy card... aborting!";
    return nullptr;
}

//HumanPlayer
HumanPlayer::HumanPlayer() {
    this->player = nullptr;
}
HumanPlayer::HumanPlayer(std::shared_ptr<Player> player) {
    this->player = player;
}

bool HumanPlayer::issueOrder(const map<int, shared_ptr<Player>>& players, const std::shared_ptr<Map>& map, Deck* deck) {

    cout << "Issuing an order to player" << endl;

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
    printHand(*player);

    bool reinforcementLock = false;
    int reinforcmentPool = player->getReinforcementPool();
    int committedReinforcements = player->getCommitedReinforcements();
    int remainingReinforcements = reinforcmentPool - committedReinforcements;
    if (remainingReinforcements > 0) {
        reinforcementLock = true;
        cout << "you have" << (remainingReinforcements)
            << " active reinforcements you must deploy on your territories" << endl;
    }

    shared_ptr<orders::Order> newOrder = nullptr;

    bool orderIssued = false;
    while (!orderIssued) {
        switch (pickOrderType(reinforcementLock)) {
        case 1: //(deploy)
            newOrder = promptDeployOrder(player, defendList, reinforcmentPool, committedReinforcements);
            orderIssued = true;
            break;
        case 2://(advance)
            newOrder = promptAdvanceOrder(player, defendList, attackList);
            orderIssued = true;
            break;
        case 3: //(bomb)
            if (!player->getHand()->contains(CardType::BOMB)) {
                cout << "Hand must contain a bomb card";
                break;
            }
            newOrder = promptBombOrder(player, attackList, deck);
            orderIssued = true;
            break;
        case 4: //(airlift)
            if (!player->getHand()->contains(CardType::AIRLIFT)) {
                cout << "Hand must contain an airlift card";
                break;
            }
            newOrder = promptAirliftOrder(player, defendList, deck);
            orderIssued = true;
            break;
        case 5: //(blockade)
            if (!player->getHand()->contains(CardType::BLOCKADE)) {
                cout << "Hand must contain a bomb card";
                break;
            }
            newOrder = promptBlockadeOrder(player, defendList, deck);
            orderIssued = true;
            break;
        case 6: //(negotiate)
            if (!player->getHand()->contains(CardType::DIPLOMACY)) {
                cout << "Hand must contain a diplomacy card";
                break;
            }
            newOrder = promptNegotiateOrder(player, players, deck);
            orderIssued = true;
            break;
        default:
            orderIssued = true;
            break;
        }
    }

    if (!newOrder) return false;

    player->addOrder(newOrder);
    std::cout << "Order issued successfully" << endl;
    return true;

}
list<std::shared_ptr<Map::Territory>> HumanPlayer::toAttack() {

    return getalladjacentTerritories(player);
}
list<std::shared_ptr<Map::Territory>> HumanPlayer::toDefend() {
    list<std::shared_ptr<Map::Territory>> defendList;                   // list of territories to defend
    // Return all territories owned by the player in priority order
    // Priority: territories that are adjacent to enemy territories should be defended first
    for (auto& territory : player->getTerritories()) {
        defendList.push_back(territory);
    }
    return defendList;
}

//AggressivePlayer
AggressivePlayer::AggressivePlayer() {
    this->player = nullptr;
}
AggressivePlayer::AggressivePlayer(std::shared_ptr<Player> player) {
    this->player = player;
}
bool AggressivePlayer::issueOrder(const map<int, shared_ptr<Player>>& players, const std::shared_ptr<Map>& map, Deck* deck) {
        //strongest territory
    auto strongestTerritory = player->toDefend().front();

    //if player has reinforcemnts deploy them on strongest defended terrritory
    if(player->getReinforcementPool() > 0){
        player->addOrder(std::make_shared<orders::Deploy>(player,player->getReinforcementPool(), strongestTerritory));
        player->assignReinforcments(player->getReinforcementPool());
        return true;
    }
    //strongest enemy territory
    auto attackList = this->toAttack();
    auto target = attackList.front();

    //if has bomb card use it on enemy territory with most units
    if (!attackList.empty() && player->getHand()->contains(CardType::BOMB)) {
        for (const auto& card : player->getHand()->getHand()) {
            if (card->getCardType() == CardType::BOMB) {
                auto order = std::make_shared<orders::Bomb>(player, target, deck, card);
                player->addOrder(order);
                return true;
            }
        }
    }

    //finally advance units from you r strongest to the oponenets strongest nation
    if (!attackList.empty()) {

        int armiesToSend = strongestTerritory->getUnits();

        auto advanceOrder = std::make_shared<orders::Advance>(player,strongestTerritory,target,armiesToSend);

        player->addOrder(advanceOrder);
        return true;
    }
    return false;
}
//gets all adjacent territories and sorts them by number of units on them (attack the strongest armies)
list<std::shared_ptr<Map::Territory>> AggressivePlayer::toAttack() {
    list<std::shared_ptr<Map::Territory>> list =  getalladjacentTerritories(player);
    
    // Sort in Descending order of army count.
    list.sort([](const std::shared_ptr<Map::Territory>& a, const std::shared_ptr<Map::Territory>& b) {
        return a->getUnits() > b->getUnits();  
    });
    return list;
}

//gets all of its own territories and sorts them by number of units on them (defend the strongest armies)
list<std::shared_ptr<Map::Territory>> AggressivePlayer::toDefend() {
    //getting all territories owned by the player
    list<std::shared_ptr<Map::Territory>> list;
    for (auto& territory : player->getTerritories()) {
        list.push_back(territory);
    }
    // Sort in Descending order of army count.
    list.sort([](const std::shared_ptr<Map::Territory>& a, const std::shared_ptr<Map::Territory>& b) {
        return a->getUnits() > b->getUnits();   
    });
    
    return list;
}

//BenevolentPlayer
BenevolentPlayer::BenevolentPlayer() {
    this->player = nullptr;
}
BenevolentPlayer::BenevolentPlayer(std::shared_ptr<Player> player) {
    this->player = player;
}
bool BenevolentPlayer::issueOrder(const map<int, shared_ptr<Player>>& players, const std::shared_ptr<Map>& map, Deck* deck) {

    
    // deploys on territory that has least amount of army units
    

    if (player->getReinforcementPool() > 0) {
        auto defendList = this->toDefend();
        if (!defendList.empty()) {
            auto weakest = defendList.front(); // lowest army territory
            int reinforcements = player->getReinforcementPool();

            player->addOrder(std::make_shared<orders::Deploy>(
                player,
                weakest,
                reinforcements
            ));

            player->assignReinforcments(player->getReinforcementPool());
            return true;
        }
    }

    //if has diplomacy card will use it
    if (player->getHand()->contains(CardType::DIPLOMACY)) {
        for (const auto& card : player->getHand()->getHand()) {
            auto targetPlayer = players.at(0);
            if (card->getCardType() == CardType::DIPLOMACY) {
                auto order = std::make_shared<orders::Negotiate>(player,targetPlayer, deck, card);
                player->addOrder(order);
                return true;
            }
        }
    }

    //advances armies to distribute them cleanly
    auto defendList = this->toDefend();
    if (defendList.size() >= 2) {

        auto weakest = defendList.front();
        auto strongest = defendList.back();

        // Only move if strongest has enough armies to spare
        if (strongest->getUnits() > 1) {
            int moveAmount = strongest->getUnits() / 2;  // move half

            auto advanceOrder = std::make_shared<orders::Advance>(
                player,
                strongest,
                weakest,
                moveAmount
            );

            player->addOrder(advanceOrder);
            return true;
        }
    }
    return false;
} 
//returns a list of its own territories (never attacks others)
list<std::shared_ptr<Map::Territory>> BenevolentPlayer::toAttack() {

    //getting all territories owned by the player
    list<std::shared_ptr<Map::Territory>> list;
    for (auto& territory : player->getTerritories()) {
        list.push_back(territory);
    }
    // Sort in Ascending order of army count.
    list.sort([](const std::shared_ptr<Map::Territory>& a, const std::shared_ptr<Map::Territory>& b) {
        return a->getUnits() < b->getUnits();   
    });
    return list;
}
//returns its own territories sorted by least amount of units
list<std::shared_ptr<Map::Territory>> BenevolentPlayer::toDefend() {
    
    //getting all territories owned by the player
    list<std::shared_ptr<Map::Territory>> list;
    for (auto& territory : player->getTerritories()) {
        list.push_back(territory);
    }
    // Sort in Ascending order of army count.
    list.sort([](const std::shared_ptr<Map::Territory>& a, const std::shared_ptr<Map::Territory>& b) {
        return a->getUnits() < b->getUnits();   
    });
    return list;
}

//NeutralPlayer
NeutralPlayer::NeutralPlayer() {
    this->player = nullptr;
}
NeutralPlayer::NeutralPlayer(std::shared_ptr<Player> player) {
    this->player = player;
}
bool NeutralPlayer::issueOrder(const map<int, shared_ptr<Player>>& players, const std::shared_ptr<Map>& map, Deck* deck) {
    //neutral player doesnt issue orders
    return false;
}
list<std::shared_ptr<Map::Territory>> NeutralPlayer::toAttack() {
    //neutral player doesnt attack others (empty list)
    list<std::shared_ptr<Map::Territory>> list;
    return list;
}
list<std::shared_ptr<Map::Territory>> NeutralPlayer::toDefend() {
    //neutral player doesnt defend (empty list)
    list<std::shared_ptr<Map::Territory>> list;
    return list;
}

//CheaterPlayer
CheaterPlayer::CheaterPlayer() {
    this->player = nullptr;
}
CheaterPlayer::CheaterPlayer(std::shared_ptr<Player> player) {
    this->player = player;
}
bool CheaterPlayer::issueOrder(const map<int, shared_ptr<Player>>& players, const std::shared_ptr<Map>& map, Deck* deck) {

    if (!this->player) {
        return false;
    }

    // reuse toAttack() which contains the traversal logic
    list<std::shared_ptr<Map::Territory>> targets = this->toAttack();

    for (auto& territory : targets) {
        std::shared_ptr<Player> oldOwner = territory->getOwnership();
        if (oldOwner && oldOwner != this->player) {
            oldOwner->removeTerritory(territory);
        }
        territory->setOwnership(this->player);
        this->player->addTerritory(territory);
    }

    return !targets.empty();
}
list<std::shared_ptr<Map::Territory>> CheaterPlayer::toAttack() {

    list<std::shared_ptr<Map::Territory>> attackList;

    if (!this->player) {
        return attackList;
    }

    list<std::shared_ptr<Map::Territory>> owned = this->player->getTerritories();               // territories already owned by the player

    for (auto& t : owned) {
        std::vector<std::shared_ptr<Map::Territory>> neighbors = t->getConnectedTerritories();      // neighboring territories to be owned
        for (auto& n : neighbors) {
            std::shared_ptr<Player> owner = n->getOwnership();
            if (owner && owner != this->player) {
                bool found = false;
                for (auto& existing : attackList) {
                    if (existing == n) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    attackList.push_back(n);
                }
            }
        }
    }

    return attackList;
}
list<std::shared_ptr<Map::Territory>> CheaterPlayer::toDefend() {
    list<std::shared_ptr<Map::Territory>> list;

    if (!this->player) {
        return list;
    }

    return this->player->getTerritories();
}

bool HumanPlayer::isNeutral() const { return false;}
bool AggressivePlayer::isNeutral() const { return false;}
bool BenevolentPlayer::isNeutral() const { return false;}
bool CheaterPlayer::isNeutral() const { return false;}
bool NeutralPlayer::isNeutral() const { return true;}