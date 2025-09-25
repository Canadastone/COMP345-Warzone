#include "GameEngine.h"

using std::string;
using std::vector;
using std::unique_ptr;
using std::make_unique;

//State definitions
State::State() = default;
State::State(const State& s) {}
State::~State() = default;

//State& operator=(const State&) {}
std::ostream& operator<<(std::ostream& stream, const State& s) {

	stream << "State: " << s.getStateName() << " | validCommands: ";
	for (auto& command : s.getValidCommands()) {
		stream << command << " ";
	}

	return stream;
}

//StateTemplate definitions
template<StateID ID>
StateTemplate<ID>& StateTemplate<ID>::operator=(const StateTemplate<ID>& other) {
	if (this == &other) return *this;
	//if they both arent null, do the swap.
	if (this->stateName && other.stateName) {
		//deep copy what other.stateName holds (a string) into the this->stateName ptr. Basically reusing the pointer but deep copying the string.
		*this->stateName = *other.stateName;
	}
	//create new stateName if other has one but this one doesnt.
	else if (other.stateName) {
		this->stateName = make_unique<string>(*other.stateName);
		//same as unique_ptr<string>(new string(*other.stateName))
	}
	else {
		this->stateName.reset(); // clear this.stateName if other is null
	}

	//Same thing for validCommands
	if (this->validCommands && other.validCommands) {
		*this->validCommands = *other.validCommands;
	}

	else if (other.validCommands) {
		this->validCommands = make_unique<unordered_set<string>>(*other.validCommands);
	}
	else {
		this->validCommands.reset();
	}

	return *this;
}

/*
Same definition for every: StateTemplate constructor, getID and getStateName accross all specialized states.
*/
template<StateID ID>
StateTemplate<ID>::StateTemplate() {

}

template<StateID ID>
StateTemplate<ID>::StateTemplate(stateInfo& info) : 
	stateName(make_unique<string>(info.name)),
	validCommands(make_unique<unordered_set<string>>(info.stateValidCommands)) {}


/*
Copy contructor; deep copies other into this state by checking whether other members are null.
*/
template<StateID ID>
StateTemplate<ID>::StateTemplate(const StateTemplate& other) :
	stateName(other.stateName ? make_unique<string>(*other.stateName) : nullptr),
	validCommands(other.validCommands ? make_unique<unordered_set<string>>(*other.validCommands) : nullptr){


}

template<StateID ID>
StateID StateTemplate<ID>::getID() const {
	return ID;
}

template<StateID ID>
string& StateTemplate<ID>::getStateName() const{
	return *stateName;
}

template<StateID ID>
unordered_set<string>& StateTemplate<ID>::getValidCommands() const {
	return *validCommands;
}


/*
Custom defintion of onEnter and onCommand for each specialized state.
*/

/*
Start State
*/
void StateTemplate<StateID::Start>::onEnter(GameEngine& engine) {

	/*
	Do something later for Start
	*/

	std::cout << "entering " << *this << "\n";
}
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
void StateTemplate<StateID::MapLoaded>::onEnter(GameEngine& engine) {

	/*
	Do something later for mapLoaded
	*/

	std::cout << "entering " << *this << "\n";
}
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
void StateTemplate<StateID::MapValidated>::onEnter(GameEngine& engine) {

	/*
	Do something later for MapValidated
	*/

	std::cout << "entering " << *this << "\n";
}
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
void StateTemplate<StateID::PlayersAdded>::onEnter(GameEngine& engine) {

	/*
	Do something later for PlayersAdded
	*/

	std::cout << "entering " << *this << "\n";
}
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
void StateTemplate<StateID::AssignReinforcements>::onEnter(GameEngine& engine) {

	/*
	Do something later for AssignReinforcements
	*/

	std::cout << "entering " << *this << "\n";
}
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
void StateTemplate<StateID::IssueOrders>::onEnter(GameEngine& engine) {

	/*
	Do something later for IssueOrders
	*/

	std::cout << "entering " << *this << "\n";
}
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
void StateTemplate<StateID::ExecuteOrders>::onEnter(GameEngine& engine) {

	/*
	Do something later for ExecuteOrders
	*/

	std::cout << "entering " << *this << "\n";
}
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
void StateTemplate<StateID::Win>::onEnter(GameEngine& engine) {

	/*
	Do something later for Win
	*/

	std::cout << "entering " << *this << "\n";
}
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
void StateTemplate<StateID::End>::onEnter(GameEngine& engine) {

	/*
	Do something later for End
	*/

	std::cout << "Ending the Game.\n";
}
bool StateTemplate<StateID::End>::onCommand(string& cmd, GameEngine& engine) {
	return true;
}

/*
If a StateTemplate<StateID::Start> object is created in another .cpp file,
the compiler would not see the definitions of the template member functions above. 
Without these definitions the compilation would fail.

For normal classes, the compiler
can defer code generation for member functions, and the linker will
resolve the symbols from another object file at link time.

But since templates are instantiated at compile time and have no
runtime resolution. The following "explicit instantializations"
force the compiler to generate all the member functions above for every specialized class as seen below, 
and emit them into the object file so the linker sees them.
*/
template class StateTemplate<StateID::Start>;
template class StateTemplate<StateID::MapLoaded>;
template class StateTemplate<StateID::MapValidated>;
template class StateTemplate<StateID::PlayersAdded>;
template class StateTemplate<StateID::AssignReinforcements>;
template class StateTemplate<StateID::IssueOrders>;
template class StateTemplate<StateID::ExecuteOrders>;
template class StateTemplate<StateID::Win>;
template class StateTemplate<StateID::End>;


/*
Define the constructors for each class. They all call their respective StateTemplate constructors with stateInfo as argument.
*/
startState::startState(stateInfo& info) : StateTemplate(info) {}
mapLoadedState::mapLoadedState(stateInfo& info) : StateTemplate(info) {}
mapValidatedState::mapValidatedState(stateInfo& info) : StateTemplate(info) {}
playersAddedState::playersAddedState(stateInfo& info) : StateTemplate(info) {}
assignReinforcementState::assignReinforcementState(stateInfo& info) : StateTemplate(info) {}
issueOrdersState::issueOrdersState(stateInfo& info) : StateTemplate(info) {}
executeOrdersState::executeOrdersState(stateInfo& info) : StateTemplate(info) {}
winState::winState(stateInfo& info) : StateTemplate(info) {}
endState::endState(stateInfo& info) : StateTemplate(info) {}


/*
 instantiate every state object. 

 Since these objects are instantiated in the same compilation unit as the template defintions,
 the explicit instantializations on line 323 werent necessary, but I added them preemptively for when GameEngine moves to its own compilation unit.
*/
GameEngine::GameEngine() : 
	states{make_unique<map<StateID, unique_ptr<State>>>()},
	currState{ nullptr } {

	//stateInfo: stateName, {list of valid commands/actions}, stateID.
	stateInfo startInfo{ "start", {"loadMap"}, StateID::Start };
	states->emplace(StateID::Start, make_unique<startState>(startInfo));//create a uniqueptr and stores startState(stateInfo& info)

	stateInfo mapLoadedInfo{ "mapLoaded", { "loadMap", "validateMap" }, StateID::MapLoaded };
	states->emplace(StateID::MapLoaded, make_unique<mapLoadedState>(mapLoadedInfo));

	stateInfo mapValidatedInfo{ "mapValidated", { "addPlayer" }, StateID::MapValidated };
	states->emplace(StateID::MapValidated, make_unique<mapValidatedState>(mapValidatedInfo));

	stateInfo playersAddedInfo{ "playersAdded", { "addPlayer", "assignCountries" }, StateID::PlayersAdded };
	states->emplace(StateID::PlayersAdded, make_unique<playersAddedState>(playersAddedInfo));

	stateInfo assignReinforcementsInfo{ "assignReinforcements", { "issueOrder" }, StateID::AssignReinforcements };
	states->emplace(StateID::AssignReinforcements, make_unique<assignReinforcementState>(assignReinforcementsInfo));

	stateInfo issueOrdersInfo{ "issueOrders", { "issueOrder", "endIssueOrders" }, StateID::IssueOrders };
	states->emplace(StateID::IssueOrders, make_unique<issueOrdersState>(issueOrdersInfo));

	stateInfo executeOrdersInfo{ "executeOrders", { "execOrder", "endExecOrder", "win" }, StateID::ExecuteOrders };
	states->emplace(StateID::ExecuteOrders, make_unique<executeOrdersState>(executeOrdersInfo));

	stateInfo winInfo{ "win", { "play", "end" }, StateID::Win };
	states->emplace(StateID::Win, make_unique<winState>(winInfo));

	stateInfo endInfo{ "end", { "NA" }, StateID::End };
	states->emplace(StateID::End, make_unique<endState>(endInfo));

	//setState(StateID::MapLoaded);

}

/*
Initate the first state.
*/
void GameEngine::init() {
	//State* currState now observes the states->at(StateID::Start). But is managed by a smartPointer.
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



