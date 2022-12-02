#include "defs.h"

//   Function:  initGhost
//         in:  Location of GhostType to be modified
//         in:  Attributes of GhostType to be added
//        out:  Modified GhostType
//    Purpose:  Initializes all fields of the given GhostType parameter to parameters
void initGhost(int bored, GhostClassType gt, GhostType **ghost) {
    GhostType *newGhost = (GhostType *)malloc(sizeof(GhostType));  // allocate mem for new ghost
    if (newGhost == NULL) {                                        // if memory failed to allocate, shut the program down
        printf("Memory allocation error\n");
        exit(C_MEM_ERR);
    }
    // initialize fields to the given parameters
    newGhost->ghostClass = gt;
    newGhost->room = NULL;
    newGhost->boredom = bored;
    *ghost = newGhost;  // return this new structure using the ghost parameter
}

//   Function:  hunterInRoom
//         in:  Pointer to ghost to check if hunters are in same room
//    Purpose:  Check if ghost's boredom needs to be modified
int hunterInRoom(GhostType *ghost) {
    int status;
    if (ghost->room->hunterCount > 0)
        return 1;
    return 0;
}

// Function: makeMove
void makeMove(GhostType* ghost) {
    
    // If the hunter's room is available for modification, lock it
    if (sem_trywait(&(current_room->mutex)) == 0) {
        // If the new room is *not* available for modification, unlock the hunter's room and return nothing
        if (sem_trywait(&(next_room->mutex)) != 0) {
            // We must remember to unlock the locked current room
            sem_post(&(current_room->mutex));
            return;
        }
    }

    sem_trywait(&(ghost->room->mutex)) {
        int nextRoom = randInt(0, ghost->room->connectedRooms.roomCount);
        RoomNodeType iterator = ghost->room->connectedRooms.head;
        for (int i = 0; i < nextRoom; i++) {
            iterator = iterator->next;
        }
        iterator->ghost = ghost;
        ghost->room = iterator;
    }
}

//   Function:  startGhost
//      in/ou:  Location of building ghost is in
//    Purpose:  Ghost thread start function
void *startGhost(void *b) {

    BuildingType* building = (Building*) b;
    GhostType* ghost = building->ghost;

    //Main loop
    while (C_TRUE) {
        
        //Check if ghost is in room with hunter
        if (hunterInRoom(ghost) == 1) {
            
            //Resetting boredom counter
            ghost->boredom = BOREDOM_MAX;

            //Choose to leave evidence (randInt == 0) or do nothing
            if (randInt(0, 5) == 0) {
                leaveEvidence();
            }

            //Sleep
            usleep(USLEEP_TIME);


        //Ghost is alone in room
        } else {

            //Decreasing boredom counter
            ghost->boredom -= 1;

            //Checking if ghost is bored
            if (ghost->boredom <= 0) {
                break;
            }

            //Choose next action
            int rand = randInt(0, 5);

            //Leave evidence
            if (rand == 0) {
                leaveEvidence(ghost);

            //Move to an adjacent room
            } else if (rand == 1) {
                makeMove(ghost);
            }
                
            //Sleep
            usleep(USLEEP_TIME);

        }




        
        
        
    }
}