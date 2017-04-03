#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_
using namespace std; //added this line
#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include "Field.h" //added this line 
#include "GraphObject.h" //added this line
#include <vector> //added this line
#include <map>

class Compiler;
class Actor; //don't need Actor.h, only using pointers

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

struct Coord
{
	int coord_x;
	int coord_y; 
};

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir); 

	virtual ~StudentWorld(); 
	virtual int init(); 
	virtual int move();
	virtual void cleanUp(); 
	
	void printMyMap(); //for debugging

	bool canMoveHere(int x, int y);
	int howMuchEaten(int x, int y, int num);  
	bool canBeStunnedHere(int x, int y); 
	bool canBePoisonedHere(int x, int y); 

	void addActorToSimulation(Actor *addMe);
	void increaseScore(int colonyNum); 
	bool isFoodHere(int x, int y); 
	bool isPherHere(int x, int y, int colony); 
	bool isDangerHere(int x, int y, int colony); 
	void addAPheromone(int x, int y, int colony); 
	bool updateExistingPher(Actor *p, int colony); 
	bool isEnemyAt(int x, int y, int colony); 
	bool biteEnemyAt(Actor*me, int colony, int biteDamage); 

private:
	// every StudentWorld has a container of pairings of a coordinate with a vector of actors
	//a map uses binary search to find a coordinate! niceee :) 
	std::map <Coord, std::vector <Actor*>> CoordToActorM;

	void updateTickCount() { tickCount++; }
	bool isSimulationOver() 
		{ return (tickCount >= 2000); }
	int tickCount;
	int numInitialAnthills;
	vector <string> fileNames;
	std::vector<Compiler*> myCompilers;
	std::vector <int> scores;

	Actor* getPtrToFood(Coord a);
	void updateActorLocations();
	void removeDeadActors();
	void updateDisplayText();
	void graphNew(GraphObject* a, int newX, int newY);
	Coord findMyLocation(Field &myfield, Field::FieldItem a) const;

	string formatMe();
	string to_string(int num); 
	int findLeadingAnt();
};

#endif // STUDENTWORLD_H_
