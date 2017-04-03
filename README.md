# Bugs-Project-3
A game simulation of insects that battle each other and forage for survival. 

SUMMARY
In this graphical simulation, up to four colonies of ants compete for dominance in a virtual playing field. They may forage for food, avoid poison and water pools, bite grasshoppers and ants from enemy colonies, and ultimately return food to their queen so she can reproduce. Each ant colony is controlled by its unique program in a .bug file, and the colony that produces the most ants by the end of the simulation is the winner.  

Ultimately, I built a playing field for which these ants can compete. People who have just started programming can implement simple algorithms for how ants can move and respond to stimuli. My program will take their code, compile it, and run them against other ant programs in my field, to see whose program is the most efficient. Thus, I've built a learning framework for beginning programmers, and a possible high-school level hackathon. 

Because of the many actors in the simulation, this project provided good polymorphism and inheritance practice. I implemented pebbles, baby grasshoppers, adult grasshoppers, pheromones, food, anthills, water pools, poison, and ants, with their (if applicable) movement and HP changes in an appropriate data structure. 

VERSION HISTORY
20170402 - Initial commit

THE FILES
field.txt - a possible text playing field. can be modified 
StudentWorld.h, StudentWorld.cpp  - manages initialization, execution, and cleanup of simulation. manages the data structure for all actors
Actor.h, Actor.cpp - contains instructions for all actors in the simulation. 
USCAnt.bug, dummy.bug, reallydumb.bug - sample ant programs that have limited functionality
freeglut - open source alternative to GLUT 

Developed by Carey Nachenburg and David Smallberg
Assets, irrKlang, SpriteManager.h files - sound and graphics
Compiler.h - compiles ant programs (.bug) 
Field.h - compiles field.txt 
GameController, GameWorld, GameConstants files - framework for game 
