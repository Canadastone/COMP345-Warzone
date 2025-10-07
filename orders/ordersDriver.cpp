#include "orders.h"
#include <list>
#include <string>
#include <iostream>

using std::list;
using std::cout;
using std::endl;

void testOrdersLists();

//int main() {
//    cout << "Testing Orders and OrderList classes" << endl;
//    testOrdersLists();
//}

void testOrdersLists() {

    std::shared_ptr<Order> o1, o2, o3, o4, o5, o6; 
    o1 = std::make_shared<deploy>();
    //o2 = new advance();
    o3 = std::make_shared<bomb>();
    o4 = std::make_shared<blockade>();
    o5 = std::make_shared<airlift>();
    o6 = std::make_shared<negotiate>();

    cout << "hello" << endl;
    cout << o1 << endl;
    list<std::shared_ptr<Order>> ordersList = {o1, o2, o3, o4, o5, o6};
};