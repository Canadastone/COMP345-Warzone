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
#include "../logObserver/LoggingObserver.h"

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
    class Order : ILoggable, Subject{


        private:
            std::shared_ptr<LogObserver> observer;
       
        protected:

            orderType type; 
            bool executed;

            //constructor to be used by children
            //No default constructor as an Order must have a type
            //creates an order according to the given type
            Order(const orderType t);
            Order(const Order& order);
            
            
        public:
            //string will store effect of the orders execution
            std::string effect;

            //desructor is public allows for anyone to destroy a preexisting Order
            virtual ~Order() = default;

            //getters
            orderType getType() const;
            bool isExecuted() const;
            std::string getTypeAsString() const;

            //checks the validity of the order
            virtual bool validate() const;

            //excecutes order
            virtual void execute() = 0;

            Order& operator=(const Order& order);
            friend std::ostream& operator<<(std::ostream& os, const Order& order);
            
            //Implement ILoggable functions
            std::string stringToLog() const override;
            
            //Implement Subject functions
            void attach(std::shared_ptr<LogObserver> pObserver) override;
            void detach() override;
            void notify(ILoggable& loggable) const override;

            //Subject notify helper method
            void notifyOrder(Order& order);
    }; 

    //class Deploy, Child of abstract Order
    class Deploy : public Order{
        public:

        Deploy();
        Deploy(const Deploy& deploy);
        
        bool validate() const override;
        void execute() override;
    };
    //class Advance, Child of abstract Order
    class Advance : public Order{
        public:

        Advance();
        Advance(const Advance& advance);
        bool validate() const override;
        void execute() override;
    };
    //class Bomb, Child of abstract Orde
    class Bomb : public Order{
        public:

        Bomb();
        Bomb(const Bomb& bomb);
        bool validate() const override;
        void execute() override; 
    };
    //class Blockade, Child of abstract Orde
    class Blockade : public Order{
        public:

        Blockade();
        Blockade(const Blockade& blockade);
        bool validate() const override;
        void execute() override;
    };
    //class AirLift, Child of abstract Orde
    class Airlift : public Order{
        public:

        Airlift();
        Airlift(const Airlift& airlift);
        bool validate() const override;
        void execute() override;
    };
    //class Negotiate, Child of abstract Order
    class Negotiate : public Order{
        public:

        Negotiate();
        Negotiate(const Negotiate& negotiate);
        bool validate() const override;
        void execute() override;
    };

    //Class Order List, will store and handle the orders in the game
    //uses vector of pointers to contain the orders
    class OrderList : ILoggable, Subject{
        private:
            
        //internal vector holding pointers to all the orders 
        std::vector<Order*> orders;
        std::shared_ptr<LogObserver> observer;
        
        //bound checker private helper
        bool indexOutOfBounds(int index) const;

        public:

        OrderList();
        OrderList(const OrderList& other);
        ~OrderList();


        int size() const;
        Order* operator[](int index) const;
        void add(Order* order);

        Order* remove(int index);
        int indexOf(Order* order) const;
        void move(int fromIndex, int toIndex);

        OrderList& operator=(const OrderList& other);
        friend std::ostream& operator<<(std::ostream& os, const OrderList& orderlist);

        //Implement ILoggable functions
        std::string stringToLog() const;
        //Implement Subject functions
        void attach(std::shared_ptr<LogObserver> pObserver);
        void detach();
        void notify(ILoggable& loggable) const;
    };
}
void testOrdersLists();
#endif