/*
* Assignment 1 - COMP 345
* Authors: Ariberto Bello Veras, Joshua Bitton, Liam Daigle, 
* Ash Stone, Cyrus Stonebanks, Kirill Vishnyakov
* Date: 2025-10-07
* Description: This is the main driver file for the Warzone game.
*/

#include <iostream>
#include "../cards/cards.h"
#include "../gameEngine/gameEngine.h"
#include "../map/map.h"
#include "../orders/orders.h"
#include "../player/player.h"
#include "../processor/CommandProcessor.h"

using namespace std;

int main() {
    //testCards();
    //testGameStates();	
    //testLoadMaps();
    //testOrdersLists();
	//testPlayers();
    runProcessor();

   return 0;

}