(this is a temp format for the README file)

Name: Hollen Lo and Samuel Ross
Student ID:101260373 and 100978341

Purpose of Program:

To simulate a hunt in a house where the hunters move through the rooms in the house searching for a ghost which is also randomly moving through the house

List of Files included in Program and their purposes:

- main.c: contains code related to user input and runs the entire program
- logger.c: contains code related to logging all the movements of the ghost and the hunters.
- house.c: contains code related to house related functions
- utils.c: contains code that provides utility functions that are used in the Ghost Simulation program
- defs.h: contains definition of constant variables and interfaces for functions in c files
- Makefile: contains defined compilation commands that can be used when compiling the files
- README.txt: contains this description

Compiling and Running the File:

1. Navigate to folder containing source code in terminal.
2. Use command 'make' to compile all source files and generate an executable called "ghostTest"(alternatively,  you can use the command 'gcc -Wall -Wextra -Werror -o ghostTest main.c ghost.c building.c room.c defs.h' to create file 'ghostTest' to execute).
3. Use command './ghostTest' while in the folder containing the executable to run the program.
4. If you wish to check for memory leaks, use command 'valgrind ./ghostTest' to run the program, and it will display any leaks in the memory at the end of the execution.

How to Use the Program:

when program begins, the user will be prompted to enter the names of 4 hunters. Once the names are all entered it will run a simulation to see if either the ghost wins by the hunters being in the house too long and getting scared or if the hunters win by finding all the evidence of the ghost before they get too scared or bored or noone wins beacuse the hunters get too bored and leave

Usage of Generative AI:

*No Generative AI assistance/code were used in the making of this program, the Makefile is entirely original and has not been copied from any other sources.
