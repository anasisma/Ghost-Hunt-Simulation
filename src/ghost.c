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
//         in:  Pointer to GhostType
//    Purpose:  Check if there is a hunter in the same room as given ghost
int hunterInRoom(GhostType *ghost) {
    if (ghost->room->hunterCount > 0)
        return C_TRUE;
    return C_FALSE;
}

//  Function: leaveEvidence
//        in: Pointer to ghost that will be leaving the evidence
//       out: Modified room that the ghost is in
//   Purpose: Check if current room is available, if so create evidence and leave it in room
void leaveEvidence(GhostType* ghost) {
    printf("Ghost left evidence\n");
    // Check if current room is available for modification
    if (sem_trywait(&(ghost->room->mutex)) == 0) {
        
        //Initalizing new evidence
        EvidenceType* evidence;
        initEvidence(&evidence);

        //Creating evidence based on ghost
        createEvidence(ghost, evidence);

        //Adding evidence to current room
        appendEvidence(&(ghost->room->evidenceList), evidence);

        //Unlocking current room
        sem_post(&(ghost->room->mutex));
    }
}

//   Function:  makeMove
//      in/ou:  Location of GhostType
//    Purpose:  Trys to move ghost to a random connected room, if rooms are locked, skips turn
void moveGhost(GhostType* ghost) {
    
    // Check if current room is available for modification
    if (sem_trywait(&(ghost->room->mutex)) == 0) {
        // Get a random connected room to move to
        int nextRoom = randInt(0, ghost->room->connectedRooms.roomCount);
        RoomNodeType* iterator = ghost->room->connectedRooms.head;
        for (int i = 0; i < nextRoom; i++) {
            iterator = iterator->next;
        }
        RoomType* newRoom = iterator->room;
        RoomType* currentRoom = ghost->room;

        // Check if current room is available for modification
        if (sem_trywait(&(newRoom->mutex)) == 0) {
            //Move ghost to new room
            currentRoom->ghost = NULL;
            newRoom->ghost = ghost;
            ghost->room = newRoom;

            printf("Ghost moved to %s\n", newRoom->name);

            //Unlocking new room
            sem_post(&(newRoom->mutex));
        }

        //Unlocking current room
        sem_post(&(currentRoom->mutex));
    }
}

//   Function:  startGhost
//      in/ou:  Location of GhostType
//    Purpose:  Ghost thread start function
void *startGhost(void *g) {

    GhostType* ghost = (GhostType*) g;

    //Main loop
    while (C_TRUE) {
        
        //printf("Ghost boredom: %d\n", ghost->boredom);

        //Check if ghost is in room with hunter
        if (hunterInRoom(ghost) == C_TRUE) {
            
            //Resetting boredom counter
            printf("Ghost and hunter in same room!!\n");
            ghost->boredom = BOREDOM_MAX;

            //Choose to leave evidence (randInt == 0) or do nothing
            if (randInt(0, 5) == 0) {
                leaveEvidence(ghost);
            }

        //Ghost is alone in room
        } else {

            //Decreasing boredom counter
            ghost->boredom--;

            //Checking if ghost is bored
            if (ghost->boredom <= 0) {
                //Ending this thread by breaking main loop
                break;
            }

            //Choose next action
            int rand = randInt(0, 5);

            //Leave evidence
            if (rand == 0) {
                leaveEvidence(ghost);

            //Move to an adjacent room
            } else if (rand == 1) {
                moveGhost(ghost);
            }
        }  

        //Sleep
        usleep(USLEEP_TIME);
    }
}