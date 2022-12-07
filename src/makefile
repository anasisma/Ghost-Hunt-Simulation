LIB = -lpthread -lm
OBJ = building.o evidence.o ghost.o hunter.o main.o room.o

all: main room hunter ghost evidence building
	gcc -o simulation $(OBJ) $(LIB)

main: main.c defs.h
	gcc -c main.c

room: room.c defs.h
	gcc -c room.c

hunter: hunter.c defs.h
	gcc -c hunter.c

ghost: ghost.c defs.h
	gcc -c ghost.c

evidence: evidence.c defs.h
	gcc -c evidence.c

building: building.c defs.h
	gcc -c building.c

clean:
	rm -f $(OBJ) simulation