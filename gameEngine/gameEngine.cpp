#include <filesystem>
#include "gameEngine.h"

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
	
	
	return effect;
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

	if (cmd->getCommandName() == "addplayer") {
		if (engine.getNumPlayersInGame() + 1 > 6) {
			effect = "can't add more than 6 players to the game.\n";
		}
		else {
			effect = "added player to the game, stayed in players added state.\n";
			engine.addPlayerToGame();
			std::cout << "number of players: " << engine.getNumPlayersInGame() << "\n";
			engine.transitionState(StateID::PlayersAdded);
		}
	}


	else if (cmd->getCommandName() == "gamestart") {
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
	for (const int id : *engine.getOrderOfPlay()) {
		// (# of territories owned divided by 3, rounded down
		// minimum units to add is 3.
		int unitsToAssign = engine.getPlayersMap().at(id)->getTerritories().size()/3;
		if(unitsToAssign < 3) unitsToAssign = 3;
	
		// TODO: if player controls continent, add extra units, how ?

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
	int orderCount;
	while (true) {
		orderCount = 0;

		for (const int id : *engine.getOrderOfPlay()) {
			std::cout << "Enter order for Player " << id << " (DEPLOY, ADVANCE, BOMB, BLOCKADE, AIRLIFT, NEGOTIATE), or blank/Enter to skip your turn: ";
			shared_ptr<orders::Order> order = nullptr;
			bool isValidOrder = true;
			do {
				std::string orderType;
				std::getline(std::cin, orderType);
				isValidOrder = true;

				if (orderType.empty()) continue;

				if (orderType == "BOMB") order = std::make_shared<orders::Bomb>();
				else if (orderType == "DEPLOY") order = std::make_shared<orders::Deploy>();
				else if (orderType == "ADVANCE") order = std::make_shared<orders::Advance>();
				else if (orderType == "BLOCKADE") order = std::make_shared<orders::Blockade>();
				else if (orderType == "AIRLIFT") order = std::make_shared<orders::Airlift>();
				else if (orderType == "NEGOTIATE") order = std::make_shared<orders::Negotiate>();
				else {
					orderType = "";
					std::cout << "Invalid Order, please try again: ";
					isValidOrder = false;
				}

			} while(!isValidOrder);

			if (order) {
				engine.getPlayersMap().at(id)->issueOrder(order);
				orderCount++;
			}
		}

		if (orderCount == 0) break;
	}
	
	return effect;
}


/*
Execute Orders State
*/

string executeOrdersState::onCommand(Command* cmd, GameEngine& engine) {
	string effect;
	bool anyOrdersExecuted = false;
	do{
		anyOrdersExecuted = false;
		for (const int id : *engine.getOrderOfPlay()) {
			auto& orders = engine.getPlayersMap().at(id)->getOrders();
			if(orders.size() == 0) continue;

			
			auto topOrder = orders[orders.size() - 1];
			std::cout << "executing player " << id << "'s " << topOrder->getTypeAsString() << " order.\n";
			topOrder->execute();
			orders.remove(orders.size() - 1);
			anyOrdersExecuted = true;
		}
	} while(anyOrdersExecuted);
	std::cout << "Successfully finished executing orders.\n";
	

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
	playersMap{ make_unique<map<int, unique_ptr<Player>>>() },
	numPlayersInGame(make_unique<int>(0)),
	currPhase(Phase::startup),
	orderOfPlay (make_unique<vector<int>>()){


	//allocate memory to every state and place them in the states map (key: stateID, val: unique_ptr to State object)
	states->emplace(StateID::Start, make_unique<startState>("start", unordered_set<string>{ "loadmap" })); // StateTemplate(const string& stateName, const unordered_set<string>& stateValidCommands);

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
	
	if(other.states){
		//creates a new pointer to a map, then deep copies the states from other into the new map by using the clone method polymorphically.
		states = make_unique<map<StateID, unique_ptr<State>>>();
		for(const std::pair<const StateID, unique_ptr<State>>& p : *other.states){
			(*states)[p.first] = p.second->clone();
		}
		//same thing for playersMap

		//Update currState to reference the same state (by ID) as in the original GameEngine
		currState = other.currState ? states->at(other.currState->getID()).get() : nullptr;

	}
	else{
		states = nullptr;
		currState = nullptr;
		playersMap = nullptr;
		currMap = nullptr;
	}
	
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
				return;
			}
			else if(playersMap->at(id)->getTerritories().size() == 0){
				playersToRemove.push_back(id);
			}

		}
		for(int id : playersToRemove){
			std::cout << "PLAYER " << id << " IS HOMELESS, REMOVING THAT PLATYER!.\n";
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
	State* currentState = this->currState;

	std::string stateString = currentState->getStateName();

	std::string logText = "GameEngine's new state: " + stateString;

	return logText;
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
Assignment operator definition
*/
GameEngine& GameEngine::operator=(const GameEngine& other){
	//compare both addresses to check for self assignment
	if(this != &other){
		//since the assignment operator also does a deep copy, just use the copy constructor, and then move ownership to assignee.
		GameEngine tempEngine(other);
		states = std::move(tempEngine.states);
		currState = tempEngine.currState;
	}
	return *this;

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
	fs::path dir_path = "map/map_files/";

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
	this->playersMap->emplace(newPlayerNum, make_unique<Player>());
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
std::map<int, std::unique_ptr<Player>>& GameEngine::getPlayersMap() {
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

