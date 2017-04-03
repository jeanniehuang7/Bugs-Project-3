#include "StudentWorld.h"
#include <string>
#include "Actor.h"
#include <iostream>
#include "Compiler.h"
#include <sstream>
#include <iomanip>
#include <map>
using namespace std;

StudentWorld::StudentWorld(std::string assetDir)
	: GameWorld(assetDir)
{
	tickCount = 0;
}

GameWorld* createStudentWorld(std::string assetDir)
{
	return new StudentWorld(assetDir);
}

bool operator<(Coord a, Coord b)
{
	if (a.coord_x < b.coord_x)
		return true;

	if (a.coord_x == b.coord_x) 
		if (a.coord_y < b.coord_y) 
			return true;

	return false;
}

void StudentWorld::printMyMap()
{
	//std::cout << "My map contains: " << std::endl;
	std::map < Coord, std::vector<Actor*>> ::iterator it;

	for (it = CoordToActorM.begin(); it != CoordToActorM.end(); it++)
	{
		for (int i = 0; i < it->second.size(); i++) //added in the it->First.size() >=2
		{
			std::string cur = "";
			BabyGrasshopper* pp = dynamic_cast<BabyGrasshopper*>(it->second[i]);
			if (pp != nullptr)
			{
				cur = "Grasshopper";
		//		std::cout << "For (" << it->first.coord_x << ", " << it->first.coord_y << ") there was a " << cur << std::endl;
			}
		}
	}
}

//void StudentWorld::initAnthills()
//{
//
//}

int StudentWorld::init()
{
	/***********************LOAD FIELD******************************************/
	Field f;
	std::string fieldName = getFieldFilename(); // function is part of GameWorld
	std::cout << "The field Name is " << fieldName << std::endl;
	if (f.loadField(fieldName) != Field::LoadResult::load_success) //loadResult is enum type in the class field. 
	{
		std::cout << "Failed to load\n";
		return GWSTATUS_LEVEL_ERROR; //field failed to load 
	}


	/*********************COMPILE ANTS/ANTHILLS*********************************/

	fileNames = getFilenamesOfAntPrograms(); //just changed this 1:43 pm hopefully it works T_T 

	for (int i = 0; i < fileNames.size(); i++) //assume that fileNames.size() <= max number of colonies? 
	{
		Compiler *p = new Compiler; 
		myCompilers.push_back(p);  //now has a vector of new compilers - need to delete later 
		scores.push_back(0); 
	} //myCompilers, scores, and fileNames should all be the same size now

	numInitialAnthills = fileNames.size(); 

	string error; 

	for (int i = 0; i < myCompilers.size(); i++)
	{
		if (!myCompilers[i]->compile(fileNames[i], error))
		{
			setError(fileNames[i] + " " + error);
			return GWSTATUS_LEVEL_ERROR; 
		}
		else //compiled correctly
		{
			Anthill *ah; 

			if (i == 0) {
				Coord loc = findMyLocation(f, Field::FieldItem::anthill0);
				ah = new Anthill(this, IID_ANT_HILL, loc.coord_x, loc.coord_y, myCompilers[i], 0);
			}
				
			else if (i == 1) {
				Coord loc = findMyLocation(f, Field::FieldItem::anthill1);
				ah = new Anthill(this, IID_ANT_HILL, loc.coord_x, loc.coord_y, myCompilers[i], 1);
			}
				
			else if (i == 2) {
				Coord loc = findMyLocation(f, Field::FieldItem::anthill2);
				ah = new Anthill(this, IID_ANT_HILL, loc.coord_x, loc.coord_y, myCompilers[i], 2);
			}
				
			else if (i == 3)
			{
				Coord loc = findMyLocation(f, Field::FieldItem::anthill3);
				ah = new Anthill(this, IID_ANT_HILL, loc.coord_x, loc.coord_y, myCompilers[i], 3);
			}		

			else //shouldn't be called, but just in case :O 
			{
				Coord loc = findMyLocation(f, Field::FieldItem::anthill0);
				ah = new Anthill(this, IID_ANT_HILL, loc.coord_x, loc.coord_y, myCompilers[i], 0);
			}

			addActorToSimulation(ah);
		}
	}

	for (int i = 0; i < myCompilers.size(); i++)
	{
		cout << "I have a compiler named " << fileNames[i] << " with a score of " << scores[i] << endl;
	}

	/********FILL MAP WITH OTHER ACTORS****************************************************/
	//making a map (connects a coordinate with a vector of actors)
	//only create a connection/mapping if an actor exists at that location!
	//actorsVector is the vector of actors at a coordinate! 

	int count = 0;

	for (int c = 0; c < 64; c++)
	{
		for (int r = 0; r < 64; r++)
		{
			std::vector <Actor*> actorsVector; //a vector of actors at a specific coordinate! 
			Field::FieldItem current = f.getContentsOf(c, r);

			Actor *p = nullptr;
			switch (current)
			{
				case (Field::FieldItem::rock): 
				{
					p = new Pebble(this, IID_ROCK, c, r); 
					break; 
				}
				case (Field::FieldItem::grasshopper): 
				{
					p = new BabyGrasshopper(this, IID_BABY_GRASSHOPPER, c, r, 0.25);  //could probably get rid of the size here 
					break;
				}
				case (Field::FieldItem::food):
				{
					p = new Food(this, IID_FOOD, c, r, 6000); //change to 6000! 
					break;
				}	

				case (Field::FieldItem::water):
				{
					p = new Pool(this, IID_WATER_POOL, c, r); 
					break;
				}

				case (Field::FieldItem::poison):
				{
					p = new Poison(this, IID_POISON, c, r); 
					break;
				}
			}

			if (p != nullptr)
			{
				actorsVector.push_back(p);
				count++; //can remove later, for sanity check
			}

			if (actorsVector.size() != 0)
			{
				Coord cur; 
				cur.coord_x = c;
				cur.coord_y = r; 
				CoordToActorM[cur] = actorsVector; //now mapped the location to the vector of actors at that coord
												   //cout << "I put in an actor at coordinate (" << cur[0] << ", "<< cur[1] << ")\n"; 
			}
		}
	}

	//printMyMap();
	std::cout << "I have " << count << " actors in my game\n";

	//cleanUp();  //REMOVE THIS, used this to test my destructor 

	return GWSTATUS_CONTINUE_GAME;
}

Coord StudentWorld::findMyLocation(Field &myfield, Field::FieldItem a) const
{
	for (int c = 0; c < 64; c++)
	{
		for (int r = 0; r < 64; r++)
		{
			Field::FieldItem current = myfield.getContentsOf(c, r);
			Coord loc;
			loc.coord_x = c;
			loc.coord_y = r;

			if (current == a) //for instance, anthill 0 = ant hill 0 
				return loc;
		}
	}

	Coord error; 
	error.coord_x = -1;  //check for when u returned an error? 
	error.coord_y = -1; 
	return error;
}


int StudentWorld::move()
{
	updateTickCount();

	std::string s = "Tick Count: ";
	s += std::to_string(2000 - tickCount);
	setGameStatText(s);

	//cout << endl << "********************Tick number" << tickCount << "***************************" << endl; 
	std::map<Coord, std::vector<Actor*>> ::iterator it;
	for (it = CoordToActorM.begin(); it != CoordToActorM.end(); it++) //looping through all the coordinates with existing actors
	{
		for (int i = 0; i < (it->second).size(); i++) //for each coordinate, loop through the actor vector 
		{
			(it->second)[i]->doSomething();		//ask each actor at element theActorVector[i] to do something
		}
	}

	//cout << "Before updating actor locations: \n";
	//		printMyMap();
	updateActorLocations();
	//cout << "After updating actor locations: \n";
	//printMyMap(); 
	removeDeadActors(); 
	updateDisplayText(); 
	if (isSimulationOver())
	{
		bool is_a_winner = false;
		for (int i = 0; i < scores.size(); i++)
		{
			if (scores[i] > 5)
				is_a_winner = true;
		}
		if (!is_a_winner)
			return GWSTATUS_NO_WINNER;
		else
		{
			setWinner(fileNames[findLeadingAnt()]);
			return GWSTATUS_PLAYER_WON;
		}	
	}

	else
		return GWSTATUS_CONTINUE_GAME;


}
//
//vector <string> StudentWorld::getFileN() const
//{
//	return fileNames;
//}

int StudentWorld::findLeadingAnt()
{
	int maxPos = 0;
	int maxScore = 0;
	for (int i = 0; i < scores.size(); i++)
	{
		if (scores[i] > maxScore)
		{
			maxScore = scores[i];
			maxPos = i;
		}
	}

	return maxPos; 
}

void StudentWorld::updateDisplayText()
{
	string s = formatMe(); 
	setGameStatText(s); 
}

string StudentWorld::formatMe()
{
	string ticks = "Ticks: ";
	stringstream oss; 
	oss << ticks; 
	oss << setw(5) << 2000 - tickCount << " -  "; //space hyphen space space 

	string ant0, ant1, ant2, ant3, score0, score1, score2, score3;
	int leading = findLeadingAnt(); 

	switch (numInitialAnthills)
	{
	case(4) :
	{
		if (scores[3] < 10)
			score3 = "0" + to_string(scores[3]);
		else
			score3 = to_string(scores[3]);
		if (leading == 3)
			ant3 = fileNames[3] + "*: " + score3;
		else
			ant3 = fileNames[3] + ": " + score3;
	}
	case(3) :
	{
		if (scores[2] < 10)
			score2 = "0" + to_string(scores[2]);
		else
			score2 = to_string(scores[2]);
		if (leading == 2)
			ant2 = fileNames[2] + "*: " + score2;
		else
			ant2 = fileNames[2] + ": " + score2;
	}
	case(2) :
	{
		if (scores[1] < 10)
			score1 = "0" + to_string(scores[1]);
		else
			score1 = to_string(scores[1]);
		if (leading == 1)
			ant1 = fileNames[1] + "*: " + score1;
		else
			ant1 = fileNames[1] + ": " + score1;
	}
		

	case(1) :
	{
		if (scores[0] < 10)
			score0 = "0" + to_string(scores[0]);
		else
			score0 = to_string(scores[0]);
		if (leading == 0)
			ant0 = fileNames[0] + "*: " + score0;
		else
			ant0 = fileNames[0] + ": " + score0;
	}

	}
	
	oss << ant0 << "  " << ant1 << "  " << ant2 << "  " << ant3;

	return oss.str(); 
}

string StudentWorld::to_string(int num)
{
	stringstream ss; 
	ss << num; 
	return ss.str(); 
}
void StudentWorld::updateActorLocations()
{
	std::map<Coord, std::vector<Actor*>> ::iterator it;
	it = CoordToActorM.begin(); 
	while (it != CoordToActorM.end()) //looping through all the coordinates with existing actors
	{
		for (int i = 0; i < (it->second).size(); i++) //for each coordinate, loop through the actor vector. should only run once if only 1 grasshopper/coord
		{
			Actor*pp = it->second[i]; //pp now points to the memory location where the actor is. 
			if (pp->didIMove())
			{
				std::map<Coord, std::vector<Actor*>> ::iterator it2; //create an iterator to search map for the new coord where actor should move to  

				Coord newLoc; 
				newLoc.coord_x = pp->x(); 
				newLoc.coord_y = pp->y(); 

				it2 = CoordToActorM.find(newLoc); //direct it2 to the new coordinate, then pushback the grasshopper into its actor vector 
				if (it2 != CoordToActorM.end()) //if that coordinate already has an existing coordinate/actor vector, push back grasshopper
				{
					(it2->second).push_back(pp); //it2 now invalidated. pp is still fine though, right? 
				}
				else
				{
					std::vector <Actor*> firstActor; //grasshopper moved onto a totally new coordinate - initialize a new coord and actor vector
					firstActor.push_back(pp);
					CoordToActorM[newLoc] = firstActor; 
				}

				it->second.erase(it->second.begin() + i); //erase the old actor pointer at actorvector[i] 
				graphNew(pp, newLoc.coord_x, newLoc.coord_y); 
			}	
		} //after going through all your actors at that vector using for loop, check to see if the vector is empty. update the map if needed. 

		if ((it->second).size() == 0) //if actor vector is now empty, remove that pairing with its coordinate from the map. 
		{
			it = CoordToActorM.erase(it); //might cause errors? you erased a pairing in the map because its actor vector was empty.
		}
		else
			it++; 
	} //end of the while loop 

}

void StudentWorld::removeDeadActors()
{
	map<Coord, vector<Actor*>> ::iterator it;
	it = CoordToActorM.begin();
	while (it != CoordToActorM.end())
	{
		for (int i = 0; i < (it->second).size(); i++) //for each coordinate, loop through the actor vector. should only run once if only 1 grasshopper/coord
		{
			if (it->second[i]->isDead()) //only when the obj is dead do you regenerate 
			{
				Insect* ptrToInsect = dynamic_cast <Insect*> (it->second[i]); 
				if (ptrToInsect != nullptr)
				{
					if (ptrToInsect->regeneratesAsAdult())
					{
						Actor *p = new AdultGrasshopper(this, IID_ADULT_GRASSHOPPER, it->first.coord_x, it->first.coord_y, 0.25);
						it->second.push_back(p); //size of the vector changed
						//cout << "I added a new adult grasshopper at (" << it->first.coord_x << ", " << it->first.coord_y << ") \n";
					}

					if (ptrToInsect->regeneratesAsFood()) //baby grasshopper regenerates as both food and adult 
					{
						Actor *p = getPtrToFood(it->first); //see if there's already food at that spot

						if (p != nullptr)
						{
							EnergyHolder* ptrToFood = dynamic_cast <EnergyHolder*> (p);  //sigh...is there a better way to do this
							ptrToFood->updateHP(100);
						}
						else
						{
							p = new Food(this, IID_ADULT_GRASSHOPPER, it->first.coord_x, it->first.coord_y, 100);
							it->second.push_back(p); //fix this, if there's already a food object at that spot, use that instead. 
						}

						Actor *a = getPtrToFood(it->first);
						//cout << "My new or updated food element at (" << a->x() << ", "
							//<< a->y() << ") now has HP"; 
						EnergyHolder *testing = dynamic_cast <EnergyHolder*> (a); 
						//cout << testing->getHP() << "\n";
					}
				}
				
				delete it->second[i]; //delete the actor that the actorsVector[i] points to 
				it->second.erase(it->second.begin() + i); //erase the actorsVector[i]th element
			}
		}

		if ((it->second).size() == 0) //if actor vector is now empty, remove that pairing with its coordinate from the map. 
		{			
			it = CoordToActorM.erase(it); //might cause errors? you erased a pairing in the map because its actor vector was empty.
		}
		else
			it++;		
	}
}

void StudentWorld::graphNew(GraphObject* a, int newX, int newY)
{
	a->moveTo(newX, newY);
	//a->setVisible(true); //I don't need this line? 
	//cout << "I graphed a new object at (" << newX << ", " << newY << ")" << endl;
}


bool StudentWorld::canMoveHere(int x, int y)
{
	Coord v; 
	v.coord_x = x; 
	v.coord_y = y; 
	std::map < Coord, std::vector <Actor*>> ::iterator it;

	it = CoordToActorM.find(v); //it now points to the coordinate

	if (it != CoordToActorM.end())
	{
		for (int i = 0; i < (it->second).size(); i++) //should only be one element here, by definition, so for loop should only run once? 
		{
			if ((it->second[i])->canBlockStuff())
			{
				//cerr << "You just blocked cuhh at " << x << "," << y;
				return false; //something's blocking that spot. can't move there
			}
		}
	}

	return true;
}

Actor* StudentWorld::getPtrToFood(Coord a)
{
	std::map < Coord, std::vector <Actor*>> ::iterator it;

	it = CoordToActorM.find(a); //it now points to the coordinate

	if (it != CoordToActorM.end()) //shouldn't be the case, but just for sanity check
	{
		for (int i = 0; i < (it->second).size(); i++)
		{
			if (it->second[i]->isFood()) {
				//cout << "Found food at: " << a.coord_x << "," << a.coord_y << endl;
				return it->second[i];
			}
		}
		return nullptr; //added this line
	}
	else
		return nullptr;
}

bool StudentWorld::isFoodHere(int x, int y)
{
	Coord a; 
	a.coord_x = x; 
	a.coord_y = y; 
	Actor *p = getPtrToFood(a); 
	if (p!= nullptr)
		return true;
	return false;
}

int StudentWorld::howMuchEaten(int x, int y, int num)
{
	Coord v;
	v.coord_x = x;
	v.coord_y = y;

	EnergyHolder *p = static_cast <EnergyHolder*> (getPtrToFood(v)); //fix later 

	if (p != nullptr)
	{
		//cout << "My food changed HP from " << p->getHP() << " to "; 
		p->updateHP(-num); 
		//cout << p->getHP() << "at (" << x << ", " << y << ") \n";
		if (p->getHP() <= 0)
		{
			//cout << "An insect ate food and now the food is dead \n";
			p->setDead();
			return (p->getHP() + num); //return the food's original HP 
		}
		return num; 
	}

	return 0; 
}

bool StudentWorld::canBeStunnedHere(int x, int y)
{
	Coord v;
	v.coord_x = x;
	v.coord_y = y;
	std::map < Coord, std::vector <Actor*>> ::iterator it;

	it = CoordToActorM.find(v); //it now points to the coordinate

	if (it != CoordToActorM.end())
	{
		for (int i = 0; i < (it->second).size(); i++)
		{
			if (it->second[i]->isWaterPool())
				return true;
		}
	}
	return false;
}

bool StudentWorld::canBePoisonedHere(int x, int y)
{
	Coord v;
	v.coord_x = x;
	v.coord_y = y;
	std::map < Coord, std::vector <Actor*>> ::iterator it;

	it = CoordToActorM.find(v); //it now points to the coordinate

	if (it != CoordToActorM.end())
	{
		for (int i = 0; i < (it->second).size(); i++)
		{
			if (it->second[i]->isPoison())
				return true;
		}
	}
	return false;
}

//int StudentWorld::getTick()
//{
//	return tickCount;
//}

void StudentWorld::addActorToSimulation(Actor *addMe)
{
	Coord a; 
	a.coord_x = addMe->x(); 
	a.coord_y = addMe->y(); 
	std::map < Coord, std::vector <Actor*>> ::iterator it;
	it = CoordToActorM.find(a);
	
	if (it != CoordToActorM.end()) //other actors also exist at that coordinate, so just push back to the actors vector
	{	
			it->second.push_back(addMe);
	}  
	else
	{
		vector <Actor*> firstVector;	//otherwise, make a new coordinate to contain your first Actor there 
		firstVector.push_back(addMe); 
		CoordToActorM[a] = firstVector;
	}
	//if (addMe->isFood())
	//	cout << "I  added a new food object" << endl;
	//else
	//cout << "I added a new ant or anthill to my simulation!" << endl;
}

void StudentWorld::increaseScore(int colonyNum)
{
	if (colonyNum < myCompilers.size()) //or number of anthills? 
	{
		//cout << "Birth count for colony " << colonyNum << " was updated from " << scores[colonyNum]; 
		scores[colonyNum] = scores[colonyNum] + 1; 
		//cout << "to" << scores[colonyNum] << endl;
	}
}

void StudentWorld::addAPheromone(int x, int y, int colony)
{
	Coord v;
	v.coord_x = x;
	v.coord_y = y;
	std::map < Coord, std::vector <Actor*>> ::iterator it;

	int imageID = IID_PHEROMONE_TYPE0;
	switch (colony)
	{
	case(0) : {imageID = IID_PHEROMONE_TYPE0; break; }
	case(1) : {imageID = IID_PHEROMONE_TYPE1; break; }
	case(2) : {imageID = IID_PHEROMONE_TYPE2; break; }
	case(3) : {imageID = IID_PHEROMONE_TYPE3; break; }
	}


	it = CoordToActorM.find(v); //it now points to the coordinate

	if (it != CoordToActorM.end()) //if there's an existing actor vector at that spot 
	{
		for (int i = 0; i < (it->second).size(); i++)
		{
			if (it->second[i]->isPher())
			{
				if (updateExistingPher(it->second[i], colony)) //i will test if they're of the same colony within this function
					return;
			}
		}
		//no pheromone was found there, but the coordinate exists, so push in a new pheroomone
		Actor *p = new Pheromone(this, imageID, x, y, colony);
		it->second.push_back(p); 
		//cout << "I'm adding a new pheromone to a location " << x << ", " << y << "where another actor(s) is present\n"; 
	}

	else //totally new coordinate, need to push that into my map
	{
		vector <Actor*> firstVector;	
		Actor *p = new Pheromone(this, imageID, x, y, colony); 
		firstVector.push_back(p); 

		CoordToActorM[v] = firstVector;
		//cout << "I added a new pheromone to a totally new location" << x << ", " << y << endl;
	}
}

bool StudentWorld::updateExistingPher(Actor *p, int colony)
{
	Pheromone *ptr = static_cast <Pheromone*> (p); 
	if (ptr->colony() != colony)
		return false;
	//else, do other stuff
	//cout << "A pheromone of the same colony exists at that location and its HP changed from " << ptr->getHP() << "to "; 
	ptr->updateHP(256);  //if previous was 600, current is now 856, which is over the max of 768
	int current = ptr->getHP(); 
	if (current > 768)
	{
		ptr->updateHP(768 - current);  // current + 768 - current
	}
	
	//cout << ptr->getHP(); 
	return true;
}

bool StudentWorld::isPherHere(int x, int y, int colony)
{
	Coord v;
	v.coord_x = x;
	v.coord_y = y;
	std::map < Coord, std::vector <Actor*>> ::iterator it;

	it = CoordToActorM.find(v); //it now points to the coordinate

	if (it != CoordToActorM.end())
	{
		for (int i = 0; i < (it->second).size(); i++)
		{
			if (it->second[i]->isPher())
			{
				Pheromone *p = static_cast <Pheromone*> (it->second[i]); 
				if (p->colony() == colony)
					return true;
			}
		}
	}
	return false;
}

bool StudentWorld::isDangerHere(int x, int y, int colony)
{
	Coord v;
	v.coord_x = x;
	v.coord_y = y;
	std::map < Coord, std::vector <Actor*>> ::iterator it;

	it = CoordToActorM.find(v); //it now points to the coordinate

	if (it != CoordToActorM.end())
	{
		for (int i = 0; i < (it->second).size(); i++)
		{
			if (it->second[i]->isDangerous(colony))
				return true;
		}
	}
	return false;
}

bool StudentWorld::isEnemyAt(int x, int y, int colony)
{
	Coord v;
	v.coord_x = x;
	v.coord_y = y;
	std::map < Coord, std::vector <Actor*>> ::iterator it;

	it = CoordToActorM.find(v); //it now points to the coordinate

	if (it != CoordToActorM.end())
	{
		for (int i = 0; i < (it->second).size(); i++)
		{
			if (it->second[i]->isEnemy(colony))
			{
				return true;
			}
		}
	}
	return false;
}
bool StudentWorld::biteEnemyAt(Actor*me, int colony, int biteDamage)
{
	Coord v;
	v.coord_x = me->x();
	v.coord_y = me->y(); 
	std::map < Coord, std::vector <Actor*>> ::iterator it;


	Actor *temp = me;
	AdultGrasshopper *gptr = dynamic_cast <AdultGrasshopper*> (temp); 

	it = CoordToActorM.find(v); //it now points to the coordinate

	if (it != CoordToActorM.end())
	{
		for (int i = 0; i < (it->second).size(); i++)
		{
			if (it->second[i] != me && gptr == nullptr) // an ant is doing the biting 
			{
				if (it->second[i]->isEnemy(colony))
				{
					it->second[i]->getBitten(biteDamage);
					return true;
				}
					
			}
			else if (it->second[i] != me && gptr!= nullptr) //a grasshopper is doing the biting 
			{
				if (it->second[i]->isInsect())
				{
					it->second[i]->getBitten(biteDamage); 
					return true;
				}
			}
			 
		}
	}
	return false;
}


void StudentWorld::cleanUp()
{
	int count = 0;
	std::map<Coord, std::vector<Actor*>> ::iterator it;
	it = CoordToActorM.begin();
	while (it != CoordToActorM.end()) //looping through all the coordinates with existing actors
	{
		for (int i = 0; i < (it->second).size(); i++)
		{
			std::cout << "I deleted the Actor at (" << it->first.coord_x << ", " << it->first.coord_y << ") " << std::endl;
			delete it->second[i];
			count++;
		}
		it = CoordToActorM.erase(it); //should point at the next coordinate
	}

	for (int i = myCompilers.size() - 1; i >= 0; i--)
	{
		//cout << "I'm about to delete compiler at pos " << i << " which has a score of " << scores[i];
		delete myCompilers[i];
		myCompilers.pop_back();
	}

	//std::cout << "I deleted " << count << " items \n";
	//int x;  //	REMOVE THIS 
	//cin >> x;
}


StudentWorld::~StudentWorld()
{
	cleanUp();
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
