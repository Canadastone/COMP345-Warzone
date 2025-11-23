/*
* Assignment 1 - COMP 345
* Authors: Ariberto Bello Veras, Joshua Bitton, Liam Daigle, 
* Ash Stone, Cyrus Stonebanks, Kirill Vishnyakov
* Date: 2025-10-07
* Description: This file contains the implementation of the move order logic for the Warzone game.
*/

#include "orders.h"
#include <string>
#include <iostream>
#include <vector>
#include <random>
#include "../player/player.h"
#include "../map/map.h"


using namespace std;

static bool belongsToPlayer(Player& player, Map::Territory& terr){
  list<shared_ptr<Map::Territory>> ownedTerritories(player.getTerritories());
  for(shared_ptr<Map::Territory> territory : ownedTerritories){
    if(*territory == terr) return true;
  }
  return false;
}

static bool areAdjacent(Map::Territory& terr1, Map::Territory& terr2){
  vector<shared_ptr<Map::Territory>> adjTerritories(terr1.getConnectedTerritories());
  for(shared_ptr<Map::Territory> territory : adjTerritories){
    if(*territory == terr2) return true;
  }
  return false;
}

static bool hasSufficientUnits(Map::Territory& terr, int units){
    return terr.getUnits() >= units;
}

static void transferOwnership(shared_ptr<Player> prevOwner, shared_ptr<Player> nextOwner, shared_ptr<Map::Territory> terr){
    prevOwner->removeTerritory(terr);
    nextOwner->addTerritory(terr);
    terr->setOwnership(nextOwner);
}

namespace orders{

    std::shared_ptr<Player> Order::neutralPlayer;

    Order::Order(const orderType t)
    :   type(t),
        executed(false),
        effect("order not excecuted yet")
    {}

    Order::Order(const Order& order)
    :   type(order.type),
        executed(false),
        effect("order not excecuted yet")
    {}

    orderType Order::getType() const{
        return type;
    }

    bool Order::isExecuted() const{
        return executed;
    }

    shared_ptr<Player> Order::getNeutral(){
        if (!neutralPlayer) {
        neutralPlayer = std::make_shared<Player>();
        }
        return neutralPlayer;
    }
    //returns type of order as a string
    string Order::getTypeAsString() const{
        switch (type){ 
            case orderType::DEPLOY: return "Deploy"; 
            case orderType::ADVANCE: return "Advance"; 
            case orderType::BOMB: return "Bomb"; 
            case orderType::BLOCKADE: return "Blockade"; 
            case orderType::AIRLIFT: return "Airlift"; 
            case orderType::NEGOTIATE: return "Negotiate"; 
        } 
        return "N/A"; 
    }

    //valid if order type is valid type and has not been executed
    bool Order::validate() const{
        return (getTypeAsString() != "N/A" && !isExecuted());
    }
            
    Order& Order::operator=(const Order& order){

        if(this == &order){
            return *this;
        }
        type = order.type;
        executed = order.executed;
        effect = order.effect;
        return *this;
    }

    std::string Order::stringToLog() const{
        std::string commandEffect = this->effect;
        std::string typeAsString =  getTypeAsString();
        std::string loggedString = typeAsString + " Order executed: " + effect;

        return loggedString;
    }

    void Order::attach(std::shared_ptr<LogObserver> pObserver) {
        this->observer = pObserver;
    }

    void Order::detach() {
        this->observer = nullptr;
    }

    void Order::notify(ILoggable& loggable) const{
        this->observer->update(loggable);
    }

    void Order::notifyOrder(Order& order) {
        this->notify(order);
    }

    //overloaded << operator
    std::ostream& operator<<(std::ostream& os, const Order& order){
        os << order.getTypeAsString() << " Order, " << order.effect;
            return os;
    }

   // static helper implementation
    std::set<std::pair<const Player*, const Player*>> Order::s_truces;

    std::pair<const Player*, const Player*>Order::normPair(const std::shared_ptr<Player>& a, const std::shared_ptr<Player>& b) {
        const Player* pa = a.get();
        const Player* pb = b.get();
        return (pa < pb) ? std::make_pair(pa, pb) : std::make_pair(pb, pa);
    }

    void Order::addTruce(const std::shared_ptr<Player>& a, const std::shared_ptr<Player>& b) {
        if (!a || !b || a.get() == b.get()) return;
        s_truces.insert(normPair(a, b));
    }

    bool Order::haveTruce(const std::shared_ptr<Player>& a, const std::shared_ptr<Player>& b) {
        if (!a || !b || a.get() == b.get()) return false;
        return s_truces.count(normPair(a, b)) != 0;
    }

    void Order::clearTruces() {
        s_truces.clear();
    }


    //Children Implementations of the Order Class:

    Deploy::Deploy() 
      : Order(orderType::DEPLOY),
        player(nullptr),
        units(),
        target(nullptr)
        {}

    Deploy::Deploy(shared_ptr<Player> player, int units, shared_ptr<Map::Territory> target)
      : Order(orderType::DEPLOY),
        player(player),
        units(units),
        target(target)
      {}

    Deploy::Deploy(const Deploy& deploy) 
      : Order(deploy),
        player(deploy.player),
        units(deploy.units),
        target(deploy.target)
      {}

    bool Deploy::validate() const{
        if(Order::validate()){
            return belongsToPlayer(*player, *target) && player->getReinforcementPool() >= units;
        }
        return false;
    }

    void Deploy::execute(){
        if(validate()){
           executed = true;
           player->assignReinforcments(-units);
           player->resetCommittedReinforcements();
           target->addUnits(units);
           ostringstream oss;
           oss << units << " units were deployed on " << target->getName();
           effect = oss.str();
        } else{
            effect = "execution failed, Order is Invalid";
        }
        this->notifyOrder(*this);
    }
    
    Advance::Advance() 
      : Order(orderType::ADVANCE),
        player(nullptr),
        units(),
        source(nullptr),
        target(nullptr)
      {}

    Advance::Advance(shared_ptr<Player> player, int units, shared_ptr<Map::Territory> source, shared_ptr<Map::Territory> target)
      : Order(orderType::ADVANCE),
        player(player),
        units(units),
        source(source),
        target(target)
      {} 
        
    Advance::Advance(const Advance& advance) 
      : Order(advance),
        player(advance.player),
        units(advance.units),
        source(advance.source),
        target(advance.target)
      {} 

    bool Advance::validate() const{
        if(!Order::validate()) return false;

        shared_ptr<Player> defender = target->getOwnership();
        if (Order::haveTruce(player, defender)) return false; 

        return belongsToPlayer(*player, *source) && areAdjacent(*source, *target) && hasSufficientUnits(*source, units);
    }

    void Advance::execute(){
        if(!validate()){
            effect = "execution failed, Order is Invalid";
            this->notifyOrder(*this);
            return;
        } 
        executed = true;
        source->removeUnits(units);
        if(belongsToPlayer(*player, *target)){
            target->addUnits(units);
            ostringstream oss;
            oss << units << " units moved to " << target->getName()
            << ", from " << source->getName();
            effect = oss.str();
            this->notifyOrder(*this);
            return;
        }
        
        int attackKillCount(0);
        int defendKillCount(0);
        int attackingUnits(units);
        int defensiveUnits(target->getUnits());

        std::random_device randomDevice; 
        std::mt19937 gen(randomDevice()); 
        std::uniform_real_distribution<> dist(0.0, 10.0);
        
        for(int i = 0; i < attackingUnits; i++){
            if(dist(gen) < 6) attackKillCount++;
        }

        for(int i = 0; i < defensiveUnits; i++){
            if(dist(gen) < 7) defendKillCount++;
        }

        attackingUnits -= defendKillCount;
        defensiveUnits -= attackKillCount;
        if(defensiveUnits < 0) defensiveUnits = 0;
        if(attackingUnits < 0) attackingUnits = 0;

        ostringstream oss;
        oss << units << " units attacked " << target->getName() 
        << ", where " << target->getUnits()
        << " units defended. "
        << attackingUnits << " attcking units survived "
        << "and " << defensiveUnits << " defensive units survived";
        effect = oss.str();
 
        if(defensiveUnits == 0 && attackingUnits > 0){
            target->setUnits(attackingUnits);
            transferOwnership(target->getOwnership(), player, target);
        } else {
            source->addUnits(attackingUnits);
            target->setUnits(defensiveUnits);
        }
        
        this->notifyOrder(*this);
        //TODO: externally player recieves a card if they succesfully conquered at least one territory 
    } 

    Bomb::Bomb() 
        :   Order(orderType::BOMB),
            player(nullptr),
            target(nullptr),
            deck(nullptr),
            bombCard(nullptr)     
        {} 

    Bomb::Bomb(shared_ptr<Player> player, shared_ptr<Map::Territory> target, Deck* deck, shared_ptr<Card> bombCard)
        :   Order(orderType::BOMB),
            player(player),
            target(target),
            deck(deck),
            bombCard(bombCard)
        {} 

    Bomb::Bomb(const Bomb& bomb) 
        :   Order(bomb),
            player(bomb.player),
            target(bomb.target),
            deck(deck),
            bombCard(bomb.bombCard)
        {} 

    bool Bomb::validate() const{
        if(!Order::validate() || belongsToPlayer(*player, *target) || bombCard->getCardType() != CardType::BOMB)
            return false;
        list<shared_ptr<Map::Territory>> ownedTerritories(player->getTerritories());
        for(shared_ptr<Map::Territory> territory : ownedTerritories){
           if(areAdjacent(*territory, *target)) return true;
        }
        return false;
    }

    void Bomb::execute(){
        if(validate()){
            executed = true;
            player->getHand()->useCard(bombCard, *deck);
            int initialUnits = target->getUnits();
            target->setUnits(initialUnits / 2);
            ostringstream oss;
            oss << initialUnits << " units bombed in " << target->getName()
            << ", only " << target->getUnits() << "survived";
            effect = oss.str();
        } else {
             effect = "execution failed, Order is Invalid";
        } 
        this->notifyOrder(*this);
    } 

    Blockade::Blockade() 
      : Order(orderType::BLOCKADE),
        player(nullptr),
        target(nullptr),
        deck(nullptr),
        blockadeCard(nullptr)
      {}

    Blockade::Blockade(shared_ptr<Player> player, shared_ptr<Map::Territory> target, Deck* deck, shared_ptr<Card> blockadeCard)
      : Order(orderType::BLOCKADE),
        player(player),
        target(target),
        deck(deck),
        blockadeCard(blockadeCard)
      {}


    Blockade::Blockade(const Blockade& blockade) 
      : Order(blockade),
        player(blockade.player),
        target(blockade.target),
        deck(blockade.deck),
        blockadeCard(blockade.blockadeCard)
    {}

    bool Blockade::validate() const{
        if(Order::validate()){
            return belongsToPlayer(*player, *target) && blockadeCard->getCardType() == CardType::BLOCKADE;
        }
        return false;
    }

    void Blockade::execute(){
       if(validate()){
            executed = true;
            player->getHand()->useCard(blockadeCard, *deck);
            player->removeTerritory(target);
            target->setUnits(target->getUnits()*2);
            transferOwnership(player, getNeutral(), target);
            ostringstream oss;
            oss << target->getName() << " blockaded. Neutral Player now occupies it with "
            << target->getUnits() << " units";
            effect = oss.str();
        } else {
            effect = "execution failed, Order is Invalid";
        } 
        this->notifyOrder(*this);
    }


    Negotiate::Negotiate() 
    : Order(orderType::NEGOTIATE),
      issuer(nullptr),
      target(nullptr),
      deck(nullptr),
      diplomacyCard(nullptr)
    {}

    Negotiate::Negotiate(shared_ptr<Player> issuer, shared_ptr<Player> target, Deck* deck, shared_ptr<Card> diplomacyCard)
    : Order(orderType::NEGOTIATE),
      issuer(issuer),
      target(target),
      deck(deck),
      diplomacyCard(diplomacyCard)
    {}
    

    Negotiate::Negotiate(const Negotiate& negotiate) 
    : Order(orderType::NEGOTIATE),
      issuer(negotiate.issuer),
      target(negotiate.target),
      deck(negotiate.deck),
      diplomacyCard(negotiate.diplomacyCard)
    {}

    bool Negotiate::validate() const{
        if(Order::validate()){
            return issuer != target && diplomacyCard->getCardType() == CardType::DIPLOMACY;
        }
        return false; 
    }
    
    void Negotiate::execute(){
        if(validate()){
            executed = true;
            issuer->getHand()->useCard(diplomacyCard, *deck);
 
            Order::addTruce(issuer, target);

            ostringstream oss;
            oss << "issuer" << " and " << "target player"
            << " negotiated. They can no longer attack eachother this round";
            effect = oss.str();
        } else {
            effect = "execution failed, Order is Invalid";
        } 
        this->notifyOrder(*this);
    }

    
    Airlift::Airlift() 
    : Order(orderType::AIRLIFT),
        player(nullptr),
        units(0),
        source(nullptr),
        target(nullptr),
        deck(nullptr),
        airliftCard(nullptr)
    {}
    Airlift::Airlift(shared_ptr<Player> player, int units, shared_ptr<Map::Territory> source, shared_ptr<Map::Territory> target, Deck* deck, shared_ptr<Card> airliftCard)
      : Order(orderType::AIRLIFT),
        player(player),
        units(units),
        source(source),
        target(target),
        deck(deck),
        airliftCard(airliftCard)
    {}

    Airlift::Airlift(const Airlift& airlift) 
    :   Order(airlift),
        player(airlift.player),
        units(airlift.units),
        source(airlift.source),
        target(airlift.target),
        deck(airlift.deck),
        airliftCard(airlift.airliftCard)
    {}
        

    bool Airlift::validate() const{
        if(Order::validate()){
            return belongsToPlayer(*player, *target) && belongsToPlayer(*player, *source) 
                    && airliftCard->getCardType() == CardType::AIRLIFT && hasSufficientUnits(*source, units);
        }
        return false;
    }

    void Airlift::execute(){
       if(validate()){
            executed = true;
            player->getHand()->useCard(airliftCard, *deck);
            source->removeUnits(units);
        
            target->addUnits(units);
            ostringstream oss;
            oss << units << " units airlifted to " << target->getName()
            << ", from " << source->getName();
            effect = oss.str();
        } else {
            effect = "execution failed, Order is Invalid";
        } 
        this->notifyOrder(*this);
    }


    OrderList::OrderList(){}

    OrderList::OrderList(const OrderList& other){
        for(int i = 0; i < size(); i++){
            add(other[i]);
        }
    }

    OrderList::~OrderList() {
        orders.clear();
    }

    bool OrderList::indexOutOfBounds(int index) const{
        return (index >= orders.size() || index < 0);
    }

    int OrderList::size() const{
        return orders.size();
    }

    //overloads the [] operator
    shared_ptr<Order> OrderList::operator[](int index) const{
        return orders.at(index);
    }

    void OrderList::add(shared_ptr<Order> order){
        order->attach(this->observer);
        orders.push_back(order);
        this->notify(*this);
    }

    /**
     * removes the Order at index
     * shifts all subsequent orders up the list to fill the empty spot
     * does not destroy the removed order
     * instead returns the pointer to the removed element
     * must be call the destroyer externally.
     */
    shared_ptr<Order> OrderList::remove(int index){

        if (indexOutOfBounds(index)){
            string err = "Index " + std::to_string(index) + ",out of bounds"; 
            throw std::runtime_error(err);
        }
        shared_ptr<Order> removed = orders.at(index);
        orders.erase(orders.begin() + index);
        return removed;
     }

     /**
      * returns the index of the Order Passed to it
      * does not compare inner fields, compares adresses
      * will only return an index if the given pointer points to an order that is in the List
      * if no index found returns -1;
      */
     int OrderList::indexOf(shared_ptr<Order> order) const{
        for(int i = 0; i < size(); i++){
            if(order == (*this)[i]){
                return i;
            }
        }
        return -1;
     }

     /**
      * moves Order at index "fromIndex" to "toIndex"
      * shifts all other orders in the list to fill the "fromIndex"
      * throws an error if the index is out of bounds
      */
    void OrderList::move(int fromIndex, int toIndex){

        if (indexOutOfBounds(fromIndex) || indexOutOfBounds(toIndex)){
            string err = "Index out of bounds"; 
            throw std::runtime_error(err);
        }

        shared_ptr<Order>* innerArray = orders.data();

        if(toIndex > fromIndex){
            shared_ptr<Order> temp = innerArray[fromIndex];
            for(int i = fromIndex; i < toIndex; i++){
                innerArray[i] = innerArray[i+1];
            } 
            innerArray[toIndex] = temp;
        }

        if(fromIndex > toIndex){
            shared_ptr<Order> temp = innerArray[fromIndex];
            for(int i = fromIndex; i > toIndex; i--){
                innerArray[i] = innerArray[i-1];
            }
            innerArray[toIndex] = temp;
        }
    }
    
    OrderList& OrderList::operator=(const OrderList& other){
        if (this == &other) return *this;
        orders.clear();
        for (int i=0; i<other.size(); ++i) add(other[i]);
        return *this;
    }

    //overloaded << operator
    std::ostream& operator<<(std::ostream& os, const OrderList& orderList){
        
        if( orderList.size() == 0){
            os << "Empty List of Orders";
        }

        for(int i = 0; i < orderList.size(); i++){
            os << "[" << i << "] " << *(orderList[i]);
        }   

        return os;
    }

    std::string OrderList::stringToLog() const {
        Order& order = *this->orders.back();
        std::string cardAdded = order.getTypeAsString();
        return cardAdded + " Order issued.";
    }

    void OrderList::attach(std::shared_ptr<LogObserver> pObserver) {
        this->observer = pObserver;
    }

    void OrderList::detach() {
        this->observer = nullptr;
    }

    void OrderList::notify(ILoggable& loggable) const {
        this->observer->update(loggable);
    }
}

