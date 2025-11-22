#include "PlayerStrategies.h"
#include "../player/player.h"
#include "../cards/cards.h"
#include <memory>

class Player;
class Map;
void testPlayerStrategies(){

	auto player = std::make_shared<Player>();
	std::shared_ptr<Map> map = std::make_shared<Map>("Africa.map");
	
	Deck deck;
	Deck* p_deck = &deck;

	player->setStrategy(std::make_unique<HumanPlayer>());
}