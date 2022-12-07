#include "defs.h"

//  Function: initRoom
//     in/ou: Pointer to location of RoomType to initilize
//        in: Pointer to string
//   Purpose: Initializes the given room
void initRoom(RoomType *room, char *name) {
    // Checking if room was allocated correctly (since calloc is called outside of initRoom)
    if (room == NULL) {
        printf("Memory allocation error: couldn't calloc new RoomType!\n");
        exit(C_MEM_ERR);
    }

    // Initlizing mutex
    if (sem_init(&(room->mutex), 0, 1) < 0) {
        printf("Semaphore error: couldn't initlize room mutex!\n");
        exit(C_SEM_ERR);
    }

    // Initilizing attributes
    strcpy(room->name, name);
    room->hunterCount = 0;
    room->ghost = NULL;
}

//   Function:  initRoomList
//      in/ou:  Location of RoomListType to initialize
//    Purpose:  Initializes the given RoomListType
void initRoomList(RoomListType *list) {
    list->head = NULL;
    list->tail = NULL;
    list->roomCount = 0;
}

//  Function:  appendRoom
//     in/ou:  Location of RoomListType to append new room to
//        in:  Location of RoomNodeType to append to list
//   Purpose:  Adds RoomNodeType to tail of the given RoomListType
void appendRoom(RoomListType *list, RoomNodeType *roomNode) {
    roomNode->next = NULL;

    // If list is empty, both head and tail need to point to new node
    if (list->head == NULL) {
        list->head = roomNode;
        list->tail = roomNode;

    // Otherwise add node after the tail
    } else {
        list->tail->next = roomNode;
        list->tail = roomNode;
    }

    //Update room count
    list->roomCount++;
}

//  Function:  connectRooms
//     in/ou:  Location of first RoomType
//     in/ou:  Location of second RoomType
//   Purpose:  Add each room to each other's RoomListType
void connectRooms(RoomType *r1, RoomType *r2) {
    //Allocating new RoomNodeTypes
    RoomNodeType *n1 = (RoomNodeType *)malloc(sizeof(RoomNodeType));
    RoomNodeType *n2 = (RoomNodeType *)malloc(sizeof(RoomNodeType));

    //Checking if nodes were allocated correctly
    if (n1 == NULL || n2 == NULL) {
        printf("Memory allocation error: couldn't malloc new RoomNodeType!\n");
        exit(C_MEM_ERR);
    }

    //Settings nodes' attributes
    n1->room = r1;
    n1->next = NULL;
    n2->room = r2;
    n2->next = NULL;

    //Adding nodes to each room's list
    appendRoom(&(r1->connectedRooms), n2);
    appendRoom(&(r2->connectedRooms), n1);
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

    // Update hunter count
    room->hunterCount++;
}

//  Function:  removeHunter
//     in/ou:  Location of RoomType to remove hunter from
//     in/ou:  Location of HunterType to remove from room
//   Purpose:  Removes HunterType from given room's hunters list and clears hunter's room
void removeHunter(RoomType *room, HunterType *hunter) {
    int size = room->hunterCount;

    // check if the room's hunter array is empty
    if (size <= 0) {
        printf("Error remove hunter from room: room's hunters array is empty!\n");
        exit(C_ARR_ERR);
    }

    // loop through hunter array to find hunter we want to remove
    for (int i = 0; i < size; i++) {
        if (room->hunters[i] == hunter) {
            // loop to iterate from the hunter's index onwards, to shift the others back
            for (int j = i; j < size - 1; j++) {
                room->hunters[j] = room->hunters[j + 1];
            }

            // set whatever was at the end of the array to null and decrement hunterCount
            room->hunters[size] = NULL;
            room->hunterCount--;

            // make hunter we removed's room parameter point to null
            hunter->room = NULL;
            return;
        }
    }

    // if this point is reached, the hunter wasn't found in the array
    printf("Error removing hunter from room: hunter not in room's hunters array!\n");
    exit(C_ARR_ERR);
}

//  Function:  cleanupRoom
//     in/ou:  Location of RoomType to cleanup
//   Purpose:  Frees all the memory associated with the given room
void cleanupRoom(RoomType *room) {
    //Freeing evidence list's data
    cleanupEvidenceListData(&room->evidenceList);

    //Freeing evidence list's nodes
    cleanupEvidenceListNodes(&room->evidenceList);

    //Freeing room list's nodes
    cleanupRoomListNodes(&room->connectedRooms);

    //Freeing room's pointer
    free(room);
}

//  Function:  cleanupRoomListNodes
//     in/ou:  Location of RoomListType to cleanup
//   Purpose:  Frees all the RoomNodeTypes contained in given linkedList
void cleanupRoomListNodes(RoomListType *list) {
    // Temporary pointers
    RoomNodeType *currNode = list->head;
    RoomNodeType *nextNode;

    // Looping through all nodes in list
    while (currNode != NULL) {
        nextNode = currNode->next;

        // Freeing node
        free(currNode);

        currNode = nextNode;
    }
}

//  Function:  cleanupRoomListData
//     in/ou:  Location of RoomListType to cleanup
//   Purpose:  Frees all the RoomTypes contained in given linkedList
void cleanupRoomListData(RoomListType *list) {
    // Temporary pointers
    RoomNodeType *currNode = list->head;
    RoomNodeType *nextNode;

    // Looping through all nodes in list
    while (currNode != NULL) {
        nextNode = currNode->next;

        // Freeing room
        cleanupRoom(currNode->room);

        currNode = nextNode;
    }
}