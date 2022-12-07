#include "defs.h"

// Function:  initBuilding
//    in/ou:  Location of BuildingType to initlize
//  Purpose:  Initializes fields of the given building
void initBuilding(BuildingType* building) {
    //Initlizing parameters
    building->ghost = NULL;
    initRoomList(&(building->rooms));
    EvidenceListType* list = &building->evidenceList;
    initEvidenceList(&list);

    // Initlizing mutex
    if (sem_init(&(building->mutex), 0, 1) < 0) {
        printf("Semaphore error: couldn't initlize building mutex!\n");
        exit(C_SEM_ERR);
    }
}

/*
    Note: While this function is provided, you will likely want
    to modify it to suit your solution. This has been provided
    to give you a starting point to lay out the room connectivity,
    but it is entirely possible that you will want to use different
    data structures to represent the rooms and their connectivity,
    or different function names.

    Not following this function's structure exactly will not result
    in a deduction of points.

    The map that this is recreating is visible in the assignment
    specification, and also available by the original creator here:
    https://steamcommunity.com/sharedfiles/filedetails/?id=2251267947

*/

// Function:  populateRooms
//    in/ou:  Location of BuildingType to populate with rooms from phasmaphobia map
//  Purpose:  Creates and connects rooms to replicate phasmaphobia
void populateRooms(BuildingType* building) {
    //Allocating and rooms and initlizing them
    RoomType* van = (RoomType*)calloc(1, sizeof(RoomType));
    initRoom(van, "Van");
    RoomType* hallway = (RoomType*)calloc(1, sizeof(RoomType));
    initRoom(hallway, "Hallway");
    RoomType* master_bedroom = (RoomType*)calloc(1, sizeof(RoomType));
    initRoom(master_bedroom, "Master Bedroom");
    RoomType* boys_bedroom = (RoomType*)calloc(1, sizeof(RoomType));
    initRoom(boys_bedroom, "Boy's Bedroom");
    RoomType* bathroom = (RoomType*)calloc(1, sizeof(RoomType));
    initRoom(bathroom, "Bathroom");
    RoomType* basement = (RoomType*)calloc(1, sizeof(RoomType));
    initRoom(basement, "Basement");
    RoomType* basement_hallway = (RoomType*)calloc(1, sizeof(RoomType));
    initRoom(basement_hallway, "Basement Hallway");
    RoomType* right_storage_room = (RoomType*)calloc(1, sizeof(RoomType));
    initRoom(right_storage_room, "Right Storage Room");
    RoomType* left_storage_room = (RoomType*)calloc(1, sizeof(RoomType));
    initRoom(left_storage_room, "Left Storage Room");
    RoomType* kitchen = (RoomType*)calloc(1, sizeof(RoomType));
    initRoom(kitchen, "Kitchen");
    RoomType* living_room = (RoomType*)calloc(1, sizeof(RoomType));
    initRoom(living_room, "Living Room");
    RoomType* garage = (RoomType*)calloc(1, sizeof(RoomType));
    initRoom(garage, "Garage");
    RoomType* utility_room = (RoomType*)calloc(1, sizeof(RoomType));
    initRoom(utility_room, "Utility Room");

    //Allocating roomNodes and to hold rooms
    RoomNodeType* van_node = (RoomNodeType*)calloc(1, sizeof(RoomNodeType));
    van_node->room = van;
    RoomNodeType* hallway_node = (RoomNodeType*)calloc(1, sizeof(RoomNodeType));
    hallway_node->room = hallway;
    RoomNodeType* master_bedroom_node = (RoomNodeType*)calloc(1, sizeof(RoomNodeType));
    master_bedroom_node->room = master_bedroom;
    RoomNodeType* boys_bedroom_node = (RoomNodeType*)calloc(1, sizeof(RoomNodeType));
    boys_bedroom_node->room = boys_bedroom;
    RoomNodeType* bathroom_node = (RoomNodeType*)calloc(1, sizeof(RoomNodeType));
    bathroom_node->room = bathroom;
    RoomNodeType* basement_node = (RoomNodeType*)calloc(1, sizeof(RoomNodeType));
    basement_node->room = basement;
    RoomNodeType* basement_hallway_node = (RoomNodeType*)calloc(1, sizeof(RoomNodeType));
    basement_hallway_node->room = basement_hallway;
    RoomNodeType* right_storage_room_node = (RoomNodeType*)calloc(1, sizeof(RoomNodeType));
    right_storage_room_node->room = right_storage_room;
    RoomNodeType* left_storage_room_node = (RoomNodeType*)calloc(1, sizeof(RoomNodeType));
    left_storage_room_node->room = left_storage_room;
    RoomNodeType* kitchen_node = (RoomNodeType*)calloc(1, sizeof(RoomNodeType));
    kitchen_node->room = kitchen;
    RoomNodeType* living_room_node = (RoomNodeType*)calloc(1, sizeof(RoomNodeType));
    living_room_node->room = living_room;
    RoomNodeType* garage_node = (RoomNodeType*)calloc(1, sizeof(RoomNodeType));
    garage_node->room = garage;
    RoomNodeType* utility_room_node = (RoomNodeType*)calloc(1, sizeof(RoomNodeType));
    utility_room_node->room = utility_room;

    //Initlizing building's rooms' linkedList
    initRoomList(&building->rooms);

    //Appending all roomNodes to building's rooms' linkedList
    appendRoom(&(building->rooms), van_node);
    appendRoom(&(building->rooms), hallway_node);
    appendRoom(&(building->rooms), master_bedroom_node);
    appendRoom(&(building->rooms), boys_bedroom_node);
    appendRoom(&(building->rooms), bathroom_node);
    appendRoom(&(building->rooms), basement_node);
    appendRoom(&(building->rooms), basement_hallway_node);
    appendRoom(&(building->rooms), right_storage_room_node);
    appendRoom(&(building->rooms), left_storage_room_node);
    appendRoom(&(building->rooms), kitchen_node);
    appendRoom(&(building->rooms), living_room_node);
    appendRoom(&(building->rooms), garage_node);
    appendRoom(&(building->rooms), utility_room_node);

    //Connecting rooms together according to phasmaphobia map
    connectRooms(van, hallway);
    connectRooms(hallway, master_bedroom);
    connectRooms(hallway, boys_bedroom);
    connectRooms(hallway, bathroom);
    connectRooms(hallway, kitchen);
    connectRooms(hallway, basement);
    connectRooms(basement, basement_hallway);
    connectRooms(basement_hallway, right_storage_room);
    connectRooms(basement_hallway, left_storage_room);
    connectRooms(kitchen, living_room);
    connectRooms(kitchen, garage);
    connectRooms(garage, utility_room);
}

// Function:  cleanupBuilding
//    in/ou:  Location of BuildingType to cleanup
//  Purpose:  Frees all memory associated with a building
void cleanupBuilding(BuildingType* building) {

    //Freeing RoomTypes
    cleanupRoomListData(&building->rooms);

    //Freeing RoomNodeTypes
    cleanupRoomListNodes(&building->rooms);

    //Freeing ghost
    free(building->ghost);

    //Freeing EvidenceTypes
    cleanupEvidenceListData(&building->evidenceList);

    //Freeing EvidenceNodeTypes
    cleanupEvidenceListNodes(&building->evidenceList);

    //Freeing HunterTypes
    for (int i = 0; i < MAX_HUNTERS; i++) {
        cleanupHunter(building->hunters[i]);
    }
}