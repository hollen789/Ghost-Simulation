TARGETS=send handle fork
all:	${TARGETS} 

send: 	p1-send.c
		gcc -o send p1-send.c
handle: 	p1-handle.c 
		gcc -o handle p1-handle.c  
fork: 
		gcc -o fork p2-fork.c  
clean:
		rm -f p1-send.o p1-handle.o p2-fork.o

