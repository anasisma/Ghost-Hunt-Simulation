CC = gcc
LIB = -lpthread -lm
OBJ = building evidence ghost hunter main room
OBJ2 = building.o evidence.o ghost.o hunter.o main.o room.o

all: $(OBJ)
	$(CC) -o linkedFiles $(OBJ2) $(LIB)

building: building.c
	$(CC) building.c -c building $(LIB)

evidence: evidence.c
	$(CC) evidence.c -c evidence $(LIB)

ghost: ghost.c
	$(CC) ghost.c -c ghost $(LIB)

hunter: hunter.c
	$(CC) hunter.c -c hunter $(LIB)

main: main.c
	$(CC) main.c -c main $(LIB)

room: room.c
	$(CC) room.c -c room $(LIB)

clean:
	rm -f *.o *.d $(OBJ) $(OBJ2)