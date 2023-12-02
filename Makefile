TARGETS = house.o logger.o main.o utils.o
OBJECTS = -g -Wextra -Wall -lpthread
all:	house logger main utils
		gcc ${OBJECTS} -o ghostTest ${TARGETS} 

house: 	house.c defs.h
		gcc ${OBJECTS} -c house.c
logger: logger.c defs.h
		gcc ${OBJECTS} -c logger.c  
main: 	main.c defs.h
		gcc ${OBJECTS} -c main.c  
utils:	utils.c defs.h
		gcc ${OBJECTS} -c utils.c  
ghost: ghost.c defs.h
		gcc ${OBJECTS} -c ghost.c
hunter: hunter.c defs.h
		gcc ${OBJECTS} -c hunter.c
clean:
		rm -f ${TARGETS} ghostTest

