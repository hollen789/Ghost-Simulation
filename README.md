(this is a temp format for the README file)

Name: Hollen Lo
Student ID:101260373

Purpose of Program:

To simulate a program that documents the ID, Type, Room, and likelihood of a ghost inside a building, and displays the information in certain formats depending predefined options that the User can choose. 

List of Files included in Program and their purposes:

- main.c: contains code related to user input
- ghost.c: contains code related to ghost related functions
- building.c: contains code related to building related functions
- room.c: contains code related to room related functions
- defs.h: contains definition of constant variables and interfaces for functions in c files
- Makefile: contains defined compilation commands that can be used when compiling the files
- README.txt: contains this description

Compiling and Running the File:

1. Navigate to folder containing source code in terminal.
2. Use command 'make' to compile all source files and generate an executable called "ghostTest"(alternatively,  you can use the command 'gcc -Wall -Wextra -Werror -o ghostTest main.c ghost.c building.c room.c defs.h' to create file 'ghostTest' to execute).
3. Use command './ghostTest' while in the folder containing the executable to run the program.
4. If you wish to check for memory leaks, use command 'valgrind ./ghostTest' to run the program, and it will display any leaks in the memory at the end of the execution.

How to Use the Program:

when program begins, the user will be prompted a menu which displays these 5 options:
(1) Print rooms
(2) Print ghosts
(3) Print ghosts by likelihood
(4) Print most/least likely ghosts
(0) Exit
the user will then asked to input an option.
- when '1' is inputted, the user will be prompted a formatted list of rooms in the building, according to the order of insertion(for both the rooms and ghosts of the building).
- when '2' is inputted, the user will be prompted a list of ghosts in the building,in ascending order by ghost ID.
- when '3' is inputted, the user will be prompted a list of ghosts in the building, in descending order by ghost likelihood.
- when '4' is inputted, the user will be prompted the most likely and least likely ghost to appear in the building.
- when '0' is inputted, the program will quit.
all of the data within this program is predefined and the user cannot alter it in anyway.

Usage of Generative AI:

*No Generative AI assistance/code were used in the making of this program, the Makefile is entirely original and has not been copied from any other sources.
