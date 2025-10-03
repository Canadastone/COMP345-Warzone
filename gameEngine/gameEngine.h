#pragma once
#include <string>
#include <iostream>
#include <map>
#include <memory>
#include <unordered_set>

using std::string;
using std::unique_ptr;
using std::make_unique;
using std::map;
using std::unordered_set;

/*
Forward declarations
*/
class GameEngine;

/*
Enum representing states, used as a parameter to the template classes below.
*/
enum class StateID {
	Start,
	MapLoaded,
	MapValidated,
	PlayersAdded,
	AssignReinforcements,
	IssueOrders,
	ExecuteOrders,
	Win,
	End
};
/*
State abstract class as a base interface for every other state in the game engine.
Define what all states must be able to do: onEnter, onCommand, ...
*/
class State {


public:
	State();
	State(const State& s);

	/*
	called when first entering a state.
	*/
	virtual void onEnter(GameEngine&) = 0;
	/*
	fires when a command is triggered, and manages it.
	*/
	virtual bool onCommand(string& cmd, GameEngine&) = 0;

	//getters
	virtual string& getStateName() const = 0;
	virtual StateID getID() const = 0;
	virtual unordered_set<string>& getValidCommands() const = 0;
	
	//output overload
	friend std::ostream& operator<<(std::ostream& os, const State& s);
	virtual ~State();
};

/*
Generic base class for all the states with a parameter in the template for the stateID.
Goal is to reduce repeated code since every state will implement a lot of the same functions but define them differently.
*/
template<StateID ID>
class StateTemplate : public State {
protected:
	//name of the state
	unique_ptr<string> stateName;
	//set of valid commands for this state
	unique_ptr<unordered_set<string>> validCommands;

public:
	//default ctr
	StateTemplate();
	//ctr takes in info about hardcoded states.
	StateTemplate(const string& stateName, const unordered_set<string>& stateValidCommands);

	//copy constructor
	StateTemplate(const StateTemplate& other);
	
	//overriden methods that are expected to work differently for each state.
	//therefore, they will be defined for each state.
	void onEnter(GameEngine& engine) override;
	bool onCommand(string& cmd, GameEngine& engine) override;

	//overriden getters
	StateID getID() const override;
	string& getStateName() const override;
	unordered_set<string>& getValidCommands() const override;

	StateTemplate& operator=(const StateTemplate& other);
};



/*
Responsible for initalizing all the states, and for managing the game.
*/
class GameEngine {

private:
	/*
	Represents the currentState of the game.
	no destructor needed for current state, since states uniquePointer will always own it and manage it.
	*/
	State* currState;
	/*
	represents the mapping of ID -> StateObj.
	*/
	unique_ptr<map<StateID, unique_ptr<State>>> states;
	
public:
	//GameEngine ctr.
	GameEngine();
	//initializes the first state.
	void init();
	//returns the current State.
	State* getState() const;


	//Apply State transition.
	void transitionState(StateID id);

};

