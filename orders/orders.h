/*
* Assignment 1 - COMP 345
* Authors: Ariberto Bello Veras, Joshua Bitton, Liam Daigle, 
* Ash Stone, Cyrus Stonebanks, Kirill Vishnyakov
* Date: 2025-10-07
* Description: This file contains the declarations for the orders.cpp file
*/
#ifndef ORDERS
#define ORDERS

#include <iostream>
#include <vector>
#include <string>



namespace orders{
    
    enum orderType{ DEPLOY, ADVANCE, BOMB, BLOCKADE, AIRLIFT, NEGOTIATE};

    class Order{
       
        protected:
            orderType type; 
            bool executed;
            Order(orderType t);
            
        public:
            std::string effect;
            virtual bool validate();
            virtual void execute() = 0;
            friend std::ostream& operator<<(std::ostream& os, const Order& order);         
    }; 

    class Deploy : public Order{
        public:

        Deploy();
        void execute() override;
    };

    class Advance : public Order{
        public:

        Advance();
        void execute() override;
    };
    class Bomb : public Order{
        public:

        Bomb();
        void execute() override; 
    };
    class Blockade : public Order{
        public:

        Blockade();
        void execute() override;
    };
    class Airlift : public Order{
        public:

        Airlift();
        void execute() override;
    };
    class Negotiate : public Order{
        public:

        Negotiate();
        void execute() override;
    };

    class OrderList{
        private:
            
       std::vector<Order *> orders;
        
        bool indexOutOfBounds(int index){
            return (index > orders.size() || index < 0);
        }

        public:

        OrderList();

        int size();
        Order* operator[](int index);
        void add(Order* order);
        Order* remove(int index);
        void move(int fromIndex, int toIndex);
    };
}
#endif