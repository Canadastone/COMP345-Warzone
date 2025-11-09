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
#include "../player/player.h"
#include "../map/map.h"


using namespace std;

static bool belongsToPlayer(Player& player, <shared_ptr>Map::Territory terr){
  List<shared_ptr<Map::Territory>> ownedTerritories (player.getTerritories());
  for( shared_ptr<Map::Territory> territory : ownedTerritories){
    if(*territory == *terr) return true;
  }
  return falsea
}

static bool areAdjacent(shared_ptr<Map::Territory> terr1, shared_ptr<Map::Territory> terr2){
  vector<shared_ptr<Map::Territory>> adjTerritories(terr1.getConnectedTerritories());
  for(shared_ptr<Map::Territory> territory : adjTerritories){
    if(*territory == *terr2) return true;
  }
  return false;
}

namespace orders{

    
    Order::Order(const orderType t){
        type = t;
    }

    Order::Order(const Order& order){
        type = order.type;
    }

    orderType Order::getType() const{
        return type;
    }

    bool Order::isExecuted() const{
        return executed;
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
        if(getTypeAsString() != "N/A" && !isExecuted()){
            cout << *this << ". Order is valid." << endl;
            return true;
        }
        return false;
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

    //overloaded << operator
    std::ostream& operator<<(std::ostream& os, const Order& order){
        os << order.getTypeAsString() << " Order, " << order.effect;
            return os;
    }

    //Children Implementations of the Order Class:

    Deploy::Deploy() 
      : Order(orderType::DEPLOY),
        player(nullptr),
        target(nullptr),
        units(0);
        effect("order not excecuted yet"),
        executed(false);
      {}

    Deploy::Deploy(shared_ptr<Player> player, int units, shared_ptr<Map::Territory> target)
      :
        player(player),
        target(target),
        units(units),
        effect("order not executed yet"),
        executed(false)
      {}

    Deploy::Deploy(const Deploy& deploy) 
      : Order(orderType::DEPLOY),
        player(deploy.player),
        target(deploy.target),
        units(deploy.units),
        effect(deploy.effect),
        executed(deploy.excecuted)
      {}

    bool Deploy::validate() const{
        if(Order::validate()){
            return belongsToPlayer(player, territory);
        }
        return false;
    }

    void Deploy::execute(){
       if(validate()){
           executed = true;
           //decremement reinforcement pool cant do yet cause private acess in player class
           //territory.numArmies += units;
           ostringstream oss;
           oss << units << "units were deployed on " << target.getName();
           effect = oss.str();
       } 
    }
    
    Advance::Advance() 
      : Order(orderType::ADVANCE),
        player(nullptr),
        source(nullptr),
        target(nullptr),
        units(0);
        effect("order not excecuted yet"),
        executed(false);
      {}

    Advance::Advance(shared_ptr<Player>, int units, shared_ptr<Map::Territory> source, shared_ptr<Map::Territory> target)
      : Order(orderType::ADVANCE),
        player(player),
        source(source),
        target(target),
        units(units),
        effect("order not executed yet"),
        executed(false)
      {} 
        
    Advance::Advance(const Advance& advance) 
      : Order(orderType::ADVANCE),
        player(advance.player),
        source(advance.source),
        target(advance.target),
        units(advance.units),
        effect("order not executed yet"),
        executed(false)
      {} 

    bool Advance::validate() const{
        if(Order::validate()){
            return belongsToPlayer(player, source) && areAdjacent(source, target);
        }
        return false;
    }

    void Advance::execute(){
        if(!validate()) return;
        executed = true;
        //source.numArmies -= units;
        if(belongsToPlayer(player, target)){
            //target.numArmies += units;
            ostringstream oss;
            oss << units << "units moved to " << target.getName();
            effect = oss.str();

            return;
        }
        
        int attackKillCount(0);
        int defendKillCount(0);
        int attackingUnits(units);
        int defensiveUnits(0); /*defensiveUnits(target.numArmies);*/
        
        for(int i = 0; i < attackingUnits; i++){
            int rand = srand(time()) % 10;
            if(rand < 6) attackKillCount++;
        }

        for(int i = 0; i < defensiveUnits; i++){
            int rand = srand(time()) % 10;
            if(rand < 7) defendKillCount++;
        }

        units -= defendKillCount;
        defensiveUnits -= attackKillCount;
        if(defensiveUnits < 0) defensiveUnits = 0;
        if(units < 0) units = 0;

        ostringstream oss;
        oss << units << "units attacked" << target.getName() 
        << ", where " << 0/*target.numArmies*/
        << " units defended. "
        << attackingUnits << " attcking units survived"
        << "and " << defensiveUnits << " defensive units survived";
        effect = oss.str();
 
        if(defensiveUnits == 0 && attackingUnits > 0){
            player.addMap::Territory(target);
            //target.numArmies = attackingUnits;
        } else {
            //source.numArmies += attackingUnits;
            //target.numArmies = defendingUnits;
        }
        //TODO externally player recieves a card if they succesfully conquered at least one territory 
    } 

    Bomb::Bomb() 
        :   Order(orderType::BOMB),
            player(nullptr),
            card(nullptr),
            target(nullptr),
            effect("order not executed yet"),
            executed(false)
        {} 

    Bomb(shared_ptr<Player> player, shared_ptr<Map::Territory> target, shared_ptr<Card> card)
        :   Order(orderType::BOMB),
            player(player),
            card(card),
            target(target),
            effect("order not executed yet"),
            executed(false)
        {} 

    Bomb::Bomb(const Bomb& bomb) 
        :   Order(orderType::BOMB),
            player(bomb.player),
            card(bomb.card),
            target(bomb.target),
            effect(bomb.effect),
            executed(bomb.executed)
        {} 

    bool Bomb::validate() const{
        if(!Order::validate() && belongsToplayer(player, target))
            return false;
        List<shared_ptr<Map::Territory>> ownedTerritories (player.getTerritories());
        for( shared_ptr<Map::Territory> territory : ownedTerritories){
           if(areAdjacent(territory, target)) return true;
        }
        return false;
    }

    void Bomb::execute(){
        if(validate()){
            executed = true;
            int initialUnits = 0/*target.numArmies*/;
            //target.numArmies /= 2;
            ostringstream oss;
            oss << initialUnits << " units bombed in " << target.getName()
            << "only " << 0/*target.numArmies*/ << "survived";
            effect = oss.str();
        } 
    } 

    Blockade::Blockade() : Order(orderType::BLOCKADE){
        effect = "order not excecuted yet";
        executed = false;
    }
    Blockade::Blockade(const Blockade& blockade) : Order(orderType::BLOCKADE){
        effect = blockade.effect;
        executed = blockade.executed;
    }

    bool Blockade::validate() const{
        if(Order::validate()){
            return true;
        }
    }

    void Blockade::execute(){
       if(validate()){
            executed = true;
            //TODO implememt the rest of execute;
        } 
    } 

    Negotiate::Negotiate() : Order(orderType::NEGOTIATE){
        effect = "order not excecuted yet";
        executed = false;
    }

    bool Negotiate::validate() const{
        if(Order::validate()){
            return true;
        }
    }

    Negotiate::Negotiate(const Negotiate& negotiate) : Order(orderType::NEGOTIATE){
        effect = negotiate.effect;
        executed = negotiate.executed;
    }

    Airlift::Airlift() : Order(orderType::AIRLIFT){
        effect = "order not excecuted yet";
        executed = false;
    }
    Airlift::Airlift(const Airlift& airlift) : Order(orderType::AIRLIFT){
        effect = airlift.effect;
        executed = airlift.executed;
    } 

    bool Airlift::validate() const{
        if(Order::validate()){
            return true;
        }
    }

    void Airlift::execute(){
       if(validate()){
            executed = true;
            //TODO implememt the rest of execute;
        } 
    }
    
    void Negotiate::execute(){
       if(validate()){
            executed = true;
            //TODO implememt the rest of execute;
        } 
    } 


    OrderList::OrderList(){}

    OrderList::OrderList(const OrderList& other){
        for(int i = 0; i < size(); i++){
            add(other[i]);
        }
    }

    OrderList::~OrderList() {
        for (auto order : orders)
            delete order;
        orders.clear();
    }

    bool OrderList::indexOutOfBounds(int index) const{
        return (index >= orders.size() || index < 0);
    }

    int OrderList::size() const{
        return orders.size();
    }

    //overloads the [] operator
    Order* OrderList::operator[](int index) const{
        return orders.at(index);
    }

    void OrderList::add(Order* order){
        orders.push_back(order);
    }

    /**
     * removes the Order at index
     * shifts all subsequent orders up the list to fill the empty spot
     * does not destroy the removed order
     * instead returns the pointer to the removed element
     * must be call the destroyer externally.
     */
    Order* OrderList::remove(int index){

        if (indexOutOfBounds(index)){
            string err = "Index " + std::to_string(index) + ",out of bounds"; 
            throw std::runtime_error(err);
        }
        Order* removed = orders.at(index);
        orders.erase(orders.begin() + index);
        return removed;
     }

     /**
      * returns the index of the Order Passed to it
      * does not compare inner fields, compares adresses
      * will only return an index if the given pointer points to an order that is in the List
      * if no index found returns -1;
      */
     int OrderList::indexOf(Order* order) const{
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

        Order** innerArray = orders.data();

        if(toIndex > fromIndex){
            Order* temp = innerArray[fromIndex];
            for(int i = fromIndex; i < toIndex; i++){
                innerArray[i] = innerArray[i+1];
            } 
            innerArray[toIndex] = temp;
        }

        if(fromIndex > toIndex){
            Order* temp = innerArray[fromIndex];
            for(int i = fromIndex; i > toIndex; i--){
                innerArray[i] = innerArray[i-1];
            } 
            innerArray[toIndex] = temp;
        }
    }
    
    OrderList& OrderList::operator=(const OrderList& other){
        
        if(this == &other){
            return *this;
        }
        
        for(int i = 0; i < size(); i++){
            add(other[i]);
        }
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
}
