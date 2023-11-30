TARGETS = house.o logger.o main.o utils.o
OBJECTS = -Wextra -Wall -lpthread
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
clean:
		rm -f ${TARGETS} ghostTest

