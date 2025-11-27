#include <filesystem>
#include <cctype>
#include <typeinfo>
#include "gameEngine.h"
#include "../playerStrategies/PlayerStrategies.h"
#include "../orders/orders.h"

//abstract State constructors definitions
State::State() = default;
State::State(const State& s) {}
State::~State() = default;

//superclass's operator<< definition. State& s is resolved at runtime, and the correct derived class member functions are called.
std::ostream& operator<<(std::ostream& stream, const State& s) {

	stream << s.getStateName() << " | valid commands/events: ";
	for (const string& command : s.getValidCommands()) {
		stream << command << " ";
	}

	return stream;
}

/*
Default constructor.
*/
template<StateID ID>
StateTemplate<ID>::StateTemplate(): stateName(nullptr), validCommands(nullptr){

}

/*
Custom constructor used to build the states, takes in the stateName and the set of validCommands.
*/
template<StateID ID>
StateTemplate<ID>::StateTemplate(const string& stateName, const unordered_set<string>& stateValidCommands) :
	stateName(make_unique<string>(stateName)),
	validCommands(make_unique<unordered_set<string>>(stateValidCommands)) {
}


/*
Copy contructor; deep copies other state into this state by checking whether other members are null.
*/
template<StateID ID>
StateTemplate<ID>::StateTemplate(const StateTemplate& other) :
	//if other ptr is not null, create a new ptr, then new string allocates new memory, the constructor does a deep copy of others string.
	stateName(other.stateName ? unique_ptr<string>(new string(*other.stateName)) : nullptr),
	validCommands(other.validCommands ? unique_ptr<unordered_set<string>>(new unordered_set<string>(*other.validCommands)) : nullptr) {


}

/*
Since the assignment operator is not inherited, it is declared for every derived class by being declared in the StateTemplate class.

Another reason why it has to be defined is becayse: 
	The default assignment operator in cpp does a shallow copy of raw pointer variables (the pointer would point to the same address),
	In this case, unique_ptr is not copyable, therefore the compiler would by default delete/not generate an assignment operator. 

*/
template<StateID ID>
StateTemplate<ID>& StateTemplate<ID>::operator=(const StateTemplate<ID>& other) {
	if (this == &other) return *this;

	//if other.stateName isnt null, create a new unique_ptr to other.stateName string.
	//deep copy since new memory is being allocated.
	if (other.stateName) {
		this->stateName = make_unique<string>(*other.stateName);
		//same as unique_ptr<string>(new string(*other.stateName))
	}
	//if other is null, reset() makes this stateName pointer null.
	else {
		this->stateName.reset(); // clear this.stateName if other is null
	}

	//same steps for validCommands variable
	if (other.validCommands) {
		this->validCommands = make_unique<unordered_set<string>>(*other.validCommands);
	}
	else {
		this->validCommands.reset();
	}

	return *this;
}

/*
define getID for every possible ID. Any StateTemplate<Start>, StateTemplate<MapValidated>, ... will use this definition
getID returns the ID
*/
template<StateID ID>
StateID StateTemplate<ID>::getID() const {
	return ID;
}
/*
return the stateName, any StateTemplate<ID> uses this definition. 
*/
template<StateID ID>
string& StateTemplate<ID>::getStateName() const{
	return *stateName;
}

/*
return the validCommands, any StateTemplate<ID> uses this definition.
*/
template<StateID ID>
unordered_set<string>& StateTemplate<ID>::getValidCommands() const {
	return *validCommands;
}


/*
onCommand has state specific behavior, it is defined here for every specific state.
*/

// ===========================================STARTUP=========================================

/*
Start State
*/
string startState::onCommand(Command* cmd, GameEngine& engine) {
	string effect;

	if (cmd->getCommandName() == "loadmap") {

		engine.loadMap();
		effect = "loaded map, proceeded to map loaded state.\n";
		engine.transitionState(StateID::MapLoaded);
		
	}
	else if (cmd->getCommandName() == "tournament") {
		effect = "tournament mode selected...";
		engine.transitionState(StateID::Tournament);
	}
	
	
	return effect;
}

/*
Tournament State
*/

string tournamentState::onCommand(Command* cmd, GameEngine& engine) {
	std::string argumentString = cmd->getArgument();
	std::cout << argumentString << std::endl;

	return "Tournament Over";
}



/*
Map Loaded State
*/

string mapLoadedState::onCommand(Command* cmd, GameEngine& engine) {
	string effect;
	
	if (cmd->getCommandName() == "loadmap") {
		engine.loadMap();
		effect = "transitioned to load map state\n";
		engine.transitionState(StateID::MapLoaded);
	}
	else if (cmd->getCommandName() == "validatemap") {

		if (engine.getCurrMap()->validate()) {
			effect = "map is valid, transitioned to map validated state\n";
			engine.transitionState(StateID::MapValidated);
		}
		else {
			effect = "map is invalid, for now just doing nothing\n";
		}

			
	}
	return effect;
}

/*
Map Validated State
*/

string mapValidatedState::onCommand(Command* cmd, GameEngine& engine) {
	string effect;
	
	if (cmd->getCommandName() == "addplayer") {
		effect = "added player to the game, and transitioned to players added state\n";
		engine.addPlayerToGame();
		std::cout << "number of players: " << engine.getNumPlayersInGame() << "\n";
		engine.transitionState(StateID::PlayersAdded);
	}

	return effect;
}


/*
Players Added State
*/

string playersAddedState::onCommand(Command* cmd, GameEngine& engine) {
	string effect;
	bool sixPlayers = false;
	if (cmd->getCommandName() == "addplayer") {
		if (engine.getNumPlayersInGame() + 1 > 6) {
			effect = "can't add more than 6 players to the game.\n";
			sixPlayers = true;
		}
		else {
			effect = "added player to the game, stayed in players added state.\n";
			engine.addPlayerToGame();
			std::cout << "number of players: " << engine.getNumPlayersInGame() << "\n";
			engine.transitionState(StateID::PlayersAdded);
		}
	}


	if (cmd->getCommandName() == "gamestart" || sixPlayers) {
		if (engine.getNumPlayersInGame() < 2) {
			std::cout << "Minimum of 2 players to proceed, currently: " << engine.getNumPlayersInGame() << " players.\n";
			effect = "Minimum of 2 players required, currently: " + std::to_string(engine.getNumPlayersInGame()) + " players. Went back to players added state.\n";
			engine.transitionState(StateID::PlayersAdded);
		}
		else{
			engine.printPlayersInGame();
			
			std::cout << "GAMESTART PHASE SETUP ...\n\n";

			//a) distribute all territories fairly somehow
			engine.assignTerritoriesFairly();

			//b) determine order
			engine.shuffleOrderOfPlay();

			//c) give 50 army units to each player
			std::cout << "Assigning 50 units to each player.\n\n";
			for (const int id : *engine.getOrderOfPlay()) {
				engine.assignUnitsToPlayer(50, id);
			}

			//d) each player draws 2 cards using draw()
			//for now just create a deck on the stack
			engine.createDeck();

			for (const int id : *engine.getOrderOfPlay()) {
				std::cout << "Player with ID: " << id << " draws twice...\n";
				engine.playerDrawsCard(id);
				engine.playerDrawsCard(id);
				std::cout << "\n";

			}

			//e) switch game to play phase.
			engine.setCurrPhase(Phase::play);
			
			effect = "Finished startup phase, beginning main game loop.\n";
		}
	}


	return effect;
}


//====================================================PLAY====================================================

/*
AssignReinforcements State
*/

string assignReinforcementsState::onCommand(Command* cmd, GameEngine& engine) {
	string effect{};
	auto continentsMap = engine.getCurrMap()->getContinentMap();


	std::cout << "\n";
	for (const int id : *engine.getOrderOfPlay()) {
		// (# of territories owned divided by 3, rounded down
		int unitsToAssign = engine.getPlayersMap().at(id)->getTerritories().size()/3;

		shared_ptr<Player> player = engine.getPlayersMap().at(id);
		unordered_set<string> playerTerritories;
		for (auto& t : player->getTerritories()) {
        	playerTerritories.insert(t->getName());
    	}
		for(auto& p : continentsMap){
			int count = 0;
			for(auto& t : p.second){
				if(playerTerritories.count(t->getName()) > 0)
					count++;
				
			}
			if(count == p.second.size()){
				std::cout << "Player " << id << " owns continent " << p.first;
				unitsToAssign += engine.getCurrMap()->getContinentControlBonuses().at(p.first);
			}

		}
		// minimum units to add is 3.
		if(unitsToAssign < 3) unitsToAssign = 3;
	

		std::cout << "assigned " << unitsToAssign << " units to player " << id << "\n";
		engine.assignUnitsToPlayer(unitsToAssign, id);
		
	}
	
	std::cout << "finished assigning reinforcements.\n";
	return effect;
}


/*
Issue Orders State
*/

string issueOrdersState::onCommand(Command* cmd, GameEngine& engine) {
	string effect;
	orders::Order::clearTruces();
	while (true) {
		bool anyIssued = false;
		for (int id : *engine.getOrderOfPlay()) {
			auto player = engine.getPlayersMap().at(id);
			std::cout << "\n--------- Player " << id << " Issue Order -------------\n";
			bool issued = player->issueOrder(engine.getPlayersMap(), engine.getCurrMap(), engine.getDeckOfCards());
			if (issued) anyIssued = true;
		}

		if (!anyIssued) {
			break;
		}
	}
	
	return effect;
}


/*
Execute Orders State
*/

string executeOrdersState::onCommand(Command* cmd, GameEngine& engine) {
	string effect;
	
	// Phase 1: Execute all Deploy orders first (in round-robin fashion)
	std::cout << "=== Executing Deploy Orders ===" << std::endl;
	bool anyDeployOrdersExecuted = false;
	do {
		anyDeployOrdersExecuted = false;
		for (const int id : *engine.getOrderOfPlay()) {
			auto& orders = engine.getPlayersMap().at(id)->getOrders();
			
			// Find the first Deploy order in this player's order list
			for (int i = 0; i < orders.size(); i++) {
				if (orders[i]->getType() == orders::orderType::DEPLOY) {
					std::cout << "Executing Player " << id << "'s " << orders[i]->getTypeAsString() << " order.\n";
					orders[i]->execute(); // validate() is called inside execute()
					orders.remove(i);
					anyDeployOrdersExecuted = true;
					break; // Move to next player after executing one Deploy order
				}
			}
		}
	} while (anyDeployOrdersExecuted);
	
	// Phase 2: Execute all other orders (in round-robin fashion)
	std::cout << "\n=== Executing Other Orders ===" << std::endl;
	bool anyOtherOrdersExecuted = false;
	do {
		anyOtherOrdersExecuted = false;
		for (const int id : *engine.getOrderOfPlay()) {
			auto& orders = engine.getPlayersMap().at(id)->getOrders();
			if (orders.size() == 0) continue;
			
			// Execute the first order (FIFO - First In First Out)
			auto nextOrder = orders[0];
			std::cout << "Executing Player " << id << "'s " << nextOrder->getTypeAsString() << " order.\n";
			nextOrder->execute(); // validate() is called inside execute()
			orders.remove(0);
			anyOtherOrdersExecuted = true;
		}
	} while (anyOtherOrdersExecuted);
	
	std::cout << "\nSuccessfully finished executing all orders.\n";

	return effect;
}


/*
Win State
*/

string winState::onCommand(Command* cmd, GameEngine& engine) {
	string effect;

	if (cmd->getCommandName() == "replay") {
		engine.transitionState(StateID::Start);
	}
	else if (cmd->getCommandName() == "quit") {
		engine.transitionState(StateID::End);
	}
		
	
	return effect;
}


/*
End State
*/
string endState::onCommand(Command* cmd, GameEngine& engine) {
	string effect;
	return effect;
}



/*
clone methods return a deep copy of the specific state
This ensures polymorphic copying works correctly when cloning from a State pointer.
*/
unique_ptr<State> startState::clone() const {
    return unique_ptr<startState>(new startState(*this));
}
unique_ptr<State> mapLoadedState::clone() const {
    return unique_ptr<mapLoadedState>(new mapLoadedState(*this));
}
unique_ptr<State> mapValidatedState::clone() const {
    return unique_ptr<mapValidatedState>(new mapValidatedState(*this));
}
unique_ptr<State> playersAddedState::clone() const {
    return unique_ptr<playersAddedState>(new playersAddedState(*this));
}
unique_ptr<State> assignReinforcementsState::clone() const {
    return unique_ptr<assignReinforcementsState>(new assignReinforcementsState(*this));
}
unique_ptr<State> issueOrdersState::clone() const {
    return unique_ptr<issueOrdersState>(new issueOrdersState(*this));
}
unique_ptr<State> executeOrdersState::clone() const {
    return unique_ptr<executeOrdersState>(new executeOrdersState(*this));
}
unique_ptr<State> winState::clone() const {
    return unique_ptr<winState>(new winState(*this));
}
unique_ptr<State> endState::clone() const {
    return unique_ptr<endState>(new endState(*this));
}


/*
 GameEngine class, that manages all the game structure by keeping track of all the states, and the current state.
*/
GameEngine::GameEngine() : 
	//allocate memory for the states smart_ptr
	states{make_unique<map<StateID, unique_ptr<State>>>()},
	//currState is initially null
	currState{ nullptr },
	//currMap is initially nulls
	currMap{nullptr},
	//allocate memory for the players
	playersMap{ make_unique<map<int, shared_ptr<Player>>>() },
	numPlayersInGame(make_unique<int>(0)),
	currPhase(Phase::startup),
	orderOfPlay (make_unique<vector<int>>()){


	//allocate memory to every state and place them in the states map (key: stateID, val: unique_ptr to State object)
	states->emplace(StateID::Start, make_unique<startState>("start", unordered_set<string>{ "loadmap", "tournament" })); // StateTemplate(const string& stateName, const unordered_set<string>& stateValidCommands);

	states->emplace(StateID::MapLoaded, make_unique<mapLoadedState>("mapLoaded", unordered_set<string>{ "loadmap", "validatemap" }));

	states->emplace(StateID::MapValidated, make_unique<mapValidatedState>("mapValidated", unordered_set<string>{ "addplayer" }));

	states->emplace(StateID::PlayersAdded, make_unique<playersAddedState>("playersAdded", unordered_set<string>{ "addplayer", "gamestart" }));

	states->emplace(StateID::AssignReinforcements, make_unique<assignReinforcementsState>("assignReinforcements", unordered_set<string>{ "issueorder" }));

	states->emplace(StateID::IssueOrders, make_unique<issueOrdersState>("issueOrders", unordered_set<string>{ "issueorder", "issueordersend" }));

	states->emplace(StateID::ExecuteOrders, make_unique<executeOrdersState>("executeOrders", unordered_set<string>{ "execorder", "endexecorders", "win" }));

	states->emplace(StateID::Win, make_unique<winState>("win", unordered_set<string>{ "replay", "quit" }));

	states->emplace(StateID::End, make_unique<endState>("end", unordered_set<string>{ "NA" }));

}

/*
Copy constructor definition
*/
GameEngine::GameEngine(const GameEngine& other){
	
	if(other.states && other.playersMap && other.orderOfPlay){
		//creates a new pointer to a map, then deep copies the states from other into the new map by using the clone method polymorphically.
		states = make_unique<map<StateID, unique_ptr<State>>>();
		for(const std::pair<const StateID, unique_ptr<State>>& p : *other.states){
			(*states)[p.first] = p.second->clone();
		}
		//same thing for playersMap
		playersMap = make_unique<map<int, shared_ptr<Player>>>();
		for(const auto& p : *other.playersMap){
			(*playersMap)[p.first] = make_shared<Player>(*p.second);
		}
		orderOfPlay = make_unique<vector<int>>(*other.orderOfPlay);
		numPlayersInGame = make_unique<int>(*other.numPlayersInGame);
		currPhase = other.currPhase;
		//Update currState to reference the same state (by ID) as in the original GameEngine
		currState = other.currState ? states->at(other.currState->getID()).get() : nullptr;

	}
	else{
		states = nullptr;
		currState = nullptr;
		playersMap = nullptr;
		currMap = nullptr;
		orderOfPlay = nullptr;
		numPlayersInGame = nullptr;
		currPhase = Phase::startup;
	}
	
}
/*
Assignment operator definition
*/
GameEngine& GameEngine::operator=(const GameEngine& other){
	//compare both addresses to check for self assignment
	if(this != &other){
		//since the assignment operator also does a deep copy, just use the copy constructor, and then move ownership to assignee.
		GameEngine tempEngine(other);
		states = std::move(tempEngine.states);
		playersMap = std::move(tempEngine.playersMap);
		orderOfPlay = std::move(tempEngine.orderOfPlay);
		numPlayersInGame = std::move(tempEngine.numPlayersInGame);
		currPhase = tempEngine.currPhase;
		currState = tempEngine.currState;
	}
	return *this;

}

void GameEngine::startupPhase(CommandProcessor& commandProcessor) {
	currState = states->at(StateID::Start).get();
	std::cout << "Beginning startup, entering state:  " << *this->getState() << "\n";

	while (true) {
		if(this->currPhase == Phase::play){
			break;
		}

		commandProcessor.readCommand();          
		Command* cmd = commandProcessor.getCommand();
		bool isValidCommand = commandProcessor.validate(cmd, this->getState()->getID());

		if (!isValidCommand) {
			cout << "Invalid command for State " << this->getState()->getStateName() << ".\n";
			continue;
		}
		if (cmd->getCommandName() == "exit") {
			std::cout << "Exiting command processor...\n";
			delete cmd;
			break;
		}
		string effect = this->getState()->onCommand(cmd, *this);
		cmd->saveEffect(effect);
		delete cmd;
	}
	
}

void GameEngine::tournamentMode(std::string args, GameEngine& engine) {
	std::cout << "In tournament mode..." << std::endl;

	bool containsMapArg = std::string::npos != args.find("-M");
	bool containsPlayerArg = std::string::npos != args.find("-P");
	bool containsGamesArg = std::string::npos != args.find("-G");
	bool containsMaxTurnsArg = std::string::npos != args.find("-D");

	//Validate all necessary arguments are there
	bool isValidCommand = containsMapArg && containsPlayerArg && containsGamesArg && containsMaxTurnsArg;

	if (!isValidCommand) {
		std::cout << "Missing necessary arguments. Ending...";
		return;
	}

	std::vector<Map> mapList;
	std::vector<unique_ptr<PlayerStrategy>> playerStrategyList;
	int numGames = -1;
	int maxNumTurns = -1;

	std::string currentArg = "N/A";	//This will be set, and when it is all currentValues will be added to that arguments variable
	std::string currentValue = "";	//Current value being constructed by chars to be added to the respective variable

	//Parse the argument data into variables mapList, playerStrategyList, numGames and maxNumTurns
	std::string mapFilePath = "";
	std::vector<char> argChars(args.begin(), args.end());
	for (int i = 0; i < argChars.size(); i++) {
		char currentChar = argChars[i];

		if (currentChar == '-' && i < argChars.size() - 1) {
			char argType = argChars[i + 1];
			currentArg.clear();
			currentArg += currentChar;
			currentArg += argType;
			i += 1; //This moves the index over to the argType, so that when we continue the next iteration starts after the argType
			continue;
		}
		if (currentArg == "N/A")
			continue;
		//Parse data into Maps
		if (currentArg == "-M") {

			if (currentChar == ' ' && currentValue.size() != 0) {
				Map mapElement(mapFilePath + currentValue);
				mapList.push_back(mapElement);
				this->tournamentMapNames.push_back(currentValue);
				currentValue.clear();
			}

			//If there is a space at the beginning
			if (currentChar == ' ')
				continue;

			currentValue += currentChar;

		}
		//Parse data into PlayerStrategies
		else if (currentArg == "-P") {

			if (currentChar == ' ' && currentValue.size() != 0) {
				if (currentValue == "Neutral") {
					playerStrategyList.push_back(make_unique<NeutralPlayer>());
					this->tournamentPlayerStrategies.push_back("Neutral");
				}
				else if (currentValue == "Cheater") {
					playerStrategyList.push_back(make_unique<CheaterPlayer>());
					this->tournamentPlayerStrategies.push_back("Cheater");
				}
				else if (currentValue == "Human") {
					playerStrategyList.push_back(make_unique<HumanPlayer>());
					this->tournamentPlayerStrategies.push_back("Human");
				}
				else if (currentValue == "Aggressive") {
					playerStrategyList.push_back(make_unique<AggressivePlayer>());
					this->tournamentPlayerStrategies.push_back("Aggressive");
				}
				else if (currentValue == "Benevolent") {
					playerStrategyList.push_back(make_unique<BenevolentPlayer>());
					this->tournamentPlayerStrategies.push_back("Benevolent");
				}
				else {
					std::cout << "Invalid Player Strategy. Exiting..." << std::endl;
					return;
				}
				currentValue.clear();
			}

			//If there is a space at the beginning
			if (currentChar == ' ')
				continue;

			currentValue += currentChar;
		}
		//Parse data into numGames
		else if (currentArg == "-G") {

			if (currentChar == ' ' && currentValue.size() != 0) {
				numGames = std::stoi(currentValue);
				this->tournamentNumGames = numGames;
				currentValue = "";
				continue;
			}

			//If there is a space at the beginning, or the number of games has already been set, continue.
			else if (currentChar == ' ' || numGames != -1)
				continue;

			bool charIsDigit = isdigit(currentChar);
			if (charIsDigit) {
				currentValue += currentChar;
			}
			else {
				std::cout << "Game arg is not a digit. Exiting..." << std::endl;
				return;
			}
		}
		//Parse data into maxNumTurns
		else if (currentArg == "-D") {

			//If we reach the end of the string
			if (i == argChars.size() - 1 && currentChar != ' ') {
				bool charIsDigit = isdigit(currentChar);

				if (charIsDigit) {
					currentValue += currentChar;
					maxNumTurns = std::stoi(currentValue);
					this->tournamentNumRounds = maxNumTurns;
					currentValue.clear();
					continue;
				}
				else {
					std::cout << "Max Turns arg is not a digit. Exiting..." << std::endl;
					return;
				}
			}
			//If you reach the end of a word
			else if (currentChar == ' ' && currentValue.size() != 0) {
				maxNumTurns = std::stoi(currentValue);
				currentValue.clear();
				continue;
			}

			//If there is a space at the beginning, or the number of turns has already been set, continue.
			if (currentChar == ' ' || maxNumTurns != -1)
				continue;

			bool charIsDigit = isdigit(currentChar);
			if (charIsDigit) {
				currentValue += currentChar;
			}
			else {
				std::cout << "Max Turns arg is not a digit. Exiting..." << std::endl;
				return;
			}
		}
		else {
			std::cout << "Error, unknown argument. Exiting..." << std::endl;
			return;
		}

	}

	//Validate all map files
	if (mapList.size() < 1 || mapList.size() > 5) {
		std::cout << "Invalid number of maps. Must be between 1-5. Exiting..." << std::endl;
		return;
	}
	for (Map map : mapList) {
		bool result = map.validate();

		if (!result) {
			std::cout << "Invalid map file(s). Exiting..." << std::endl;
			return;
		}
	}

	//Validating number of player strategies
	if (playerStrategyList.size() < 2 || playerStrategyList.size() > 4) {
		std::cout << "Invalid number of player strategies. Must be between 2-4. Exiting..." << std::endl;
		return;
	}

	//Validating number of games
	if (numGames < 1 || numGames > 5) {
		std::cout << "Invalid number of games. Must be between 1-5. Exiting..." << std::endl;
		return;
	}

	//Validating number of max turns
	if (maxNumTurns < 10 || maxNumTurns > 50) {
		std::cout << "Invalid number of max turns. Must be between 10-50. Exiting..." << std::endl;
		return;
	}

	//Create players and assign the player strategies to them
	std::vector<std::shared_ptr<Player>> players;
	for (int i = 0; i < playerStrategyList.size(); i++) {
		std::shared_ptr<Player> p = make_shared<Player>();
		p->setStrategy(std::move(playerStrategyList.at(i)));
		players.push_back(p);
		this->playersMap->emplace(i, p);
		this->orderOfPlay->push_back(i);
	}

	shuffleOrderOfPlay();

	//Tournament Game Loop
	for (int game = 0; game < numGames; game++) {
		for (int mapNum = 0; mapNum < mapList.size(); mapNum++) {
			this->currMap = make_shared<Map>(mapList.at(mapNum));
			//Fairly assign territories to all players
			assignTerritoriesFairly();

			for (int round = 0; round < maxNumTurns; round++) {
				reinforcmentPhase();
				issueOrdersPhase();
				executeOrdersPhase();

				vector<int> playersToRemove;
				for (const int id : *this->orderOfPlay) {
					if (playersMap->at(id)->getTerritories().size() == currMap->getAdjacencyMatrix().size()) {
						std::cout << "PLAYER " << id << " WON!.\n";
						this->tournamentWinners.push_back(playersMap->at(id)->getStrategyName());
						break;
					}
					else if (playersMap->at(id)->getTerritories().size() == 0) {
						playersToRemove.push_back(id);
					}

				}
				for (int id : playersToRemove) {
					std::cout << "PLAYER " << id << " IS HOMELESS, REMOVING THAT PLAYER!.\n";
					playersMap->erase(id);
					auto it = std::find(orderOfPlay->begin(), orderOfPlay->end(), id);
					if (it != orderOfPlay->end()) orderOfPlay->erase(it); //checking in case

				}
				std::cout << "\n Next turn ...\n\n";
			}
			this->tournamentWinners.push_back("Draw");
			std::cout << "Hit the max (" << maxNumTurns << ") number of turns..." << std::endl;
		}
	}

	this->tournamentOver = true;
	this->observer->update(*this);
}


void GameEngine::mainGameLoop() {
	while(true){
		reinforcmentPhase();
		issueOrdersPhase();
		executeOrdersPhase();
		
		/*
		if checkWin() -> check if a player owns all territories: that player calls win()
		if anyHomelessPlayers() -> check if there is a player with no territories: those players are removed from the game.
		*/
		vector<int> playersToRemove;
		for(const int id : *this->orderOfPlay){
			if(playersMap->at(id)->getTerritories().size() == currMap->getAdjacencyMatrix().size()){
				std::cout << "PLAYER " << id << " WON!.\n";
				this->tournamentWinners.push_back(playersMap->at(id)->getStrategyName());
				return;
			}
			else if(playersMap->at(id)->getTerritories().size() == 0){
				playersToRemove.push_back(id);
			}

		}
		for(int id : playersToRemove){
			std::cout << "PLAYER " << id << " IS HOMELESS, REMOVING THAT PLAYER!.\n";
			playersMap->erase(id);
			auto it = std::find(orderOfPlay->begin(), orderOfPlay->end(), id);
			if(it != orderOfPlay->end()) orderOfPlay->erase(it); //checking in case

		}
		std::cout << "\n Next turn ...\n\n";
	}
}



void GameEngine::reinforcmentPhase() {
	this->transitionState(StateID::AssignReinforcements);
	this->getState()->onCommand(nullptr, *this);
}

void GameEngine::issueOrdersPhase() {
	this->transitionState(StateID::IssueOrders);
	this->getState()->onCommand(nullptr, *this);
}

void GameEngine::executeOrdersPhase() {
	this->transitionState(StateID::ExecuteOrders);
	this->getState()->onCommand(nullptr, *this);
}

std::string GameEngine::stringToLog() const {
	std::string logText;
	if (this->tournamentOver) {
		logText = "Tournament Mode:\n";
		logText += "M: ";
		for (std::string map : this->tournamentMapNames)
			logText += map + ", ";
		logText += "\n";
		logText += "P: ";
		for (std::string strat : this->tournamentPlayerStrategies)
			logText += strat + ", ";
		logText += "\n";
		logText += "G: " + std::to_string(this->tournamentNumGames) + "\n";
		logText += "D: " + std::to_string(this->tournamentNumRounds) + "\n";
		logText += "Results:\n";
		int winnerIndex = 0;
		for (int i = 0; i < this->tournamentNumGames; i++) {
			logText += "Game: " + std::to_string(i) + "\n";
			for (int j = 0; j < this->tournamentMapNames.size(); j++) {
				logText += "\t" + tournamentMapNames.at(j) + ": " + this->tournamentWinners.at(winnerIndex) + "\n";
				winnerIndex++;
			}
		}
		return logText;
	}
	else {
		State* currentState = this->currState;

		std::string stateString = currentState->getStateName();

		logText = "GameEngine's new state: " + stateString;

		return logText;
	}
}
void GameEngine::attach(std::shared_ptr<LogObserver> pObserver) {
	this->observer = pObserver;
}
void GameEngine::detach() {
	this->observer = nullptr;
}
void GameEngine::notify(ILoggable& loggable) const {
	this->observer->update(loggable);
}


/*
Stream operator definition
*/ 
std::ostream& operator<<(std::ostream& stream, const GameEngine& engine) {

	stream << "Engine at address: " << &engine << "\nCurrent State: " << *engine.getState() << "\n";

	return stream;

}


/*
Initialize the currState to be the startState, basically the entry point of the game.
*/
void GameEngine::init() {
	/*
	State* currState now observes the states->at(StateID::Start). 
	But the memory of what it is pointing to is managed by the smartPtr: unique_ptr<map<StateID, unique_ptr<State>>> states;
	*/
	currState = states->at(StateID::Start).get();
	std::cout << "\nBeginning startup state: " << this->getState() << "\n";
}
/*
Return the currentState
*/
State* GameEngine::getState() const { return currState; }

/*
Apply State transition.
*/
void GameEngine::transitionState(StateID id) {
	if (states->count(id) > 0) {
		currState = states->at(id).get();
		//==================NOTIFY LOGS ================================
		std::cout << "\nTransitionned to state: " << *this->getState() << "\n";
		this->notify(*this);
	}
}

void GameEngine::printPlayersInGame() {

	std::cout << "LISTING ALL PLAYERS IN THE GAME: \n\n";
	for (const int id : *this->orderOfPlay) {
		std::cout << "Player " << id << " is " << *this->playersMap.get()->at(id) << "\n";

	}

}
void GameEngine::loadMap() {
	namespace fs = std::filesystem;
	//fs::path dir_path = "map/map_files/";
	fs::path dir_path = "./";

	std::cout << "\nChoose from the following maps: \n\n";
	for(auto& entry : fs::directory_iterator(dir_path)){
		//stem only takes everythign before .map
		std::cout << entry.path().filename().stem().string() << "\n";
	}

	std::shared_ptr<Map> map;
	string mapName{};
    do {
        std::cout << "\nChoose a map (enter the full name):\n";
        std::getline(std::cin >> std::ws, mapName);
		map = std::make_shared<Map>(dir_path.string() + mapName + ".map");

		if(!map.get()->validFile){
			//map contstructor prints an error message
			continue;

		}
		else{
			currMap = map;
			std::cout << "Map is loaded into the game\n";
			return;
		}

    } while (true);
}

void GameEngine::addPlayerToGame() {
	int newPlayerNum = getNumPlayersInGame() + 1;
	setNumPlayersInGame(newPlayerNum);
	this->playersMap->emplace(newPlayerNum, make_shared<Player>());
	this->playersMap->at(newPlayerNum)->getOrders().attach(this->observer);
	this->orderOfPlay->push_back(newPlayerNum);

}

void GameEngine::assignTerritoriesFairly(){
	vector<shared_ptr<Map::Territory>> allTerritories;
	//adjacency : territoryName, TerritoryPointer, auto takes care of it.
	for (const auto& p : currMap->getAdjacencyMatrix()) {
		allTerritories.push_back(p.second);
	}

	auto it = playersMap->begin();
	for(int i = 0; i < allTerritories.size(); i++){
		Player* currentPlayer = it->second.get();
		currentPlayer->addTerritory(allTerritories[i]);
		it++;
		if(it == playersMap->end()) it = playersMap->begin();
	}
	std::cout << "finished assigning territories fairly.\n";

}

void GameEngine::shuffleOrderOfPlay() {

	std::cout << "\nShuffling the order of play...\n";
	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(orderOfPlay->begin(), orderOfPlay->end(), gen);

	std::cout << "new order of play: ";
	for (const int id : *orderOfPlay) {
		std::cout << id << " ";
	}
	std::cout << "\n\n";

}

void GameEngine::assignUnitsToPlayer(int units, int playerIdInMap) {
	this->playersMap.get()->at(playerIdInMap).get()->assignReinforcments(units);

}

void GameEngine::createDeck() {
	this->deckOfCards = make_unique<Deck>();
}

void GameEngine::playerDrawsCard(int playerIdInMap) {
	this->deckOfCards.get()->draw(*this->playersMap.get()->at(playerIdInMap).get()->getHand());
}


//getters
std::map<int, std::shared_ptr<Player>>& GameEngine::getPlayersMap() {
    return *playersMap;
}

std::shared_ptr<Map> GameEngine::getCurrMap() const {
    return currMap;
}

int GameEngine::getNumPlayersInGame() {
    return *numPlayersInGame.get();
}

Phase GameEngine::getCurrPhase() const {
    return currPhase;
}

vector<int>* GameEngine::getOrderOfPlay() {
    return orderOfPlay.get();
}

Deck* GameEngine::getDeckOfCards() const {
    return deckOfCards.get();
}

//setters
void GameEngine::setCurrPhase(Phase newPhase) {
    currPhase = newPhase;
}

void GameEngine::setNumPlayersInGame(int currNumOfPlayers){

	*this->numPlayersInGame = currNumOfPlayers;
}

