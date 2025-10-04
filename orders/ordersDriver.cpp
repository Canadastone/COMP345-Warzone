#include "orders.h"
#include "orders.cpp"
#include <list>
#include <string>
#include <iostream>

using std::list;
using std::cout;
using std::endl;

void testOrdersLists();

int main() {
    cout << "Testing Orders and OrderList classes" << endl;
    testOrdersLists();
}

void testOrdersLists() {

    Order *o1, *o2, *o3, *o4, *o5, *o6; 
    o1 = new deploy();
    //o2 = new advance();
    o3 = new bomb();
    o4 = new blockade();
    o5 = new airlift();
    o6 = new negotiate();

    cout << "hello" << endl;
    cout << o1 << endl;
    list<Order*> ordersList = {o1, o2, o3, o4, o5, o6};

    delete o1;
    delete o2;
    delete o3;
    delete o4;
    delete o5;
    delete o6;

};