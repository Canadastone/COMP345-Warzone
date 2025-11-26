#pragma once
#include <string>
#include <iostream>
#include <map>
#include <memory>
#include <unordered_set>
#include <algorithm> 
#include <random>
#include "../processor/CommandProcessor.h"
#include "../logObserver/LoggingObserver.h"

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
State abstract class as a base interface for every other state in the game engine.
Define what all states must be able to do: onCommand, ...
*/
class State {


public:
	State();
	State(const State& s);

	/*
	fires when a command is triggered, and manages it.
	*/
	virtual string onCommand(Command* cmd, GameEngine&) = 0;

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

    string onCommand(Command* cmd, GameEngine& engine) override;
    unique_ptr<State> clone() const override;
};

/*
Tournament state
*/
class tournamentState : public StateTemplate<StateID::Tournament> {
public:
	using StateTemplate<StateID::Tournament>::StateTemplate;

	string onCommand(Command* cmd, GameEngine& engine) override;
	unique_ptr<State> clone() const override;
};

/*
Map loaded state
*/
class mapLoadedState : public StateTemplate<StateID::MapLoaded> {
public:
    using StateTemplate<StateID::MapLoaded>::StateTemplate;
    string onCommand(Command* cmd, GameEngine& engine) override;
	unique_ptr<State> clone() const override;
};

/*
Map validated state
*/
class mapValidatedState : public StateTemplate<StateID::MapValidated> {
public:
    using StateTemplate<StateID::MapValidated>::StateTemplate;
    string onCommand(Command* cmd, GameEngine& engine) override;
	unique_ptr<State> clone() const override;
};

/*
Players Added State
*/
class playersAddedState : public StateTemplate<StateID::PlayersAdded> {
public:
    using StateTemplate<StateID::PlayersAdded>::StateTemplate;
    string onCommand(Command* cmd, GameEngine& engine) override;
	unique_ptr<State> clone() const override;
};

/*
Assign Reinforcments State
*/
class assignReinforcementsState : public StateTemplate<StateID::AssignReinforcements> {
public:
    using StateTemplate<StateID::AssignReinforcements>::StateTemplate;
    string onCommand(Command* cmd, GameEngine& engine) override;
	unique_ptr<State> clone() const override;
};
/*
Issue orders state
*/
class issueOrdersState : public StateTemplate<StateID::IssueOrders> {
public:
    using StateTemplate<StateID::IssueOrders>::StateTemplate;
    string onCommand(Command* cmd, GameEngine& engine) override;
	unique_ptr<State> clone() const override;
};

/*
Execute orders state
*/ 
class executeOrdersState : public StateTemplate<StateID::ExecuteOrders> {
public:
    using StateTemplate<StateID::ExecuteOrders>::StateTemplate;
    string onCommand(Command* cmd, GameEngine& engine) override;
	unique_ptr<State> clone() const override;
};
/*
Win state
*/ 
class winState : public StateTemplate<StateID::Win> {
public:
    using StateTemplate<StateID::Win>::StateTemplate;
    string onCommand(Command* cmd, GameEngine& engine) override;
	unique_ptr<State> clone() const override;
};

/*
End state
*/ 
class endState : public StateTemplate<StateID::End> {
public:
    using StateTemplate<StateID::End>::StateTemplate;
    string onCommand(Command* cmd, GameEngine& engine) override;
	unique_ptr<State> clone() const override;
};



enum class Phase {
	startup,
	play

};


/*
Responsible for initalizing all the states, and for managing the game.
*/
class GameEngine : ILoggable, Subject {

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
	std::shared_ptr<LogObserver> observer;

	/*
	mapping an integer to a player
	*/
	unique_ptr<map<int, shared_ptr<Player>>> playersMap;

	/*
	using the int keys in playersMap to keep an order of play.
	*/
	unique_ptr<vector<int>> orderOfPlay;

	/*
	The current map object currently loaded in the game
	*/
	shared_ptr<Map> currMap;

	/*
	current number of player in the game
	*/
	unique_ptr<int> numPlayersInGame;

	/*
	current deck of cards object being used in the game
	*/
	unique_ptr<Deck> deckOfCards;

	/*
	current "global game phase" (Startup, play) as per A2 instructions.
	*/
	Phase currPhase;
	
public:
	
	//GameEngine ctr.
	GameEngine();

	//GameEngine copy ctr
	GameEngine(const GameEngine& other);

	//GameEngine assignment operator
	GameEngine& operator=(const GameEngine& other);

	//GameEngine streamInsertion operator
	friend std::ostream& operator<<(std::ostream& os, const GameEngine& s);


	//initializes the first state.
	void init();
	//returns the current State.
	State* getState() const;


	//Apply State transition.
	void transitionState(StateID id);
	
	
	//inits the startup phase as per part 2.
	void startupPhase(CommandProcessor& commandProcessor);

	//helpers for startupPhase

	/*
	calls the << operator for every play in the game
	*/
	void printPlayersInGame();

	/*
	promps the user to choose from a list of maps throught he command line, and loads the map into currMap
	*/
	void loadMap();

	/*
	Adds a player to the playersMap
	*/
	void addPlayerToGame();

	/*
	Assigns territories in round robin to each player
	*/
	void assignTerritoriesFairly();

	/*
	randomizes the indices in orderOfPlay
	*/
	void shuffleOrderOfPlay();

	/*
	Assigns the passed number of units to the player.
	*/
	void assignUnitsToPlayer(int units, int playerIdInMap);

	/*
	instantiates the deck object
	*/
	void createDeck();
	/*
	calls the player.draw() for a player.
	*/
	void playerDrawsCard(int playerIdInMap);

	/*
	handles functionality of tournament mode
	*/
	void tournamentMode(std::string args, GameEngine& engine);

	void mainGameLoop();
	void reinforcmentPhase();
	void issueOrdersPhase();
	void executeOrdersPhase();

	//getters
	map<int, shared_ptr<Player>>& getPlayersMap();
    shared_ptr<Map> getCurrMap() const;
    int getNumPlayersInGame();
    Phase getCurrPhase() const;
    vector<int>* getOrderOfPlay();
    Deck* getDeckOfCards() const;

	//setters
	void setCurrPhase(Phase newPhase);
	void setNumPlayersInGame(int currNumOfPlayers);

	//Implements ILoggable functions
	std::string stringToLog() const;
	//Implements Subject functions
	void attach(std::shared_ptr<LogObserver> pObserver);
	void detach();
	void notify(ILoggable& loggable) const;
};
void testStartupPhase();
void testTournament();