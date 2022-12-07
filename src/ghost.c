#include "defs.h"

//   Function:  initGhost
//      in/ou:  Pointer to location of GhostType to initialize
//         in:  GhostClassType to set ghost's class to
//    Purpose:  Allocates and initializes the given GhostType
void initGhost(GhostType** ghost, GhostClassType gt) {
    //Allocating memory for ghost
    GhostType* newGhost = (GhostType*)malloc(sizeof(GhostType));

    //Checking if memory was allocated correctly
    if (newGhost == NULL) {
        printf("Memory allocation error: couldn't malloc new GhostType!\n");
        exit(C_MEM_ERR);
    }

    //Setting attributes of new GhostType
    newGhost->ghostClass = gt;
    newGhost->room = NULL;
    newGhost->boredom = BOREDOM_MAX;

    //return newGhost structure using the pointer parameter
    *ghost = newGhost;
}

//   Function:  hunterInRoom
//         in:  Location of GhostType
//    Purpose:  Checks if there is a hunter in the same room as given ghost
//     Return:  Returns C_TRUE if there is a hunter in the same room, returns C_FALSE otherwise
int hunterInRoom(GhostType* ghost) {
    if (ghost->room->hunterCount > 0)
        return C_TRUE;
    return C_FALSE;
}

//  Function: leaveEvidence
//        in: Location of GhostType
//   Purpose: Checks if current room is available, if so create evidence and leave it in room
void leaveEvidence(GhostType* ghost) {
    // Check if current room is available for modification
    if (sem_trywait(&(ghost->room->mutex)) == 0) {
        // Initalizing new evidence
        EvidenceType* evidence;
        initEvidence(&evidence);

        // Creating evidence based on ghost
        createEvidence(ghost->ghostClass, evidence);

        // Adding evidence to current room
        appendEvidence(&(ghost->room->evidenceList), evidence);

        //Check if evidence ghost left is standard or ghostly
        if (isGhostlyVal(evidence)) {
            printf("| %20s | %30s | %20s |\n", "Ghost", "left ghostly evidence", getTypeString(evidence->evidenceClass));
        } else {
            printf("| %20s | %30s | %20s |\n", "Ghost", "left standard evidence", getTypeString(evidence->evidenceClass));
        }

        // Unlocking current room
        sem_post(&(ghost->room->mutex));
    }
}

//   Function:  moveGhost
//      in/ou:  Location of GhostType
//    Purpose:  Tries to move ghost to a random adjacent room if rooms' mutexs are available
void moveGhost(GhostType* ghost) {
    // Check if current room is available for modification
    if (sem_trywait(&(ghost->room->mutex)) == 0) {
        // Get a random connected room to move to
        int nextRoom = randInt(0, ghost->room->connectedRooms.roomCount);

        //Looping to selected room
        RoomNodeType* iterator = ghost->room->connectedRooms.head;
        for (int i = 0; i < nextRoom; i++) {
            iterator = iterator->next;
        }

        //Creating temporary pointers
        RoomType* newRoom = iterator->room;
        RoomType* currentRoom = ghost->room;

        // Check if new room is available for modification
        if (sem_trywait(&(newRoom->mutex)) == 0) {
            // Move ghost to new room
            currentRoom->ghost = NULL;
            newRoom->ghost = ghost;
            ghost->room = newRoom;

            printf("| %20s | %30s | %20s |\n", "Ghost", "changed location to", newRoom->name);

            // Unlocking new room
            sem_post(&(newRoom->mutex));
        }

        // Unlocking current room
        sem_post(&(currentRoom->mutex));
    }
}

//   Function:  startGhost
//      in/ou:  Location of GhostType
//    Purpose:  Ghost thread start function
void* startGhost(void* g) {
    GhostType* ghost = (GhostType*)g;

    // Main loop
    while (C_TRUE) {

        // Check if ghost is in room with hunter
        if (hunterInRoom(ghost) == C_TRUE) {
            // Resetting boredom counter
            ghost->boredom = BOREDOM_MAX;

            // Choose to leave evidence: 20% to leave evidence, 80% to do nothing
            if (randInt(0, 5) == 0) {
                leaveEvidence(ghost);
            }

        // Ghost is alone in room
        } else {
            // Decreasing boredom counter
            ghost->boredom -= BOREDOM_RATE;
            // Checking if ghost is bored
            if (ghost->boredom <= 0) {
                // Ending this thread by breaking main loop
                printf("| %20s | %30s | %20s |\n", "Ghost", "leaving building", "BORED");
                break;
            }

            // Choose next action, 20% to leave evidence, 20% to move, 60% to do nothing
            int rand = randInt(0, 5);

            // Leaving evidence
            if (rand == 0) {
                leaveEvidence(ghost);

            // Moving to an adjacent room
            } else if (rand == 1) {
                moveGhost(ghost);
            }
        }

        // Sleeping
        usleep(USLEEP_TIME);
    }
}