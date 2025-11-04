#pragma once
#include <string>
#include <iostream>
#include <map>
#include <memory>
#include <unordered_set>
#include <algorithm> 
#include <random>

#include "../player/player.h"

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
	//stream operator
	friend std::ostream& operator<<(std::ostream& os, const State& s);
	
	/*
	clone methods return a deep copy of the specific state
	This ensures polymorphic copying works correctly when cloning from a State pointer.
	*/
	virtual std::unique_ptr<State> clone() const = 0;
	virtual ~State();
};

/*
Generic base class for all the states with a parameter in the template for the stateID.
Goal is to reduce repeated code since every state will use the functions/variables defined in this template the exact same way for every state.
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
	//custom ctr that takes in info about states.
	StateTemplate(const string& stateName, const unordered_set<string>& stateValidCommands);

	//copy constructor
	StateTemplate(const StateTemplate& other);
	
	//overriden getters
	StateID getID() const override;
	string& getStateName() const override;
	unordered_set<string>& getValidCommands() const override;

	StateTemplate& operator=(const StateTemplate& other);
};

/*
Each derived class is defined here
Every derived state: 
	- uses the StateTemplate constructors
	- has a different onEnter, and onCommand implementation
	- needs a clone method unique to them.

Now, this design makes it easy to define custom behavior for each state
while reusing the shared template logic for common functionality.
*/

/*
Start state
*/
class startState : public StateTemplate<StateID::Start> {
public:
    using StateTemplate<StateID::Start>::StateTemplate;

    void onEnter(GameEngine& engine) override;
    bool onCommand(string& cmd, GameEngine& engine) override;
    unique_ptr<State> clone() const override;
};

/*
Map loaded state
*/
class mapLoadedState : public StateTemplate<StateID::MapLoaded> {
public:
    using StateTemplate<StateID::MapLoaded>::StateTemplate;
    void onEnter(GameEngine& engine) override;
    bool onCommand(string& cmd, GameEngine& engine) override;
	unique_ptr<State> clone() const override;
};

/*
Map validated state
*/
class mapValidatedState : public StateTemplate<StateID::MapValidated> {
public:
    using StateTemplate<StateID::MapValidated>::StateTemplate;
    void onEnter(GameEngine& engine) override;
    bool onCommand(string& cmd, GameEngine& engine) override;
	unique_ptr<State> clone() const override;
};

/*
Players Added State
*/
class playersAddedState : public StateTemplate<StateID::PlayersAdded> {
public:
    using StateTemplate<StateID::PlayersAdded>::StateTemplate;
    void onEnter(GameEngine& engine) override;
    bool onCommand(string& cmd, GameEngine& engine) override;
	unique_ptr<State> clone() const override;
};

/*
Assign Reinforcments State
*/
class assignReinforcementsState : public StateTemplate<StateID::AssignReinforcements> {
public:
    using StateTemplate<StateID::AssignReinforcements>::StateTemplate;
    void onEnter(GameEngine& engine) override;
    bool onCommand(string& cmd, GameEngine& engine) override;
	unique_ptr<State> clone() const override;
};
/*
Issue orders state
*/
class issueOrdersState : public StateTemplate<StateID::IssueOrders> {
public:
    using StateTemplate<StateID::IssueOrders>::StateTemplate;
    void onEnter(GameEngine& engine) override;
    bool onCommand(string& cmd, GameEngine& engine) override;
	unique_ptr<State> clone() const override;
};

/*
Execute orders state
*/ 
class executeOrdersState : public StateTemplate<StateID::ExecuteOrders> {
public:
    using StateTemplate<StateID::ExecuteOrders>::StateTemplate;
    void onEnter(GameEngine& engine) override;
    bool onCommand(string& cmd, GameEngine& engine) override;
	unique_ptr<State> clone() const override;
};
/*
Win state
*/ 
class winState : public StateTemplate<StateID::Win> {
public:
    using StateTemplate<StateID::Win>::StateTemplate;
    void onEnter(GameEngine& engine) override;
    bool onCommand(string& cmd, GameEngine& engine) override;
	unique_ptr<State> clone() const override;
};

/*
End state
*/ 
class endState : public StateTemplate<StateID::End> {
public:
    using StateTemplate<StateID::End>::StateTemplate;
    void onEnter(GameEngine& engine) override;
    bool onCommand(string& cmd, GameEngine& engine) override;
	unique_ptr<State> clone() const override;
};



enum class Phase {
	startup,
	play

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
	
	unique_ptr<map<int, unique_ptr<Player>>> playersMap;
	std::shared_ptr<Map> currMap;

	int numPlayersInGame;

	Phase currPhase;

	vector<int> orderOfPlay;

	unique_ptr<Deck> deckOfCards;

	//GameEngine ctr.
	GameEngine();

	//GameEngine copy ctr
	GameEngine(const GameEngine& other);

	//GameEngine assignment operator
	GameEngine& operator=(const GameEngine& other);

	//GameEngine streamInsertion operator
	friend std::ostream& operator<<(std::ostream& os, const GameEngine& s);

	//inits the startup phase as per part 2.
	void startupPhase();

	//helpers for startupPhase
	void printPlayersInGame();
	bool loadMap();
	void addPlayerToGame();
	void shuffleOrderOfPlay();
	void assignUnitsToPlayer(int units, int playerIdInMap);
	void createDeck();
	void playerDrawsCard(int playerIdInMap);





	void mainGameLoop();
	void reinforcmentPhase();
	void issueOrdersPhase();
	void executeOrdersPhase();

	//initializes the first state.
	void init();
	//returns the current State.
	State* getState() const;


	//Apply State transition.
	void transitionState(StateID id);

};
void testGameStates();
