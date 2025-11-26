#pragma once

#include <memory>
#include "../player/player.h"
#include "../map/map.h"

class PlayerStrategy {
protected:
    std::shared_ptr<Player> player;

public:
    virtual bool issueOrder(const map<int, shared_ptr<Player>>& players, const std::shared_ptr<Map>& map, Deck* deck) = 0;
    virtual list<std::shared_ptr<Map::Territory>> toAttack() = 0;
    virtual list<std::shared_ptr<Map::Territory>> toDefend() = 0;
    void setPlayer(std::shared_ptr<Player> player);
    PlayerStrategy& operator=(const PlayerStrategy& ps);
};

class HumanPlayer : public PlayerStrategy {
public:
    HumanPlayer();
    HumanPlayer(std::shared_ptr<Player> player);
    bool issueOrder(const map<int, shared_ptr<Player>>& players, const std::shared_ptr<Map>& map, Deck* deck);
    list<std::shared_ptr<Map::Territory>> toAttack();
    list<std::shared_ptr<Map::Territory>> toDefend();
};

class AggressivePlayer : public PlayerStrategy {
public:
    AggressivePlayer();
    AggressivePlayer(std::shared_ptr<Player> player);
    bool issueOrder(const map<int, shared_ptr<Player>>& players, const std::shared_ptr<Map>& map, Deck* deck);
    list<std::shared_ptr<Map::Territory>> toAttack();
    list<std::shared_ptr<Map::Territory>> toDefend();
};

class BenevolentPlayer : public PlayerStrategy {
public:
    BenevolentPlayer();
    BenevolentPlayer(std::shared_ptr<Player> player);
    bool issueOrder(const map<int, shared_ptr<Player>>& players, const std::shared_ptr<Map>& map, Deck* deck);
    list<std::shared_ptr<Map::Territory>> toAttack();
    list<std::shared_ptr<Map::Territory>> toDefend();
};

class NeutralPlayer : public PlayerStrategy {
public:
    NeutralPlayer();
    NeutralPlayer(std::shared_ptr<Player> player);
    bool issueOrder(const map<int, shared_ptr<Player>>& players, const std::shared_ptr<Map>& map, Deck* deck);
    list<std::shared_ptr<Map::Territory>> toAttack();
    list<std::shared_ptr<Map::Territory>> toDefend();
};

class CheaterPlayer : public PlayerStrategy {
public:
    CheaterPlayer();
    CheaterPlayer(std::shared_ptr<Player> player);
    bool issueOrder(const map<int, shared_ptr<Player>>& players, const std::shared_ptr<Map>& map, Deck* deck);
    list<std::shared_ptr<Map::Territory>> toAttack();
    list<std::shared_ptr<Map::Territory>> toDefend();
};

void testPlayerStrategies();