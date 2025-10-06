#include "orders.h"
#include <iostream>

using namespace orders;


void printListHelper(OrderList* list){
    for(int i = 0; i < list->size(); i++){
        std::cout << "[" << i << "] "   
        << *((*list)[i]) << std::endl;
    }
    std::cout << std::endl; 
}

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
    
    std::cout << "Initial OrderList:" << std::endl;
    printListHelper(testList);

    std::cout << std::endl << "moving order at index 1 to end of list: " << std::endl;
    testList->move(1, testList->size()-1);
    printListHelper(testList);

    std::cout << std::endl << "moving order at index 4 to index 2: " << std::endl;
    testList->move(4, 2);
    printListHelper(testList);
    
    std::cout << std::endl << "removing order at index 3: " << std::endl;
    testList->remove(3);
    printListHelper(testList);
    
};

int main(){
    testOrdersLists();
    return 0;
}