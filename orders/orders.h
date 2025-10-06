#pragma once

#include <string>
#include <list>

enum class OrderType { DEPLOY, ADVANCE, BOMB, BLOCKADE, AIRLIFT, NEGOTIATE };

class Order{

    private:
        OrderType type;

    public:
        void execute();
        bool validate();
        Order();
        ~Order();
        void setType(OrderType t);
        OrderType getType();
};

class OrderList{

    private:
        std::list<Order> actionList; 

    public:
        void remove();
        void move();

};


class deploy: public Order{

    public:
        deploy();
        ~deploy();
        void execute();
        bool validate();

};

class advance: public Order{

    public:
        advance();
        ~advance();
        void execute();
        bool validate();

};

class bomb: public Order{

    public:
        bomb();
        ~bomb();
        void execute();
        bool validate();

};

class blockade: public Order{

    public:
        blockade();
        ~blockade();
        void execute();
        bool validate();

};

class airlift: public Order{

    public:
        airlift();
        ~airlift();
        void execute();
        bool validate();

};

class negotiate: public Order{

    public:
        negotiate();
        ~negotiate();
        void execute();
        bool validate();

};

