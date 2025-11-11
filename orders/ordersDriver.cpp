#include "orders.h"
#include <iostream>

using namespace orders;

void testOrdersLists() {

    OrderList* testList = new OrderList();

    Deploy* testDeploy = new Deploy();
    Advance* testAdvance = new Advance();
    Bomb* testBomb = new Bomb();
    Blockade* testBlockade = new Blockade();
    Airlift* testAirlift = new Airlift();
    Negotiate* testNegotiate = new Negotiate();
    
    testList->add(testDeploy);
    testList->add(testAdvance);
    testList->add(testBomb);
    testList->add(testBlockade);
    testList->add(testAirlift);
    testList->add(testNegotiate);
    
    std::cout << "Initial OrderList:" << std::endl << *testList << std::endl;
    

    std::cout << std::endl << "moving order at index 1 to end of list: " << std::endl;
    testList->move(1, testList->size()-1);
    std::cout << *testList << std::endl;

    std::cout << std::endl << "moving order at index 4 to index 2: " << std::endl;
    testList->move(4, 2);
     std::cout << *testList << std::endl;
    
    std::cout << std::endl << "removing order at index 3: " << std::endl;
    Order* removed = testList->remove(3);
    delete removed;
    std::cout << *testList << std::endl;
    
    std::cout << "validating all remaining orders" << std::endl;
    for(int i = 0; i < testList->size(); i++){
        (*testList)[i]->validate();
    }

    delete(testList);
};
