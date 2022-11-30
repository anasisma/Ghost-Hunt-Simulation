#include "defs.h"

//  Function: initRoom
//     in/ou: Pointer to location of RoomType to initilize
//        in: Pointer to string
//   Purpose: Initializes fields of given room
void initRoom(RoomType* room, char* name) {

    //Checking if room was allocated correctly (since calloc is called outside of initRoom)

}

//  Function:  appendRoom
//     in/ou:  Location of RoomListType to append new room to
//        in:  Location of RoomType to append to list
//   Purpose:  Adds RoomType to tail of the given RoomListType
void appendRoom(RoomListType* list, RoomType* room) {

    //Allocating memory for new RoomNodeType
    RoomNodeType* newRoomNode = (RoomNodeType*) malloc(sizeof(RoomNodeType));

    //Checking for memory allocation error
    if (newRoomNode == NULL) {                                         
        printf("Memory allocation error: couldn't malloc new RoomNodeType!\n");
        exit(C_MEM_ERR);
    }

    //Setting attributes of RoomNodeType
    newRoomNode->room = room;
    newRoomNode->next = NULL;

    if (list->head == NULL) {  // if list is empty, both head and tail need to point to same node
        list->head = newRoomNode;
        list->tail = newRoomNode;
    } else {  // otherwise add node after the tail
        list->tail->next = newRoomNode;
        list->tail = list->tail->next;
    }
}

//  Function:  connectRooms
//     in/ou:  Location of first RoomType
//     in/ou:  Location of second RoomType
//   Purpose:  Add each room to each other's RoomListType
void connectRooms(RoomType* r1, RoomType* r2) {
    appendRoom(r1->connectedRooms, r2); 
    appendRoom(r2->connectedRooms, r1);
}