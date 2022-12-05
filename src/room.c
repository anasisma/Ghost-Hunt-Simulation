#include "defs.h"

//  Function: initRoom
//     in/ou: Pointer to location of RoomType to initilize
//        in: Pointer to string
//   Purpose: Initializes fields of given room
void initRoom(RoomType *room, char *name) {
    // Checking if room was allocated correctly (since calloc is called outside of initRoom)
    if (room == NULL) {
        printf("Memory allocation error: couldn't calloc new RoomType!\n");
        exit(C_MEM_ERR);
    }

    // Initlizing mutex
    if (sem_init(&room->mutex, 0, 1) < 0) {
        printf("Semaphore error: couldn't initlize mutex!\n");
        exit(C_SEM_ERR);
    }

    // Initilizing attributes
    strcpy(room->name, name);
    room->ghost = NULL;
}

//  Function:  appendEvidence
//     in/ou:  Location of EvidenceListType to append new room to
//        in:  Location of EvidenceType to append to list
//   Purpose:  Adds EvidenceType to tail of the given EvidenceListType
void appendEvidence(EvidenceListType* list, EvidenceType* evidence) {

    // Allocating memory for new EvidenceNodeType
    EvidenceNodeType* newEvidenceNode = (EvidenceNodeType *)malloc(sizeof(EvidenceNodeType));

    // Checking for memory allocation error
    if (newEvidenceNode == NULL) {
        printf("Memory allocation error: couldn't malloc new EvidenceNodeType!\n");
        exit(C_MEM_ERR);
    }

    // Setting attributes of newEvidenceNode
    newEvidenceNode->evidence = evidence;
    newEvidenceNode->next = NULL;

    // If list is empty, both head and tail need to point to new node
    if (list->head == NULL) {
        list->head = newEvidenceNode;
        list->tail = newEvidenceNode;

        // Otherwise add node after the tail
    } else {
        list->tail->next = newEvidenceNode;
        list->tail = newEvidenceNode;
    }
}

//  Function:  appendRoom
//     in/ou:  Location of RoomListType to append new room to
//        in:  Location of RoomType to append to list
//   Purpose:  Adds RoomType to tail of the given RoomListType
void appendRoom(RoomListType *list, RoomType *room) {
    // Allocating memory for new RoomNodeType
    RoomNodeType *newRoomNode = (RoomNodeType *)malloc(sizeof(RoomNodeType));

    // Checking for memory allocation error
    if (newRoomNode == NULL) {
        printf("Memory allocation error: couldn't malloc new RoomNodeType!\n");
        exit(C_MEM_ERR);
    }

    // Setting attributes of RoomNodeType
    newRoomNode->room = room;
    newRoomNode->next = NULL;

    // If list is empty, both head and tail need to point to new node
    if (list->head == NULL) {
        list->head = newRoomNode;
        list->tail = newRoomNode;

        // Otherwise add node after the tail
    } else {
        list->tail->next = newRoomNode;
        list->tail = newRoomNode;
    }
    list->roomCount++;
}

//  Function:  connectRooms
//     in/ou:  Location of first RoomType
//     in/ou:  Location of second RoomType
//   Purpose:  Add each room to each other's RoomListType
void connectRooms(RoomType *r1, RoomType *r2) {
    appendRoom(r1->connectedRooms, r2);
    appendRoom(r2->connectedRooms, r1);
}

//  Function:  addHunter
//     in/ou:  Location of RoomType to add hunter to
//     in/ou:  Location of HunterType to add to room
//   Purpose:  Adds HunterType to given room's hunters array and sets hunter's room
void addHunter(RoomType *room, HunterType *hunter) {
    // Check if room's hunters array is full
    if (room->hunterCount >= MAX_HUNTERS) {
        printf("Error adding hunter to room: room's hunters array is full!\n");
        exit(C_ARR_FULL);
    }

    // Adding new hunter to end of array
    room->hunters[room->hunterCount] = hunter;

    // Setting hunter's room attribute
    hunter->room = room;
}

//  Function:  removeHunter
//     in/ou:  Location of RoomType to remove hunter from
//     in/ou:  Location of HunterType to remove from room
//   Purpose:  Removes HunterType from given room's hunters list and clears hunter's room
void removeHunter(RoomType *room, HunterType *hunter) {
    int size = room->hunterCount;

    if (size <= 0) {
        printf("Error remove hunter from room: room's hunters array is empty!\n");
        exit(C_ARR_ERR);
    }
    for (int i = 0; i < size; i++) {
        if (room->hunters[i] == hunter) {
            if (size < MAX_HUNTERS) {
                for (int j = i; j < size; j++) {
                    room->hunters[j] = room->hunters[j + 1];
                }
            }
            room->hunters[size] = NULL;
            size--;
            return;
        }
    }
    printf("Error removing hunter from room: hunter not in room's hunters array!\n");
    exit(C_ARR_ERR);
}

//  Function:  cleanupRoom
//     in/ou:  Location of RoomType to cleanup
//   Purpose:  Free memory allocated for a particular room (doesn't free it's data)
void cleanupRoom(RoomType *room) {
    free(room);
}