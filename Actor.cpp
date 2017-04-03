#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>
#include "Compiler.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

GraphObject::Direction randDir() //is this okay to go here? 
{
	int c = randInt(1, 4);
	switch (c)
	{
	case 1: return GraphObject::right;
	case 2: return GraphObject::left;
	case 3: return GraphObject::up;
	case 4: return GraphObject::down;
	}
	return GraphObject::right; //shouldn't ever be called if randInt works correctly
}

/////////////////////////////////////////////////////////////////////////////////////////
//							ACTOR IMPLEMENATION										////
////////////////////////////////////////////////////////////////////////////////////////
int Actor::x() const
{
	return m_x;
}

int Actor::y() const
{
	return m_y; 
}

void Actor::setX(int newX)
{
	m_x= newX;
}

void Actor::setY(int newY)
{
	m_y = newY;
}

StudentWorld* Actor::getStudentWorld()
{
	return pw;
}

bool Actor::didIMove() const
{
	return m_moved;
}

void Actor::setDidIMove(bool value)
{
	m_moved = value;
}

bool Actor::canBlockStuff() const
{
	return false;
}

bool Actor::isDead() const
{	
	return m_dead; 
}

void Actor::setDead()
{
	//cout << "Actor at (" << x() << ", " << y() << ") is dead! delete me pls\n"; 
	m_dead = true; 
}

bool Actor::isFood() const
{
	return false;
}
//
//void Actor::getStunned()
//{
//	return; 
//}
//
//void Actor::getPoisoned()
//{
//	return;
//}

void Actor::getBitten(int damage)
{
	return; 
}

bool Actor::isWaterPool() const
{
	return false;
}

bool Actor::isPoison() const
{
	return false;
}

bool Actor::isPher() const
{
	return false;
}

bool Actor::isEnemy(int colony) const
{
	return false;
}

bool Actor::isInsect() const
{
	return false;
}

bool Actor::isDangerous(int colony) const
{
	return false;
}

//ENERGY HOLDER 
void EnergyHolder::updateHP(int num)
{
	m_HP += num;  
}

int EnergyHolder::getHP() const
{
	return m_HP; 
}


/////////////////////////////////////////////////////////////////////////////////////////
//							INSECT IMPLEMENATION									 ////
////////////////////////////////////////////////////////////////////////////////////////

void Insect::doSomething()
{
	updateHP(-1); 
	setDidIMove(false);
	
	if (getHP() <= 0) //fix! if baby grasshopper reaches 0, doesn't regenerate. 
	{
		//set to dead, add 100 units of food to this location //student world does this for me 
		//cout << "Insect at " << x() << ", " << y() << "has <=0 HP, so I died" << endl;
		setDead(); 
		return; 
	}

	if (ticksToSleep != 0)
	{
		//cout << "Insect at " << x() << ", " << y() << "still has " << ticksToSleep << " ticks to sleep\n"; 
		ticksToSleep--;
		return;
	}
	else
		doDifferentStuff(); 

	if (didIMove())
	{
		if(getStudentWorld()->canBeStunnedHere(x(), y()))
			getStunned();

		if (getStudentWorld()->canBePoisonedHere(x(), y()))
			getPoisoned(); 
	}
		

}

void Insect::updateXandY(GraphObject::Direction dir) //I don't need virtual, right? I'm not specializing this function between ants or grasshoppers. 
{
	switch (dir)
	{
	case right:
		setX(x() + 1); break;
	case left:
		setX(x() - 1); break;
	case up:
		setY(y() + 1); break;
	case down:
		setY(y() - 1); break;
	}
}

void Insect::updateXandY(int newX, int newY)
{
	setX(newX); 
	setY(newY);

	//could do this->moveTo()
}

int Insect::getTicksToSleep()
{
	return ticksToSleep;
}

void Insect::setTicksToSleep(int a) 
{
	ticksToSleep = a;
}

bool Insect::attemptToEat(int num)
{
	int temp = getHP(); 
	//updateHP(getStudentWorld()->addOrRemoveFoodHere(x(), y(), -num)); //returns how much was eaten
	updateHP(getStudentWorld()->howMuchEaten(x(), y(), num));
	//cout << "Grasshopper at " << x() << " " << y() << "has HP" << getHP() << endl;
	return (temp != getHP()); //if they're not equal, then eating was successful 
}

bool Insect::regeneratesAsAdult()  
{
	return m_regenAsAdult;
}

void Insect::setRegenAsAdult(bool val)
{
	m_regenAsAdult = val;
}

bool Insect::regeneratesAsFood()
{
	return true;
}

bool Insect::isDangerous(int colony) const
{
	return true;
}

 
void Insect::getStunned() //??? how do I prevent a grasshopper from being restunned?  //an adult grasshopper cannot be stunned, so i wrote another virtual function for adults
{
	//cout << "An insect just moved onto a water pool at " << x() << " , " << y() << endl;
	//cout << "Ticks to sleep was " << getTicksToSleep() << "and now it is ";
	setTicksToSleep(getTicksToSleep() + 2);
	//cout << getTicksToSleep() << endl;
}

void Insect::getPoisoned() //an adult grasshopper cannot be poisoned, so I overrode that later on 
{	
	//cout << "I called getPoisoned for an insect at " << x() << ", " << y() << endl; 
	//cout << "HP for the insect changed from " << getHP() << " to "; 
	updateHP(-150); 
	//cout << getHP() << endl;
	/*if (getHP() <= 0) //this should happen automatically the next time the insect's do something is called - avoid repeats? 
		setDead(); *///student world should automatically add food here 
}

void Insect::getBitten(int damage)
{
	//cout << "An insect got bit at " << x() << ", " << y() << "! Prev HP: " << getHP() << endl;
	updateHP(-damage); 
	////cout << "After being bitten, HP is " << getHP() << endl;
}

bool Insect::isInsect() const
{
	return true;
}

GraphObject::Direction Insect::oppDir(GraphObject::Direction dir) const
{
	switch (dir)
	{
	case right:
		return left;
	case left:
		return right;
	case up:
		return down;
	case down:
		return up;
	}
	return right;
}

/////////////////////////////////////////////////////////////////////////////////////////
//							BABY GRASSHOPPER IMPLEMENATION							 ////
////////////////////////////////////////////////////////////////////////////////////////

void BabyGrasshopper::setStepsToWalk(int a)
{
	stepsToWalk = a;
}

int BabyGrasshopper::getStepsToWalk() const
{
	return stepsToWalk;
}


bool BabyGrasshopper::isEnemy(int colony) const
{
	return true;
}


void BabyGrasshopper::doDifferentStuff()
{
	//std::cout << "I called doSomething for Grasshopper at coordinate (" << x() << ", " << y() << ") \n";

	 if (getHP() >= 1600)
	{
		//molt and change into adult grasshopper, add 100 units of food

		setRegenAsAdult(true); //regenerate as food is automatically true 
		//getStudentWorld()->addOrRemoveFoodHere(x(), y(), 100); 
		setDead(); 
		return;
	} 

	//attempt to eat

	 if (attemptToEat(200)) 
	 {
		 //cout << "Grasshopper at " << x() << " " << y() << "now has HP" << getHP() << endl;
		 if (randInt(1, 2) == 1)
		 {
			 setTicksToSleep(2);
			 return;
		 }	 
	 }

	 attemptAStep(); 

	 setTicksToSleep(2); 
}

void BabyGrasshopper::attemptAStep()
{
	GraphObject::Direction dir;

	if (getStepsToWalk() > 0)
		dir = getDirection(); //keep walking in same direction
	else
	{
		dir = randDir();
		setStepsToWalk(randInt(2, 10));
		setDirection(dir); //when I call my graphNew function in studentworld, I will see the change in direction. 
	}

	updateXandY(dir);  //will update new x and y locations

	if (!getStudentWorld()->canMoveHere(x(), y()))	//if you can't move to the new location because something's blocking it, take step in opp dir
	{
		updateXandY(oppDir(dir)); //undo what I just did...bad programming I know I know 
		setStepsToWalk(0);
	}
	else
	{
		//setStepsToWalk(-1);
		setStepsToWalk(getStepsToWalk() - 1); 
		setDidIMove(true);
	}
}


/////////////////////////////////////////////////////////////////////////////////////////
//							ADULT GRASSHOPPER IMPLEMENATION							////
////////////////////////////////////////////////////////////////////////////////////////
void AdultGrasshopper::doDifferentStuff()
{
	//bite, jump, eat

	if (randInt(1, 3) == 1)
	{
		if (attemptToBite())
			return;
	}
		

	if (randInt(1,10) == 1) //you have a 1 in 10 chance of jumping
		jump(); 
	else
	{
		if (attemptToEat(200) && randInt(1, 2) == 1)
		{
			setTicksToSleep(2);
			return; 
		}

		attemptAStep(); 
	}

	setTicksToSleep(2);
}

bool AdultGrasshopper::attemptToBite()
{
	//cout << "an adult grasshopper at " << x() << ", " << y() << " is attempting to bite!" << endl;
	return getStudentWorld()->biteEnemyAt(this, -2, 50); //pass in dummy value for colony # 
}

void AdultGrasshopper::getBitten(int damage)
{
	Insect::getBitten(damage); 
	if (getHP() < 0)
	{
		setDead(); 
		return; //might cause double the food to be added? probably not tho because on same square. 
	}

	else if (randInt(1,2) == 1) //if still alive, 50% chance of biting. 
		attemptToBite(); 
}

void AdultGrasshopper::jump() //is there a way to make my grasshoppers pivot before they jump? 
{
	const double pi = 3.14159265; 
	double newAngle = randInt(0, 360) / 2 / pi; //results in a double
	int newX = x() + m_radius * cos(newAngle);  
	int newY = y() + m_radius * sin(newAngle); 

	if (outOfBounds(newX, newY) || !getStudentWorld()->canMoveHere(newX, newY)) //try jumping again if out of bounds or blocked by a pebble
	{
		jump(); //is this bad practice? but it's so nice and clean 
	}
	else
		updateXandY(newX, newY); //student world will move my adult grasshopper for me
	setDidIMove(true); 
}

bool AdultGrasshopper::outOfBounds(int x, int y) const
{
	if (x > 63 || x < 0 || y > 63 || y < 0) //63 or 64? i think 63. this is where a pebble would be located anyway. 
		return true;
	return false;
}

void AdultGrasshopper::getPoisoned()
{
	return; 
}
void AdultGrasshopper::getStunned()
{
	return;
}

/////////////////////////////////////////////////////////////////////////////////////////
//							PEBBLE IMPLEMENATION									 ////
////////////////////////////////////////////////////////////////////////////////////////

void Pebble::doSomething() 
{
	//std::cout << "I called dosomething for Rock\n"; 
	return;
}

bool Pebble::canBlockStuff() const
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
//							FOOD IMPLEMENATION										 ////
////////////////////////////////////////////////////////////////////////////////////////

void Food::doSomething()
{
	return;
}

bool Food::isFood() const
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
//							POOL IMPLEMENATION										 ////
////////////////////////////////////////////////////////////////////////////////////////

bool Pool::isWaterPool() const
{
	return true;
}

void Pool::doSomething()
{
	//getStudentWorld()->stunIfStunnable(x(), y()); 
	return; 
}

/////////////////////////////////////////////////////////////////////////////////////////
//							POISON IMPLEMENATION									 ////
////////////////////////////////////////////////////////////////////////////////////////

void Poison::doSomething()
{
	//getStudentWorld()->poisonifPoisonable(x(), y()); 
	return; 
}

bool Poison::isPoison() const
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
//							PHEROMONE IMPLEMENATION									////
////////////////////////////////////////////////////////////////////////////////////////

void Pheromone::doSomething()
{
	updateHP(-1); 
	if (getHP() <= 0)
		setDead(); 
}

bool Pheromone::isPher() const
{
	return true;
}

int Pheromone::colony() const
{
	return m_colonyNum; 
}

/////////////////////////////////////////////////////////////////////////////////////////
//							ANTHILL IMPLEMENATION									 ////
////////////////////////////////////////////////////////////////////////////////////////
 
void Anthill::doSomething()
{
	updateHP(-1); 
	//cout << "\nIn anthill's doSomething, My anthill at " << x() << ", " << y() << " is part of colony " << m_colony << endl;
//	cout << "I called doSomething for my Anthill" << "which has HP " << getHP() << endl;
	if (getHP() <= 0)
	{
		//cout << "My anthill died!" << endl;
		setDead(); 
		return; 
	}

	if (attemptToEat(10000))
		return; 

	if (getHP() >= 2000)
		giveBirth(); 
}

void Anthill::giveBirth()
{
	//cout << "My Anthill HP is " << getHP() << " which >= 2000 so I'm giving birth for my colony " << m_colony << endl;
	int imageID; 
	
	if (m_colony == 0)
		imageID = IID_ANT_TYPE0;
	else if (m_colony == 1)
		imageID = IID_ANT_TYPE1;
	else if (m_colony == 2)
		imageID = IID_ANT_TYPE2;
	else if (m_colony == 3)
		imageID = IID_ANT_TYPE3;
	else
		imageID = IID_ANT_TYPE0; //shouldn't ever be called but just in case 

	Ant *newAnt = new Ant(getStudentWorld(), imageID, x(), y(), m_cptr, m_colony);
	updateHP(-1500);

	getStudentWorld()->addActorToSimulation(newAnt);  //put that new Ant into my data structure 
	getStudentWorld()->increaseScore(m_colony); 
}

bool Anthill::attemptToEat(int num)
{
	//cout << "Anthill at " << x() << " " << y() << "currently has HP" << getHP() << endl;
	int temp = getHP();
	updateHP(getStudentWorld()->howMuchEaten(x(), y(), num)); //ask student world if there's food there, and if so, then eat
	
	//cout << "Anthill at " << x() << " " << y() << ", after attempting to eat, has HP" << getHP() << endl;

	return (temp != getHP()); //if they're not equal, then at least some eating was successful 
}

/////////////////////////////////////////////////////////////////////////////////////////
//							ANT IMPLEMENATION										 ////
////////////////////////////////////////////////////////////////////////////////////////

void Ant::doDifferentStuff()
{
	//fetch the next command, interpret it, and execute it 
	//cout << endl << "I called doDiffStuff for my ant at " << x() << ", " << y() << endl;
	
	//for debugging *****************************
	//Compiler::Command cmdprint; 
	//int counter = 0;
	/*cout << "My vector has: " << endl;
	while (m_cptr->getCommand(counter, cmdprint))
	{
	cout << cmdprint.text << endl;
	counter++;
	}
	cout << endl << endl;*/
	//*****************************************
	interpret(); 
	return;
}

bool Ant::interpret()
{
	Compiler::Command cmd;

	int count = 0;  //number of times a command runs (I won't count the times where the ant changes the simulation, because then you would immediately
				//return, so I wouldn't have to keep track of the # of commands) 
	
	while (count !=10)
	{
		if (!m_cptr->getCommand(m_ic, cmd)) //if m_ic got too large? 
		{
			m_ic = 0; //reset and keep going? 
			if (!m_cptr->getCommand(m_ic, cmd)) //if even getCommand(0, cmd) doesn't work, then you return false;
				return false;				//otherwise, you can now keep going with the switch statement until your COUNT becomes too large.
		}
			
		//cout << cmd.text << "is being executed for the ant at " << x() << y() << endl;

		switch (cmd.opcode)
		{
		case (Compiler::Opcode::moveForward) :
		{	
			//cout << "moveFoward is being executed when ic is " << m_ic<< "\n"; 
			++m_ic; 
			if (moveForward())  //the spec says to return true if the ant attempted to move, even if it couldn't actually move. 
			{
				wasBlocked = false;
				wasBitten = false;
			}
			else
				wasBlocked = true;
			return true;
		}
		
		case(Compiler::Opcode::eatFood) :
		{
			++m_ic;
			eatFood();
			return true;
		}

		case(Compiler::Opcode::dropFood) :
		{
			++m_ic;
			dropFood();
			return true;
		}
		case(Compiler::Opcode::bite) :
		{
			++m_ic;
			bite();
			return true;
		}
		case(Compiler::Opcode::pickupFood) :
		{
			//cout << "pickUpFood is being executed when ic is " << m_ic << "\n";
			++m_ic;
			pickUpFood();
			return true;
		}
		case(Compiler::Opcode::emitPheromone) :
		{
			++m_ic;
			emitPher();
			return true;
		}


		case (Compiler::Opcode::faceRandomDirection) :
		{
			//cout << "faceRandomDir is being executed when ic is " << m_ic << "\n";
			++m_ic; 
			faceRandDir(); 
			return true;
		}
		case(Compiler::Opcode::rotateClockwise) :
		{
			++m_ic;
			rotateClockwise();
			return true;
		}

		case(Compiler::Opcode::rotateCounterClockwise) :
		{
			++m_ic;
			rotateCounterClockwise();
			return true;
		}
		case(Compiler::Opcode::generateRandomNumber) :
		{
			++m_ic;
			m_randNum = randInt(0, stoi(cmd.operand1) - 1);
			count++; 
			break;
		}


		case(Compiler::Opcode::goto_command) :
		{
			//cout << "goto_command is being executed when ic is " << m_ic << "\n";
			m_ic = stoi(cmd.operand1);
			//cout << "I jumped to m_ic " << m_ic << endl;
			count++; 
			break; 
		}

		case(Compiler::Opcode::if_command) :
		{
			//cout << "if Command is being executed for " << cmd.operand1 << " when ic is " << m_ic << endl;
			if (conditionTriggered(cmd.operand1))
			{
				//cout << "triggered" << endl;
				m_ic = stoi(cmd.operand2); //the spec says operand 2? 
			}

			else
			{
				++m_ic;
				//cout << "Not triggered" << endl;
			}

			count++;
			break;
		}
		}
		if (count == 10) //runs at the end of each while loop. count increments for generateRandNum, goto, and if
			return false; //do i need this if I have the return false below? would quit the while loop 
	}

	return false; 
}

bool Ant::bite()
{
	if (getStudentWorld()->isEnemyAt(x(), y(), m_colonyNum))
		return getStudentWorld()->biteEnemyAt(this, m_colonyNum, 15); 
	return false; //added this 
}

bool Ant::moveForward() //need to account for if it was already blocked by a pebble
{
	updateXandY(this->getDirection());
	if (getStudentWorld()->canMoveHere(x(), y()))
	{
		setDidIMove(true);
		return true;
	}
	else
		updateXandY(oppDir(this->getDirection()));
	return false;
}

void Ant::faceRandDir()
{
	setDirection(randDir());
}

void Ant::pickUpFood()
{
	//cout << "I called pickUpFood for an ant at " << x() << ", " << y() << endl;
	int num = 1800 - m_foodOnHand; //1800 max - 1700 on hand = only pick up 100 units
	if (num >= 400)			//1800 max - 700 on hand = capacity for 1100 more, but pick up 400 max 
		num = 400;
	//cout << "My food on hand was " << m_foodOnHand;
	m_foodOnHand += getStudentWorld()->howMuchEaten(x(), y(), num); //may or may not have been food at that spot; if not, returns 0 
	//cout << "After picking up food, my food on hand is " << m_foodOnHand; 

}

void Ant::dropFood()
{

	if (m_foodOnHand > 0)
	{
		//cout << "I am dropping all my food, " << m_foodOnHand << " at my location" << x() << ", " << y();

		if (!getStudentWorld()->isFoodHere(x(), y()))
		{
			Actor *p = new Food(getStudentWorld(), IID_FOOD, x(), y(), m_foodOnHand);
			getStudentWorld()->addActorToSimulation(p);
		}
		else
			getStudentWorld()->howMuchEaten(x(), y(), -m_foodOnHand); //instead of removing HP, you're adding HP to that food object 
	}		
}

void Ant::eatFood()
{
	int eat = 0; 
	if (m_foodOnHand >= 100)
		eat = 100;
	else
		eat = m_foodOnHand; 

	updateHP(-eat); //error? is this positive for eat?  
	m_foodOnHand -= eat; 
	//cout << "After eating, ny ant's HP is " << getHP() << endl;
	//cout << "After eating, food on Hand is now " << m_foodOnHand << endl;
}

void Ant::rotateClockwise()
{
	GraphObject::Direction dir;
	dir = getDirection(); 
	if (dir == up)
		setDirection(right); 
	if (dir == right)
		setDirection(down); 
	if (dir == left)
		setDirection(up); 

}
void Ant::rotateCounterClockwise()
{
	rotateClockwise(); 
	setDirection(oppDir(getDirection())); 
}

void Ant::emitPher()
{
	//cout << "I called emitPher for my ant at " << x() << ", " << y() << endl;
	getStudentWorld()->addAPheromone(x(), y(), m_colonyNum); 
}

bool Ant::isEnemy(int colony) const
{
	if (colony != m_colonyNum)
		return true;
	return false;
}

void Ant::getBitten(int damage)
{
	Insect::getBitten(damage); 
	wasBitten = true;
}


bool Ant::isDangerous(int colony) const
{
	return (colony!= m_colonyNum); //if differing colony numbers, then the ant there is dangerous 
}


bool Ant::conditionTriggered(string s)
{
	//is there a better way than hardcoding this?  opcode1 returns a number. 

	Direction dir = getDirection(); //used in i smell blank in front of me 
	int newX = x();
	int newY = y();
	switch (dir)
	{
	case(up) : { newY += 1; break; }
	case(down) : { newY -= 1; break; }
	case(right) : {newX += 1; break; }
	case(left) : {newX -= 1; break; }
	}

	if (s == "0") // "i_smell_danger_in_front_of_me"
		return getStudentWorld()->isDangerHere(newX, newY, m_colonyNum); 

	if (s == "1") //I smell pheromone in front of me 
	{
		return getStudentWorld()->isPherHere(newX, newY, m_colonyNum);
	}

	if (s == "2") // i_was_bit;
		return wasBitten;

	if (s == "3") //"i_am_carrying_food")
		return (m_foodOnHand > 0); 
	if (s == "4") //"i_am_hungry")
	{
		//cout << "My HP is " << getHP() << endl;
		return (getHP() <= 25);
	}
	if (s == "5") //"i_am_standing_on_my_anthill")
		return (x() == anthillX && y() == anthillY); 
	
	if (s == "6") //if (s == "i_am_standing_on_food"
		return getStudentWorld()->isFoodHere(x(), y());

	if (s == "7") //return i_am_standing_with_an_enemy;
		return getStudentWorld()->isEnemyAt(x(), y(), m_colonyNum); 

	if (s == "8") //i_was_blocked_from_moving;
	{
		//cout << "I called if I was blocked for ant at " << x() << ", " << y() << endl;
		return wasBlocked;
	}
	if (s == "9") //last rand number was 0 
		return (m_randNum == 0); 

	return false;
}