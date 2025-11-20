#include "PlayerStrategies.h"
#include "../player/player.h"
#include "../map/map.h"
#include <memory>
#include <stdio.h>

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

    std::cout << "Human Player issued order" << std::endl;

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

    std::cout << "Aggressive Player issued order" << std::endl;

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

    std::cout << "Benevolent Player issued order" << std::endl;

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

    std::cout << "Neutral Player issued order" << std::endl;


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

    std::cout << "Cheater Player issued order" << std::endl;


    return false;
}
list<std::shared_ptr<Map::Territory>> CheaterPlayer::toAttack() {
    list<std::shared_ptr<Map::Territory>> list;

    return list;
}
list<std::shared_ptr<Map::Territory>> CheaterPlayer::toDefend() {
    list<std::shared_ptr<Map::Territory>> list;

    return list;
}