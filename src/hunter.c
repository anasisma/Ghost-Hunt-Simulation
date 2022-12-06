#include "defs.h"

void initHunter(HunterType** hunter) {
    HunterType* newHunter = (HunterType*)malloc(sizeof(HunterType));

    // Checking if hunter was allocated correctly
    if (newHunter == NULL) {
        printf("Memory allocation error: couldn't malloc new hunter!\n");
        exit(C_MEM_ERR);
    }

    // Initlizing mutex
    if (sem_init(&(newHunter->mutex), 0, 1) < 0) {
        printf("Semaphore error: couldn't initlize hunter mutex!\n");
        exit(C_SEM_ERR);
    }

    EvidenceListType* list = (EvidenceListType*)malloc(sizeof(EvidenceListType));
    initEvidenceList(list);

    newHunter->suspicious = C_FALSE;
    newHunter->evidenceList = list;
    newHunter->room = NULL;
    newHunter->boredom = BOREDOM_MAX;
    newHunter->fear = 0;
    *hunter = newHunter;
}

//  Function: createInitHunters
//     in/ou: Pointer to array of HunterType pointers
//   Purpose: Creates and initializes HunterTypes and stores them in given array
void createInitHunters(HunterType** hunters, char** names) {
    for (int i = 0; i < MAX_HUNTERS; i++) {
        HunterType* hunter;              // Temp hunter struct used to assign values
        initHunter(&hunter);             // Initialize the temp struct
        hunter->evidenceClass = i;       // Each hunter will have a unique device
        strcpy(hunter->name, names[i]);  // Copy the name to the hunter
        hunters[i] = hunter;             // Assign this hunter struct to the array of hunters
    }
}

//   Function:  makeMove
//      in/ou:  Location of HunterType
//    Purpose:  Trys to move hunter to a random connected room, if rooms are locked, skips turn
void moveHunter(HunterType* hunter) {
    // Check if current room is available for modification
    if (sem_trywait(&(hunter->room->mutex)) == 0) {
        // Get a random connected room to move to
        int nextRoom = randInt(0, hunter->room->connectedRooms.roomCount);
        RoomNodeType* iterator = hunter->room->connectedRooms.head;
        for (int i = 0; i < nextRoom; i++) {
            iterator = iterator->next;
        }
        RoomType* newRoom = iterator->room;
        RoomType* currentRoom = hunter->room;

        // Check if current room is available for modification
        if (sem_trywait(&(newRoom->mutex)) == 0) {
            // Remove hunter from its current room
            removeHunter(currentRoom, hunter);

            // Move hunter to new room
            addHunter(newRoom, hunter);

            printf("| %20s | %30s | %20s |\n", hunter->name, "changed location", hunter->room->name);

            // Unlocking new room
            sem_post(&(newRoom->mutex));
        }

        // Unlocking current room
        sem_post(&(currentRoom->mutex));
    }
}

//  Function: getEvidence
//        in: Pointer to hunter that will be getting the evidence
//       out: Modified room that the hunter is in
//   Purpose: Check if current room is available, if so get evidence and remove it from room
void getEvidence(HunterType* hunter) {
    int evidFound = C_FALSE;

    // Check if current room is available for modification
    if (sem_trywait(&(hunter->room->mutex)) == 0) {
        // Wait for hunters list to be available
        sem_wait(&(hunter->mutex));
        // Wait for buildings list to be avaible
        sem_wait(&(hunter->building->mutex));
        // Creating temporary node pointer
        EvidenceNodeType* currNode;
        currNode = hunter->room->evidenceList.head;

        // Looping through list
        while (currNode != NULL) {
            // Checking if currNode has evidence hunter can pick up
            if (currNode->evidence->evidenceClass == hunter->evidenceClass) {
                evidFound = C_TRUE;
                // append evidence to hunter's list and remove it from room's list
                appendEvidence(hunter->evidenceList, currNode->evidence);

                if (isGhostlyVal(currNode->evidence)) {
                    printf("| %20s | %30s | %20s |\n", hunter->name, "found ghostly evidence", getTypeString(currNode->evidence->evidenceClass));
                } else {
                    printf("| %20s | %30s | %20s |\n", hunter->name, "found random evidence", getTypeString(currNode->evidence->evidenceClass));
                }

                // Append evidence to buildings list
                appendEvidence(&hunter->building->evidenceList, currNode->evidence);
                removeEvidence(&(hunter->room->evidenceList), currNode->evidence);

                hunter->boredom = BOREDOM_MAX;  // reset hunter's boredom since they found ghostly evidence
                break;                          // break to immediately exit the loop, to not add more evidence
            }

            currNode = currNode->next;
        }

        if (!evidFound) {
            // Initalizing new evidence
            EvidenceType* evidence;
            initEvidence(&evidence);

            // Creating evidence based on hunter's device type
            createStdEvidence(hunter, evidence);

            // Adding evidence to hunter's list
            appendEvidence(hunter->evidenceList, evidence);
            // Append evidence to buildings list
            appendEvidence(&hunter->building->evidenceList, evidence);
            printf("| %20s | %30s | %20s |\n", hunter->name, "found random evidence", getTypeString(evidence->evidenceClass));
        }

        // Unlocking buildings list
        sem_post(&(hunter->building->mutex));
        // Unlocking hunters list
        sem_post(&(hunter->mutex));
        // Unlocking current room
        sem_post(&(hunter->room->mutex));
    }
}

//   Function:  createStdEvidence
//         in:  Pointer to GhostType to determine ghost class
//      in/ou:  Pointer to EvidenceType to be created
//    Purpose:  Determines the class of the evidence as well as its value
void createStdEvidence(HunterType* hunter, EvidenceType* evidence) {
    float value;

    // Generating evidence value based on hunter's device
    switch (hunter->evidenceClass) {
        case EMF:
            value = randFloat(0, 4.9);
            break;

        case TEMPERATURE:
            value = randFloat(0, 27);
            break;

        case FINGERPRINTS:
            value = 0;
            break;

        case SOUND:
            value = randFloat(40, 70);
            break;
    }

    // Setting evidence's attributes
    evidence->value = value;
    evidence->evidenceClass = hunter->evidenceClass;
}

//   Function:  ghostInRoom
//         in:  Location of HunterType
//    Purpose:  Check if there is a ghost in the same room as given hunter
int ghostInRoom(HunterType* hunter) {
    if (hunter->room->ghost != NULL)
        return C_TRUE;
    return C_FALSE;
}

//   Function:  communicateEvidence
//      in/ou:  Location of HunterType that will share evidence
//    Purpose:  Share evidence with hunters that exist in HunterType's room's array of hunters
void communicateEvidence(HunterType* hunter) {
    int hunterIndex;
    HunterType* fellowHunter;
    while (C_TRUE) {
        if (hunter->room->hunterCount <= 1) {
            return;
        }

        // Random int for the index of the hunter to share with
        hunterIndex = randInt(0, hunter->room->hunterCount);
        if (hunter->room->hunters[hunterIndex] != hunter) {
            fellowHunter = hunter->room->hunters[hunterIndex];
            break;
        }
        // If the randomly selected hunter is the same as the actual hunter, loop again to get another value
    }

        // Check if selected hunter's evidence list is available for modification
        if (sem_trywait(&(fellowHunter->mutex)) == 0) {
            // Node to iterate through hunter's evidence list
            EvidenceNodeType* iterator = hunter->evidenceList->head;
            while (iterator != NULL) {
                // If the current node's value is in the ghostly range
                if (isGhostlyVal(iterator->evidence)) {
                    // Int to know if 2nd hunter already has the evidence from current node
                    int exists = C_FALSE;

                    // Node to iterate through 2nd hunter's evidence list
                    EvidenceNodeType* iterator2 = fellowHunter->evidenceList->head;
                    while (iterator2 != NULL) {
                        // If outer loop's node and this node contain the same evidence, then it exists
                        if (iterator->evidence == iterator2->evidence) {
                            exists = C_TRUE;
                        }
                        iterator2 = iterator2->next;
                    }
                    // If it doesn't exist, append it to 2nd hunter's evidence list
                    if (!exists) {
                        appendEvidence(fellowHunter->evidenceList, iterator->evidence);
                    }
                }
                // Move to next node
                iterator = iterator->next;
            }
            printf("| %20s | %30s | %20s |\n", hunter->name, "shared evidence with", fellowHunter->name);
            // Unlocking fellow hunter
            sem_post(&(fellowHunter->mutex));
        } 
}

//   Function:  checkIfFoundGhost
//         in:  Location of HunterType
//    Purpose:  Check if given hunter has collected three different pieces of ghostly evidence
int checkIfFoundGhost(HunterType* hunter) {
    // Variables to know if hunter has each evidence type
    int emfFound = C_FALSE;
    int tempFound = C_FALSE;
    int soundFound = C_FALSE;
    int fingerFound = C_FALSE;

    // Node to iterate through hunter's evidence list
    EvidenceNodeType* iterator = hunter->evidenceList->head;
    if (iterator == NULL)
        return C_FALSE;  // If head is null, hunter has no evidence at all
    while (iterator != NULL) {
        // Determining if current evidence is ghostly
        // If evidence is ghostly then set its corresponding variable to true (C_TRUE = 1)
        switch (iterator->evidence->evidenceClass) {
            case EMF:
                if (isGhostlyVal(iterator->evidence))
                    emfFound = C_TRUE;
                break;

            case TEMPERATURE:
                if (isGhostlyVal(iterator->evidence))
                    tempFound = C_TRUE;
                break;

            case FINGERPRINTS:
                if (isGhostlyVal(iterator->evidence))
                    soundFound = C_TRUE;
                break;

            case SOUND:
                if (isGhostlyVal(iterator->evidence))
                    fingerFound = C_TRUE;
                break;
        }

        iterator = iterator->next;
    }

    // Return true if at least 3 of the evidence types were found by hunter
    if ((emfFound + tempFound + soundFound + fingerFound) >= 3)
        return C_TRUE;
    return C_FALSE;
}

//   Function:  startHunter
//      in/ou:  Location of HunterType
//    Purpose:  Hunter thread start function
void* startHunter(void* h) {
    HunterType* hunter = (HunterType*)h;

    // The first decision will always be to move, since there will not be evidence in van at first
    moveHunter(hunter);

    // Main loop
    while (C_TRUE) {

        // Check if hunter is in a room with a ghost
        if (ghostInRoom(hunter) == C_TRUE) {
            // Increasing hunter's fear and resetting boredome
            hunter->fear += FEAR_RATE;

            // Checking if hunter fear is too high
            if (hunter->fear >= MAX_FEAR) {
                // Ending this thread by breaking main loop
                printf("| %20s | %30s | %20s |\n", hunter->name, "leaving building", "SCARED");
                removeHunter(hunter->room, hunter);
                break;
            }

            // Hunter is not in room with a ghost
        } else {
            // Decreasing boredom counter
            hunter->boredom--;

            // Checking if hunter is bored
            if (hunter->boredom <= 0) {
                // Ending this thread by breaking main loop
                printf("| %20s | %30s | %20s |\n", hunter->name, "leaving building", "BORED");
                removeHunter(hunter->room, hunter);
                break;
            }
        }

        // Choose next action
        int rand = randInt(0, 10);

        // Check if hunter is alone
        if (hunter->room->hunterCount > 1 && hunter->evidenceList->head != NULL) {
            // If hunter is with other hunters, 30% to communicate, 50% to move, 20% to get evidence
            if (rand < 3) {
                communicateEvidence(hunter);
            } else if (rand == 3 || rand == 4) {
                getEvidence(hunter);
            } else {
                moveHunter(hunter);
            }

            // If hunter is alone in room, 80% to move and 20% to get evidence
        } else {
            if (rand < 8) {
                moveHunter(hunter);
            } else {
                getEvidence(hunter);
            }
        }

        // Checking if hunter found a ghost
        if (checkIfFoundGhost(hunter)) {
            // Ending this thread by breaking main loop
            hunter->suspicious = C_TRUE;
            printf("| %20s | %30s | %20s |\n", hunter->name, "leaving building", "FOUND GHOST");
            removeHunter(hunter->room, hunter);
            break;
        }

        // Sleep
        usleep(USLEEP_TIME);
    }
}

void cleanupHunter(HunterType* hunter) {
    cleanupEvidenceListNodes(hunter->evidenceList);
    // Freeing evidenceList pointer
    free(hunter->evidenceList);

    free(hunter);
}