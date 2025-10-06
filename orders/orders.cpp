/*
* Assignment 1 - COMP 345
* Authors: Ariberto Bello Veras, Joshua Bitton, Liam Daigle, 
* Ash Stone, Cyrus Stonebanks, Kirill Vishnyakov
* Date: 2025-10-07
* Description: This file contains the implementation of the player order logic for the
*/

#include "orders.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

namespace{
    

    string getTypeAsString(orders::orderType type){
        switch (type){
            case orders::DEPLOY: return "Deploy";
            case orders::ADVANCE: return "Advance";
            case orders::BOMB: return "Bomb";
            case orders::BLOCKADE: return "Blockade";
            case orders::AIRLIFT: return "Airlift";
            case orders::NEGOTIATE: return "Negotiate";
        }
        return "N/A";
    }
}

namespace orders{

    
    Order::Order(orderType t){
        type = t;
    }

    bool Order::validate(){
        //TODO implememt Validate
        return true;
    }
            

    std::ostream& operator<<(std::ostream& os, const Order& order){
        os << getTypeAsString(order.type) << " Order, " << order.effect;
            return os;
    }

    Deploy::Deploy() : Order::Order(DEPLOY){
        effect = "order not excecuted yet";
        executed = false;
    }

    void Deploy::execute(){
       if(validate()){
            executed = true;
            //TODO implememt the rest of execute;
        } 
    }
    
    Advance::Advance() : Order::Order(ADVANCE){
        effect = "order not excecuted yet";
        executed = false;
    }

    void Advance::execute(){
       if(validate()){
            executed = true;
            //TODO implememt the rest of execute;
        } 
    } 

    Bomb::Bomb() : Order::Order(BOMB){
        effect = "order not excecuted yet";
        executed = false;
    }

    void Bomb::execute(){
       if(validate()){
            executed = true;
            //TODO implememt the rest of execute;
        } 
    } 

    Blockade::Blockade() : Order::Order(BLOCKADE){
        effect = "order not excecuted yet";
        executed = false;
    }

    void Blockade::execute(){
       if(validate()){
            executed = true;
            //TODO implememt the rest of execute;
        } 
    } 

    Negotiate::Negotiate() : Order::Order(NEGOTIATE){
        effect = "order not excecuted yet";
        executed = false;
    }

    Airlift::Airlift() : Order::Order(AIRLIFT){
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

    int OrderList::size(){
        return orders.size();
    }

    Order* OrderList::operator[](int index){
        return orders.at(index);
    }

    void OrderList::add(Order* order){
        orders.push_back(order);
    }

    Order* OrderList::remove(int index){

            if (indexOutOfBounds(index)){
                string err = "Index " + std::to_string(index) + ",out of bounds"; 
                throw std::runtime_error(err);
            }
            Order* removed = orders.at(index);
            orders.erase(orders.begin() + index);
            return removed;
        }

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