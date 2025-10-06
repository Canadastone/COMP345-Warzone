/*
* Assignment 1 - COMP 345
* Authors: Ariberto Bello Veras, Joshua Bitton, Liam Daigle, 
* Ash Stone, Cyrus Stonebanks, Kirill Vishnyakov
* Date: 2025-10-07
* Description: This file contains the implementation of the move order logic for the Warzone game.
*/

#include "orders.h"
#include <list>
#include <string>
#include <iostream>
#include <vector>
#include <string>
using namespace std;


namespace orders{

    
    Order::Order(orderType t){
        type = t;
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

    bool Order::validate() const{
        //TODO implememt Validate
        return true;
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

    Airlift::Airlift() : Order::Order(orderType::AIRLIFT){
        effect = "order not excecuted yet";
        executed = false;
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

    int OrderList::size() const{
        return orders.size();
    }

    //overloads the [] operator
    Order* OrderList::operator[](int index){
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
     int OrderList::indexOf(Order* order){
        for(int i = 0; i < size(); i++){
            if(order == (*this)[i]){
                return true;
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
}