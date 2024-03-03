# Rubik

# Demonstration of mapping

This program is a derivation of the 2x2 rubik's cube simulator and solver made by aschwenn (https://github.com/aschwenn/Rubiks-2x2-Cube-Solver).

It allows you to create a representation of the moves that were made and use them to create a map.
Then, temporary states are located in a few rubik's cube solve to understand what must be reached.
The map allows you to efficiently navigate through defined moves which improve time a lot if you deal with many cubes to solve.

The currrent hold is that defining the map takes time, despite a simplified representation to avoid doing actions leading to the same outcome.
My aim is now to improve it using a new representation based on individual values.


Libraries to install:

sudo apt install g++


Command to launch:

g++ rubik.cpp utils.cpp Beginner.cpp Cube.cpp Side.cpp Row.cpp -o rubik
