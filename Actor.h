#ifndef ACTOR_H_
#define ACTOR_H_
//using namespace std; //why does this give me errors? 

#include "GraphObject.h"
#include "StudentWorld.h"
class Compiler;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

GraphObject::Direction randDir(); 

//							ACTOR													 ////
////////////////////////////////////////////////////////////////////////////////////////
class Actor : public GraphObject
{
public:								 
	Actor(StudentWorld* ptr, int imageID, int startX, int startY, Direction dir, unsigned int depth, double size)
		:GraphObject(imageID, startX, startY, dir, depth, size) //last parameter is size
	{
		m_x= startX;
		m_y= startY;
		pw = ptr; 
		m_moved = false;
		m_dead = false;
	}

	int x() const;
	int y() const;
	void setX(int newX);
	void setY(int newY);

	virtual void doSomething() = 0;
	virtual bool canBlockStuff() const;	

	bool didIMove() const;			 
	bool isDead() const; 
	virtual bool isFood() const;
	virtual bool isWaterPool() const;
	virtual bool isPoison() const;
	virtual bool isPher() const;
	virtual bool isEnemy(int colony) const;
	virtual bool isInsect() const;
	virtual bool isDangerous(int colony) const;

	virtual void getBitten(int damage); 

	void setDead(); //my student world uses this for updating food D: 
	
protected:  
	void setDidIMove(bool value); //use this if object has a capability of moving 
	StudentWorld* getStudentWorld();


private: 
	StudentWorld* pw; //every actor has a pointer to the (student) world 
	int m_x; 
	int m_y; 
	bool m_moved; 
	bool m_dead; 
};

//							ENERGY HOLDER											 ////
////////////////////////////////////////////////////////////////////////////////////////

class EnergyHolder : public Actor
{
public:
	EnergyHolder(StudentWorld* ptr, int imageID, int startX, int startY, Direction dir, unsigned int depth, double size)
		: Actor(ptr, imageID, startX, startY, dir, depth, size)
	{
		m_HP = 0;
	}
	//virtual void doSomething = 0 inherits this from Actor, making it an ABC
	virtual void updateHP(int num);
	virtual int getHP() const;
private:
	int m_HP;
};


//							INSECT													 ////
////////////////////////////////////////////////////////////////////////////////////////
class Insect : public EnergyHolder
{
public:
	Insect(StudentWorld* ptr, int imageID, int startX, int startY, unsigned int depth, double size)
		:EnergyHolder(ptr, imageID, startX, startY, randDir(), depth, size) //insects always start out facing a random direction
	{
		ticksToSleep = 0;  //this is what's common across all three insects
		m_regenAsAdult = false;
		justStunned = false;
		//tickLastStunned = -1; 
	}

	virtual void doSomething();
	bool regeneratesAsAdult();
	bool regeneratesAsFood(); 
	virtual void getStunned();  //added virtual. these two are overridden for adult to just return 
	virtual void getPoisoned();
	virtual void getBitten(int damage); //only one that's inherited from actor class 
	virtual bool isInsect() const; 
	virtual bool isDangerous(int colony) const; 

protected:

	void updateXandY(GraphObject::Direction dir); //not necessary if I just use moveTo! 
	void updateXandY(int newX, int newY);
	
	void setRegenAsAdult(bool val); 
	void setTicksToSleep(int a);
	int getTicksToSleep(); 
	bool attemptToEat(int num);
	GraphObject::Direction oppDir(GraphObject::Direction dir) const;

private:
	int ticksToSleep;
	virtual void doDifferentStuff() = 0;
	bool m_regenAsAdult;
	//int tickLastStunned; 
	bool justStunned; 
};

//							BABY GRASSHOPPER										 ////
////////////////////////////////////////////////////////////////////////////////////////

class BabyGrasshopper : public Insect
{
public:
	BabyGrasshopper(StudentWorld* ptr, int imageID, int startX, int startY, double size)
		:Insect(ptr, imageID, startX, startY, 1, size) //depth is 1 for baby grasshopper
	{
		setStepsToWalk(randInt(2, 10));
		updateHP(500);  //change to 500 //why does it molt so fast? beacuse HP-- when HP is only 20 -> molts when only twenty ticks have passed		
	}

protected:
	void attemptAStep();
	void setStepsToWalk(int a);
	int getStepsToWalk() const;
	//void setRegenerates(bool val); 
	virtual bool isEnemy(int colony) const; //both adults and babies are considered enemies to an ant 
	
private:
	virtual void doDifferentStuff();
	int stepsToWalk;
	

};
//							ADULT GRASSHOPPER										 ////
////////////////////////////////////////////////////////////////////////////////////////
class AdultGrasshopper : public BabyGrasshopper
{
public:
	AdultGrasshopper(StudentWorld* ptr, int imageID, int startX, int startY, double size)
		:BabyGrasshopper(ptr, imageID, startX, startY, size) //depth is 1
	{
		updateHP(1600); //change later to 1600
		m_radius = 10;
	}
	virtual void getStunned(); //should just return 
	virtual void getPoisoned(); //should just return
	virtual void getBitten(int damage); //should attempt to bite back
private:
	virtual void doDifferentStuff();
	void jump();
	bool outOfBounds(int x, int y) const;
	int m_radius;
	bool attemptToBite(); 
};

//						 DERIVED ENERGY HOLDERS											 ////
////////////////////////////////////////////////////////////////////////////////////////

class Food : public EnergyHolder
{
public:
	Food(StudentWorld* ptr, int imageID, int startX, int startY, int initialEnergy)
		:EnergyHolder(ptr, IID_FOOD, startX, startY, right, 2, 0.25) //depth, then size
	{
		updateHP(initialEnergy); //either 6000 if totally new, or 100 if from insect dying, or some other value from ant dropping food
	}

private:
	virtual void doSomething();
	virtual bool isFood() const;
};

class Anthill : public EnergyHolder
{
public:
	Anthill(StudentWorld* ptr, int imageID, int startX, int startY, Compiler* cptr, int colonyNum)
		:EnergyHolder(ptr, IID_ANT_HILL, startX, startY, right, 2, 0.25)
	{
		updateHP(8999); //change to 8999
		m_colony = colonyNum; 
		m_cptr = cptr; //compiler pointer 
	}
	virtual void doSomething(); 

private:
	int m_colony; 
	Compiler* m_cptr; 
	bool attemptToEat(int num);
	void giveBirth(); 
};


class Pheromone : public EnergyHolder
{
public: 
	Pheromone(StudentWorld* ptr, int imageID, int startX, int startY, int colonyNum)
		:EnergyHolder(ptr, imageID, startX, startY, right, 2, 0.25)
	{
		updateHP(256); 
		m_colonyNum = colonyNum; 
	}
	virtual void doSomething(); 
	virtual bool isPher() const;
	int colony() const;
private:
	int m_colonyNum; 
};

//							NONENERGY HOLDERS										 ////
////////////////////////////////////////////////////////////////////////////////////////
class NonEnergyHolder : public Actor
{
public:
	NonEnergyHolder(StudentWorld* ptr, int imageID, int startX, int startY, Direction dir, unsigned int depth, double size)
		: Actor(ptr, imageID, startX, startY, dir, depth, size)
	{}
	//virtual void doSomething() = 0 inherited from Actor, making it an ABC
};


class Pebble : public NonEnergyHolder
{
public:
	Pebble(StudentWorld* ptr, int imageID, int startX, int startY)
		:NonEnergyHolder(ptr, IID_ROCK, startX, startY, right, 1, 0.25) // depth then size. an initially made rock will always have a rock ID, right direction, and depth of 1 
	{}
	virtual void doSomething();
	virtual bool canBlockStuff() const;
};


class Poison : public NonEnergyHolder
{
public: 
	Poison(StudentWorld* ptr, int imageID, int startX, int startY)
		: NonEnergyHolder(ptr, IID_POISON, startX, startY, right, 2, 0.25) //depth, then size
	{}
	virtual bool isPoison() const; 
	virtual void doSomething();
private:
	
};

class Pool : public NonEnergyHolder
{
public:
	Pool(StudentWorld* ptr, int imageID, int startX, int startY)
		: NonEnergyHolder(ptr, IID_WATER_POOL, startX, startY, right, 2, 0.25) //depth, then size
	{}
	virtual bool isWaterPool() const; 
	virtual void doSomething();
private:
	
};

//										ANT											 ////
////////////////////////////////////////////////////////////////////////////////////////
class Ant : public Insect
{
public:
	Ant(StudentWorld* ptr, int imageID, int startX, int startY, Compiler* cptr, int colonyNum)
		:Insect(ptr, imageID, startX, startY, 1, 0.25)
	{
		updateHP(1500);
		m_ic = 0; //instruction counter starts at zero
		m_colonyNum = colonyNum;
		m_cptr = cptr;
		m_foodOnHand = 0; 
		anthillX = startX; 
		anthillY = startY; 
		m_randNum = 0; 
		wasBlocked = false;
		//was not previously bitten or blocked from moving
	}
	virtual bool isEnemy(int colony) const;
	virtual bool isDangerous(int colony) const; 
	virtual void getBitten(int damage); //should set wasBitten to true for an ant, bite-back for a grasshopper
private:
	virtual void doDifferentStuff();
	int m_colonyNum;
	Compiler* m_cptr;
	int m_ic;
	bool interpret(); 
	int m_foodOnHand; 
	int anthillX; 
	int anthillY; 
	int m_randNum; 
	bool wasBlocked;
	bool wasBitten;

	bool moveForward(); 
	void faceRandDir(); 
	void pickUpFood();
	void dropFood(); 
	void eatFood(); 
	void rotateClockwise(); 
	void rotateCounterClockwise(); 
	void emitPher(); 
	bool conditionTriggered (string s); 
	bool bite(); 
	
};

#endif // ACTOR_H_
