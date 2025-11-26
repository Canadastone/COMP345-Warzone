#include "PlayerStrategies.h"
#include "../player/player.h"
#include "../map/map.h"
#include <memory>
#include <stdio.h>
#include <vector>

void PlayerStrategy::setPlayer(std::shared_ptr<Player> player) {
    this->player = player;
}

PlayerStrategy& PlayerStrategy::operator=(const PlayerStrategy& ps) {

    if (this != &ps) {
        this->player = ps.player;
    }

    return *this;
}

//HumanPlayer
HumanPlayer::HumanPlayer(){
    this->player = nullptr;
}
HumanPlayer::HumanPlayer(std::shared_ptr<Player> player){
    this->player = player;
}
bool HumanPlayer::issueOrder() {

    return false;
}
list<std::shared_ptr<Map::Territory>> HumanPlayer::toAttack() {
    list<std::shared_ptr<Map::Territory>> list;

    return list;
}
list<std::shared_ptr<Map::Territory>> HumanPlayer::toDefend() {
    list<std::shared_ptr<Map::Territory>> list;

    return list;
}

//AggressivePlayer
AggressivePlayer::AggressivePlayer(){
    this->player = nullptr;
}
AggressivePlayer::AggressivePlayer(std::shared_ptr<Player> player){
    this->player = player;
}
bool AggressivePlayer::issueOrder() {

    return false;
}
list<std::shared_ptr<Map::Territory>> AggressivePlayer::toAttack() {
    list<std::shared_ptr<Map::Territory>> list;

    return list;
}
list<std::shared_ptr<Map::Territory>> AggressivePlayer::toDefend() {
    list<std::shared_ptr<Map::Territory>> list;

    return list;
}

//BenevolentPlayer
BenevolentPlayer::BenevolentPlayer(){
    this->player = nullptr;
}
BenevolentPlayer::BenevolentPlayer(std::shared_ptr<Player> player){
    this->player = player;
}
bool BenevolentPlayer::issueOrder() {

    return false;
}
list<std::shared_ptr<Map::Territory>> BenevolentPlayer::toAttack() {
    list<std::shared_ptr<Map::Territory>> list;

    return list;
}
list<std::shared_ptr<Map::Territory>> BenevolentPlayer::toDefend() {
    list<std::shared_ptr<Map::Territory>> list;

    return list;
}

//NeutralPlayer
NeutralPlayer::NeutralPlayer(){
    this->player = nullptr;
}
NeutralPlayer::NeutralPlayer(std::shared_ptr<Player> player){
    this->player = player;
}
bool NeutralPlayer::issueOrder() {

    return false;
}
list<std::shared_ptr<Map::Territory>> NeutralPlayer::toAttack() {
    list<std::shared_ptr<Map::Territory>> list;

    return list;
}
list<std::shared_ptr<Map::Territory>> NeutralPlayer::toDefend() {
    list<std::shared_ptr<Map::Territory>> list;

    return list;
}

//CheaterPlayer
CheaterPlayer::CheaterPlayer(){
    this->player = nullptr;
}
CheaterPlayer::CheaterPlayer(std::shared_ptr<Player> player){
    this->player = player;
}
bool CheaterPlayer::issueOrder() {

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