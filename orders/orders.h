#pragma once

/*
* Assignment 1 - COMP 345
* Authors: Ariberto Bello Veras, Joshua Bitton, Liam Daigle, 
* Ash Stone, Cyrus Stonebanks, Kirill Vishnyakov
* Date: 2025-10-07
* Description: This file contains the declarations for the orders.cpp file
*/
#ifndef ORDERS_H
#define ORDERS_H

#include <iostream>
#include <vector>
#include <string>


//namespace containing all order related definitions
namespace orders{
    
    /*
    enum for all valid order types
    */
    enum class orderType{ DEPLOY, ADVANCE, BOMB, BLOCKADE, AIRLIFT, NEGOTIATE};

    /*
    Base Absract Class, shall only be instantiated through its children
    defines what specific orders will implement
    */
    class Order{
       
        protected:

            orderType type; 
            bool executed;

            //constructor to be used by children
            //No default constructor as an Order must have a type
            //creates an order according to the given type
            Order(orderType t);
            virtual ~Order() = default;
            
        public:
            //string will store effect of the orders execution
            std::string effect;
            
            //getters
            orderType getType() const;
            bool isExecuted() const;
            std::string getTypeAsString() const;

            //checks the validity of the order
            virtual bool validate() const;

            //excecutes order
            virtual void execute() = 0;

            friend std::ostream& operator<<(std::ostream& os, const Order& order);         
    }; 

    //class Deploy, Child of abstract Order
    class Deploy : public Order{
        public:

        Deploy();
        void execute() override;
    };
    //class Advance, Child of abstract Order
    class Advance : public Order{
        public:

        Advance();
        void execute() override;
    };
    //class Bomb, Child of abstract Orde
    class Bomb : public Order{
        public:

        Bomb();
        void execute() override; 
    };
    //class Blockade, Child of abstract Orde
    class Blockade : public Order{
        public:

        Blockade();
        void execute() override;
    };
    //class AirLift, Child of abstract Orde
    class Airlift : public Order{
        public:

        Airlift();
        void execute() override;
    };
    //class Negotiate, Child of abstract Order
    class Negotiate : public Order{
        public:

        Negotiate();
        void execute() override;
    };

    //Class Order List, will store and handle the orders in the game
    //uses vector of pointers to contain the orders
    class OrderList{
        private:
            
        //internal vector holding pointers to all the orders 
        std::vector<Order*> orders;
        
        //bound checker private helper
        bool indexOutOfBounds(int index){
            return (index >= orders.size() || index < 0);
        }

        public:

        OrderList();
        ~OrderList();

        int size() const;
        Order* operator[](int index);
        void add(Order* order);

        Order* remove(int index);
        int indexOf(Order* order);
        void move(int fromIndex, int toIndex);
    };
}
#endif