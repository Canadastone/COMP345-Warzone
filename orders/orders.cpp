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
using namespace std;


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
        switch (static_cast<int>(type)){ 
            case static_cast<int>(orderType::DEPLOY): return "Deploy"; 
            case static_cast<int>(orderType::ADVANCE): return "Advance"; 
            case static_cast<int>(orderType::BOMB): return "Bomb"; 
            case static_cast<int>(orderType::BLOCKADE): return "Blockade"; 
            case static_cast<int>(orderType::AIRLIFT): return "Airlift"; 
            case static_cast<int>(orderType::NEGOTIATE): return "Negotiate"; 
        } 
        return "N/A"; 
    }

    //valid if order type is valid type and has not been executed
    bool Order::validate() const{
        if(getTypeAsString() != "N/A" && executed == false){
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

    Deploy::Deploy() : Order::Order(orderType::DEPLOY){
        effect = "order not excecuted yet";
        executed = false;
    }
    Deploy::Deploy(const Deploy& deploy) : Order::Order(orderType::DEPLOY){
        effect = deploy.effect;
        executed = deploy.executed;
    }

    bool Deploy::validate() const{
        if(Order::validate()){
            return true;
        }
    }

    void Deploy::execute(){
       if(validate()){
            executed = true;
            //TODO implememt the rest of execute;
        } 
    }
    
    Advance::Advance() : Order::Order(orderType::ADVANCE){
        effect = "order not excecuted yet";
        executed = false;
    }
    Advance::Advance(const Advance& advance) : Order::Order(orderType::ADVANCE){
        effect = advance.effect;
        executed = advance.executed;
    }

    bool Advance::validate() const{
        if(Order::validate()){
            return true;
        }
    }

    void Advance::execute(){
       if(validate()){
            executed = true;
            //TODO implememt the rest of execute;
        } 
    } 

    Bomb::Bomb() : Order::Order(orderType::BOMB){
        effect = "order not excecuted yet";
        executed = false;
    }
    Bomb::Bomb(const Bomb& bomb) : Order::Order(orderType::BOMB){
        effect = bomb.effect;
        executed = bomb.executed;
    }

    bool Bomb::validate() const{
        if(Order::validate()){
            return true;
        }
    }

    void Bomb::execute(){
       if(validate()){
            executed = true;
            //TODO implememt the rest of execute;
        } 
    } 

    Blockade::Blockade() : Order::Order(orderType::BLOCKADE){
        effect = "order not excecuted yet";
        executed = false;
    }
    Blockade::Blockade(const Blockade& blockade) : Order::Order(orderType::BLOCKADE){
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

    Negotiate::Negotiate() : Order::Order(orderType::NEGOTIATE){
        effect = "order not excecuted yet";
        executed = false;
    }

    bool Negotiate::validate() const{
        if(Order::validate()){
            return true;
        }
    }

    Negotiate::Negotiate(const Negotiate& negotiate) : Order::Order(orderType::NEGOTIATE){
        effect = negotiate.effect;
        executed = negotiate.executed;
    }

    Airlift::Airlift() : Order::Order(orderType::AIRLIFT){
        effect = "order not excecuted yet";
        executed = false;
    }
    Airlift::Airlift(const Airlift& airlift) : Order::Order(orderType::AIRLIFT){
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

        for(int i = 0; i < orderList.size(); i++){
            os << "[" << i << "] " << *(orderList[i]) << std::endl;
        }

        return os;
    }
}