#include "GameEngine.h"

//abstract State constructors definitions
State::State() = default;
State::State(const State& s) {}
State::~State() = default;

//superclass's operator<< definition. State& s is resolved at runtime, and the correct derived class member functions are called.
std::ostream& operator<<(std::ostream& stream, const State& s) {

	stream << "State: " << s.getStateName() << " | validCommands: ";
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
onEnter and onCommand have state specific behavior, they are defined here for every specific state.

clone methods return a deep copy of the specific state
This ensures polymorphic copying works correctly when cloning from a State pointer.
*/


// ===========================================STARTUP=========================================

/*
Start State
*/
void startState::onEnter(GameEngine& engine) {

	std::cout << "\nentering " << *this << "\n\n";

	
}

string startState::onCommand(Command* cmd, GameEngine& engine) {
	string effect;

	if (cmd->getCommandName() == "loadmap") {
		if(!engine.loadMap()){
			effect = "can't load map transitioning back to start state.\n";
			engine.transitionState(StateID::Start);
		}
		else{
			effect = "loaded map, proceeding to map loaded state.\n";
			engine.transitionState(StateID::MapLoaded);
		}
	}
	
	
	return effect;
}

unique_ptr<State> startState::clone() const {
    return unique_ptr<startState>(new startState(*this));
}

/*
Map Loaded State
*/
void mapLoadedState::onEnter(GameEngine& engine) {

	std::cout << "\nentering " << *this << "\n";
	

}
string mapLoadedState::onCommand(Command* cmd, GameEngine& engine) {
	string effect;
	
	if (cmd->getCommandName() == "loadmap") {
		effect = "transitionning to load map state\n";
		engine.transitionState(StateID::MapLoaded);
	}
	else if (cmd->getCommandName() == "validatemap") {

		if (engine.currMap->validate()) {
			effect = "map is valid, transitioning to map validated state\n";
			engine.transitionState(StateID::MapValidated);
		}
		else {
			effect = "map is invalid, for now just doing nothing\n";
			return effect;
		}

			
	}
	return effect;
}

unique_ptr<State> mapLoadedState::clone() const {
    return unique_ptr<mapLoadedState>(new mapLoadedState(*this));
}

/*
Map Validated State
*/
void mapValidatedState::onEnter(GameEngine& engine) {


	std::cout << "\nentering " << *this << "\n";
}
string mapValidatedState::onCommand(Command* cmd, GameEngine& engine) {
	string effect;
	
	if (cmd->getCommandName() == "addplayer") {
		effect = "adding player to the game, and transitioning to players added state\n";
		engine.addPlayerToGame();
		engine.transitionState(StateID::PlayersAdded);
	}

	return effect;
}

unique_ptr<State> mapValidatedState::clone() const {
    return unique_ptr<mapValidatedState>(new mapValidatedState(*this));
}

/*
Players Added State
*/
void playersAddedState::onEnter(GameEngine& engine) {

	/*
	Do something later for PlayersAdded
	*/

	std::cout << "\nentering " << *this << "\n";
	std::cout << "current number of players: " << engine.numPlayersInGame << "\n";
}

string playersAddedState::onCommand(Command* cmd, GameEngine& engine) {
	string effect;

	if (cmd->getCommandName() == "addplayer") {
		if (engine.numPlayersInGame + 1 > 6) {
			std::cout << "Too many players, proceeding to AssignReinforcments Phase...\n";
			engine.transitionState(StateID::AssignReinforcements);

		}
		else {
			effect = "adding player to the game, staying in players added state.\n";
			engine.addPlayerToGame();
			engine.transitionState(StateID::PlayersAdded);
		}
	}


	else if (cmd->getCommandName() == "gamestart") {
		if (engine.numPlayersInGame < 2) {
			std::cout << "Minimum of 2 players to proceed, currently: " << engine.numPlayersInGame << " players.\n";
			effect = "Minimum of 2 players required, currently: " + std::to_string(engine.numPlayersInGame) + " players. Going back to players added state.\n";
			engine.transitionState(StateID::PlayersAdded);
		}
		else{
			engine.printPlayersInGame();
			
			std::cout << "GAMESTART PHASE SETUP ...\n\n";

			//a) distribute all territories fairly somehow
			std::cout << "assign territories evenly to all players...\n\n";

			//b) determine order
			engine.shuffleOrderOfPlay();

			//c) give 50 army units to each player
			std::cout << "Assigning 50 units to each player.\n\n";
			for (const int id : engine.orderOfPlay) {
				engine.assignUnitsToPlayer(50, id);
			}

			//d) each player draws 2 cards using draw()
			//for now just create a deck on the stack
			engine.createDeck();

			for (const int id : engine.orderOfPlay) {
				std::cout << "Player with ID: " << id << " draws twice...\n";
				engine.playerDrawsCard(id);
				engine.playerDrawsCard(id);
				std::cout << "\n";

			}

			//e) switch game to play phase.
			engine.currPhase = Phase::play;
			
			effect = "game startup phase initiated, game phase changed to play, and transitionning to assign reinforcments phase.\n";
			engine.transitionState(StateID::AssignReinforcements);
		}
	}


	return effect;
}

unique_ptr<State> playersAddedState::clone() const {
    return unique_ptr<playersAddedState>(new playersAddedState(*this));
}


//====================================================PLAY====================================================


/*
AssignReinforcments State
*/
void assignReinforcementsState::onEnter(GameEngine& engine) {

	/*
	Do something later for AssignReinforcements
	*/

	std::cout << "\nentering " << *this << "\n";
	

}

string assignReinforcementsState::onCommand(Command* cmd, GameEngine& engine) {
	string effect;
	
	if (cmd->getCommandName() == "issueorder") {
		engine.transitionState(StateID::IssueOrders);
		
	}

	return effect;
}

unique_ptr<State> assignReinforcementsState::clone() const {
    return unique_ptr<assignReinforcementsState>(new assignReinforcementsState(*this));
}

/*
Issue Orders State
*/
void issueOrdersState::onEnter(GameEngine& engine) {

	/*
	Do something later for IssueOrders
	*/

	std::cout << "\nentering " << *this << "\n";
}

string issueOrdersState::onCommand(Command* cmd, GameEngine& engine) {
	string effect;

	if (cmd->getCommandName() == "issueOrder") {
		engine.transitionState(StateID::IssueOrders);
	}
	else if (cmd->getCommandName() == "issueordersend") {
		engine.transitionState(StateID::ExecuteOrders);
	}
	
	return effect;
}

unique_ptr<State> issueOrdersState::clone() const {
    return unique_ptr<issueOrdersState>(new issueOrdersState(*this));
}

/*
Execute Orders State
*/
void executeOrdersState::onEnter(GameEngine& engine) {

	/*
	Do something later for ExecuteOrders
	*/

	std::cout << "\nentering " << *this << "\n";
}

string executeOrdersState::onCommand(Command* cmd, GameEngine& engine) {
	string effect;

	if (cmd->getCommandName() == "execorder") {
		engine.transitionState(StateID::ExecuteOrders);
	}
	else if (cmd->getCommandName() == "endexecorders") {
		engine.transitionState(StateID::AssignReinforcements);
	}
	else if (cmd->getCommandName() == "win") {
		engine.transitionState(StateID::Win);
	}
		
	

	return effect;
}

unique_ptr<State> executeOrdersState::clone() const {
    return unique_ptr<executeOrdersState>(new executeOrdersState(*this));
}

/*
Win State
*/
void winState::onEnter(GameEngine& engine) {

	/*
	Do something later for Win
	*/

	std::cout << "\nentering " << *this << "\n";
}

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

unique_ptr<State> winState::clone() const {
    return unique_ptr<winState>(new winState(*this));
}

/*
End State
*/
void endState::onEnter(GameEngine& engine) {

	/*
	Do something later for End
	*/

	std::cout << "Ending the Game.\n";
}
string endState::onCommand(Command* cmd, GameEngine& engine) {
	string effect;
	return effect;
}

unique_ptr<State> endState::clone() const {
    return unique_ptr<endState>(new endState(*this));
}

/*
 GameEngine class, that manages all the game structure by keeping track of all the states, and the current state.
*/

/*

	std::shared_ptr<Map> currMap;
	unique_ptr<map<int, unique_ptr<Player>>> players;
*/
GameEngine::GameEngine() : 
	//allocate memory for the states smart_ptr
	states{make_unique<map<StateID, unique_ptr<State>>>()},
	//currState is initially null
	currState{ nullptr },
	//currMap is initially null
	currMap{nullptr},
	//allocate memory for the players
	playersMap{ make_unique<map<int, unique_ptr<Player>>>() },
	numPlayersInGame (0),
	currPhase(Phase::startup){


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

void GameEngine::startupPhase() {
	currState = states->at(StateID::Start).get();
	currState->onEnter(*this);
	
}

/*
void GameEngine::mainGameLoop() {
	//In the order of playters from 4b in part 2, proceed in round-robin:
	for (Player in playersOrderList) {

		reinforcmentPhase(Player)
	
	}

	for (Player in playersOrderList) {

		std::cout << Player.reinforcmentPool << "\n";

	}
	std::cout << "\n";

}
*/

/*
void GameEngine::reinforcmentPhase(Player) {

	ownedTerritories = Player.ownedTerritories;
	armyUnitsGiven = ownedTerritories / 3;

	if Player owns all territories in a Continent:
		armyUnitsGiven += continentsBonus;

	if armyUnitsGiven < 3:
		armyUnitsGiven = 3;
		Player.reinforcmentPool = armyUnitsGiven;

}*/

void GameEngine::issueOrdersPhase() {

}

void GameEngine::executeOrdersPhase() {

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
	currState->onEnter(*this);
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
		currState->onEnter(*this);
	}
}

void GameEngine::printPlayersInGame() {

	std::cout << "LISTING ALL PLAYERS IN THE GAME: \n\n";
	for (const int id : this->orderOfPlay) {
		std::cout << "Player " << id << " is " << *this->playersMap.get()->at(id) << "\n";

	}

}

bool GameEngine::loadMap() {
	std::shared_ptr<Map> asia = std::make_shared<Map>("map/map_files/Asia.map");
	if(!asia.get()->validFile){
		return false;

	}

	this->currMap = asia;
	std::cout << "Map is loaded into the game\n\n";
	return true;

}
void GameEngine::addPlayerToGame() {
	this->playersMap->emplace(++this->numPlayersInGame, make_unique<Player>());
	this->orderOfPlay.push_back(this->numPlayersInGame);

}

void GameEngine::shuffleOrderOfPlay() {

	std::cout << "\nShuffling the order of play...\n";
	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(this->orderOfPlay.begin(), this->orderOfPlay.end(), gen);

	std::cout << "new order of play: ";
	for (const int id : this->orderOfPlay) {
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


