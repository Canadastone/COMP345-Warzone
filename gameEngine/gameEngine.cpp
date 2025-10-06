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

/*
Start State
*/
void startState::onEnter(GameEngine& engine) {

	/*
	Do something later for Start
	*/

	std::cout << "entering " << *this << "\n";
}

bool startState::onCommand(string& cmd, GameEngine& engine) {
	if (this->getValidCommands().count(cmd) > 0) {
		if (cmd == "loadMap") {
			engine.transitionState(StateID::MapLoaded);
		}
		return true;
	}

	return false;
}

unique_ptr<State> startState::clone() const {
    return unique_ptr<startState>(new startState(*this));
}

/*
Map Loaded State
*/
void mapLoadedState::onEnter(GameEngine& engine) {

	/*
	Do something later for mapLoaded
	*/

	std::cout << "entering " << *this << "\n";
}
bool mapLoadedState::onCommand(string& cmd, GameEngine& engine) {
	if (this->getValidCommands().count(cmd) > 0) {
		if (cmd == "loadMap") {
			engine.transitionState(StateID::MapLoaded);
		}
		else if (cmd == "validateMap") {
			engine.transitionState(StateID::MapValidated);
		}
		return true;
	}

	return false;
}

unique_ptr<State> mapLoadedState::clone() const {
    return unique_ptr<mapLoadedState>(new mapLoadedState(*this));
}

/*
Map Validated State
*/
void mapValidatedState::onEnter(GameEngine& engine) {

	/*
	Do something later for MapValidated
	*/

	std::cout << "entering " << *this << "\n";
}
bool mapValidatedState::onCommand(string& cmd, GameEngine& engine) {
	if (this->getValidCommands().count(cmd) > 0) {
		if (cmd == "addPlayer") {
			engine.transitionState(StateID::PlayersAdded);
		}
		return true;
	}

	return false;
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

	std::cout << "entering " << *this << "\n";
}

bool playersAddedState::onCommand(string& cmd, GameEngine& engine) {
	if (this->getValidCommands().count(cmd) > 0) {
		if (cmd == "addPlayer") {
			engine.transitionState(StateID::PlayersAdded);
		}
		else if (cmd == "assignCountries") {
			engine.transitionState(StateID::AssignReinforcements);
		}
		return true;
	}

	return false;
}

unique_ptr<State> playersAddedState::clone() const {
    return unique_ptr<playersAddedState>(new playersAddedState(*this));
}

/*
AssignReinforcments State
*/
void assignReinforcementsState::onEnter(GameEngine& engine) {

	/*
	Do something later for AssignReinforcements
	*/

	std::cout << "entering " << *this << "\n";
}

bool assignReinforcementsState::onCommand(string& cmd, GameEngine& engine) {
	if (cmd == "issueOrder") {
		engine.transitionState(StateID::IssueOrders);
		return true;
	}

	return false;
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

	std::cout << "entering " << *this << "\n";
}

bool issueOrdersState::onCommand(string& cmd, GameEngine& engine) {
	if (this->getValidCommands().count(cmd) > 0) {
		if (cmd == "issueOrder") {
			engine.transitionState(StateID::IssueOrders);
		}
		else if (cmd == "endIssueOrders") {
			engine.transitionState(StateID::ExecuteOrders);
		}
		return true;
	}

	return false;
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

	std::cout << "entering " << *this << "\n";
}

bool executeOrdersState::onCommand(string& cmd, GameEngine& engine) {
	if (this->getValidCommands().count(cmd) > 0) {
		if (cmd == "execOrder") {
			engine.transitionState(StateID::ExecuteOrders);
		}
		else if (cmd == "endExecOrders") {
			engine.transitionState(StateID::AssignReinforcements);
		}
		else if (cmd == "win") {
			engine.transitionState(StateID::Win);
		}
		return true;
	}

	return false;
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

	std::cout << "entering " << *this << "\n";
}

bool winState::onCommand(string& cmd, GameEngine& engine) {
	if (this->getValidCommands().count(cmd) > 0) {
		if (cmd == "play") {
			engine.transitionState(StateID::Start);
		}
		else if (cmd == "end") {
			engine.transitionState(StateID::End);
		}
		return true;
	}

	return false;
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
bool endState::onCommand(string& cmd, GameEngine& engine) {
	return true;
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
	currState{ nullptr } {


	//allocate memory to every state and place them in the states map (key: stateID, val: unique_ptr to State object)
	states->emplace(StateID::Start, make_unique<startState>("start", unordered_set<string>{ "loadMap" }));

	states->emplace(StateID::MapLoaded, make_unique<mapLoadedState>("mapLoaded", unordered_set<string>{ "loadMap", "validateMap" }));

	states->emplace(StateID::MapValidated, make_unique<mapValidatedState>("mapValidated", unordered_set<string>{ "addPlayer" }));

	states->emplace(StateID::PlayersAdded, make_unique<playersAddedState>("playersAdded", unordered_set<string>{ "addPlayer", "assignCountries" }));

	states->emplace(StateID::AssignReinforcements, make_unique<assignReinforcementsState>("assignReinforcements", unordered_set<string>{ "issueOrder" }));

	states->emplace(StateID::IssueOrders, make_unique<issueOrdersState>("issueOrders", unordered_set<string>{ "issueOrder", "endIssueOrders" }));

	states->emplace(StateID::ExecuteOrders, make_unique<executeOrdersState>("executeOrders", unordered_set<string>{ "execOrder", "endExecOrder", "win" }));

	states->emplace(StateID::Win, make_unique<winState>("win", unordered_set<string>{ "play", "end" }));

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
		//Update currState to reference the same state (by ID) as in the original GameEngine
		currState = other.currState ? states->at(other.currState->getID()).get() : nullptr;

	}
	else{
		states = nullptr;
		currState = nullptr;

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



