#include "GameEngine.h"

using std::string;
using std::unique_ptr;
using std::make_unique;

//useful alias's to reduce boilerplate
using startState = StateTemplate<StateID::Start>;
using mapLoadedState = StateTemplate<StateID::MapLoaded>;
using mapValidatedState = StateTemplate<StateID::MapValidated>;
using playersAddedState = StateTemplate<StateID::PlayersAdded>;
using assignReinforcementState = StateTemplate<StateID::AssignReinforcements>;
using issueOrdersState = StateTemplate<StateID::IssueOrders>;
using executeOrdersState = StateTemplate<StateID::ExecuteOrders>;
using winState = StateTemplate<StateID::Win>;
using endState = StateTemplate<StateID::End>;

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
	stateName(other.stateName ? make_unique<string>(*other.stateName) : nullptr),
	validCommands(other.validCommands ? make_unique<unordered_set<string>>(*other.validCommands) : nullptr) {


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
onEnter and onCommand behave differently depending on which state they are called on.
Define each function for the specific StateTemplate instantiation (specified by the ID) corresponding to that state.

template<> signifies an explicit specialization of a function template
*/

/*
Start State
*/
template<>
void StateTemplate<StateID::Start>::onEnter(GameEngine& engine) {

	/*
	Do something later for Start
	*/

	std::cout << "entering " << *this << "\n";
}
template<>
bool StateTemplate<StateID::Start>::onCommand(string& cmd, GameEngine& engine) {
	if (this->getValidCommands().count(cmd) > 0) {
		if (cmd == "loadMap") {
			engine.transitionState(StateID::MapLoaded);
		}
		return true;
	}

	return false;
}

/*
Map Loaded State
*/
template<>
void StateTemplate<StateID::MapLoaded>::onEnter(GameEngine& engine) {

	/*
	Do something later for mapLoaded
	*/

	std::cout << "entering " << *this << "\n";
}
template<>
bool StateTemplate<StateID::MapLoaded>::onCommand(string& cmd, GameEngine& engine) {
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

/*
Map Validated State
*/
template<>
void StateTemplate<StateID::MapValidated>::onEnter(GameEngine& engine) {

	/*
	Do something later for MapValidated
	*/

	std::cout << "entering " << *this << "\n";
}
template<>
bool StateTemplate<StateID::MapValidated>::onCommand(string& cmd, GameEngine& engine) {
	if (this->getValidCommands().count(cmd) > 0) {
		if (cmd == "addPlayer") {
			engine.transitionState(StateID::PlayersAdded);
		}
		return true;
	}

	return false;
}

/*
Players Added State
*/
template<>
void StateTemplate<StateID::PlayersAdded>::onEnter(GameEngine& engine) {

	/*
	Do something later for PlayersAdded
	*/

	std::cout << "entering " << *this << "\n";
}
template<>
bool StateTemplate<StateID::PlayersAdded>::onCommand(string& cmd, GameEngine& engine) {
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

/*
AssignReinforcments State
*/
template<>
void StateTemplate<StateID::AssignReinforcements>::onEnter(GameEngine& engine) {

	/*
	Do something later for AssignReinforcements
	*/

	std::cout << "entering " << *this << "\n";
}
template<>
bool StateTemplate<StateID::AssignReinforcements>::onCommand(string& cmd, GameEngine& engine) {
	if (cmd == "issueOrder") {
		engine.transitionState(StateID::IssueOrders);
		return true;
	}

	return false;
}

/*
Issue Orders State
*/
template<>
void StateTemplate<StateID::IssueOrders>::onEnter(GameEngine& engine) {

	/*
	Do something later for IssueOrders
	*/

	std::cout << "entering " << *this << "\n";
}
template<>
bool StateTemplate<StateID::IssueOrders>::onCommand(string& cmd, GameEngine& engine) {
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

/*
Execute Orders State
*/
template<>
void StateTemplate<StateID::ExecuteOrders>::onEnter(GameEngine& engine) {

	/*
	Do something later for ExecuteOrders
	*/

	std::cout << "entering " << *this << "\n";
}
template<>
bool StateTemplate<StateID::ExecuteOrders>::onCommand(string& cmd, GameEngine& engine) {
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

/*
Win State
*/
template<>
void StateTemplate<StateID::Win>::onEnter(GameEngine& engine) {

	/*
	Do something later for Win
	*/

	std::cout << "entering " << *this << "\n";
}
template<>
bool StateTemplate<StateID::Win>::onCommand(string& cmd, GameEngine& engine) {
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

/*
End State
*/
template<>
void StateTemplate<StateID::End>::onEnter(GameEngine& engine) {

	/*
	Do something later for End
	*/

	std::cout << "Ending the Game.\n";
}
template<>
bool StateTemplate<StateID::End>::onCommand(string& cmd, GameEngine& engine) {
	return true;
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

	states->emplace(StateID::AssignReinforcements, make_unique<assignReinforcementState>("assignReinforcements", unordered_set<string>{ "issueOrder" }));

	states->emplace(StateID::IssueOrders, make_unique<issueOrdersState>("issueOrders", unordered_set<string>{ "issueOrder", "endIssueOrders" }));

	states->emplace(StateID::ExecuteOrders, make_unique<executeOrdersState>("executeOrders", unordered_set<string>{ "execOrder", "endExecOrder", "win" }));

	states->emplace(StateID::Win, make_unique<winState>("win", unordered_set<string>{ "play", "end" }));

	states->emplace(StateID::End, make_unique<endState>("end", unordered_set<string>{ "NA" }));

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



