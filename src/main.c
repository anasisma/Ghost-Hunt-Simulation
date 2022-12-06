#include "defs.h"

int main(int argc, char* argv[]) {
    // Initialize a random seed for the random number generators
    srand(time(NULL));

    // Creating array of hunter names
    char* names[MAX_HUNTERS];

    // Getting names from user
    printf("Ghost and hunters simulation!\n");
    getHunterNames(names);

    // Creating building, initilizing it, and populating it with rooms
    BuildingType building;
    initBuilding(&building);
    populateRooms(&building);

    // Creating and initlizing hunters
    HunterType* hunters[MAX_HUNTERS];
    createInitHunters(hunters, names);

    // Placing hunters into the building's hunter array
    for (int i = 0; i < MAX_HUNTERS; i++) {
        building.hunters[i] = hunters[i];
        hunters[i]->building = &building;
    }

    // Creating and initializing ghost
    GhostType* ghost;
    int ghostClass = randInt(0, 4);
    initGhost(BOREDOM_MAX, ghostClass, &ghost);
    building.ghost = ghost;

    // Placing hunters into the van
    for (int i = 0; i < MAX_HUNTERS; i++) {
        addHunter(building.rooms.head->room, hunters[i]);
    }

    // Placing ghost in random room (other than van)
    placeGhostRandRoom(ghost, &building);

    // Creating and initilizing hunter threads
    pthread_t* hunterThreads[MAX_HUNTERS];
    createHunterThreads(hunterThreads);

    // Creating ghost thread
    pthread_t ghostThread;

    // Running threads
    pthread_create(&ghostThread, NULL, startGhost, ghost);

    for (int i = 0; i < MAX_HUNTERS; i++) {
        pthread_create(hunterThreads[i], NULL, startHunter, hunters[i]);
    }

    // Waiting on threads
    pthread_join(ghostThread, NULL);

    for (int i = 0; i < MAX_HUNTERS; i++) {
        pthread_join(*hunterThreads[i], NULL);
    }

    // Printing results of simulation
    printResults(&building);

    // Freeing memory
    cleanupBuilding(&building);

    //Freeing dynamically allocated hunter threads
    for (int i = 0; i < MAX_HUNTERS; i++) {
        free(hunterThreads[i]);
    }

    //Freeing names
    for (int i = 0; i < MAX_HUNTERS; i++) {
        free(names[i]);
    }

    printf("NICE\n");
    return 0;
}
// Function: placeGhostRandRoom
//       in: Pointer to ghost to put in building
//    in/ou: Pointer to building where ghost will be added
//  Purpose: Puts ghost in a random room in the building (other than the van)
void placeGhostRandRoom(GhostType* ghost, BuildingType* building) {
    int roomNum = randInt(1, building->rooms.roomCount);
    RoomNodeType* iterator = building->rooms.head;
    for (int i = 0; i < roomNum; i++) {
        iterator = iterator->next;
    }
    iterator->room->ghost = ghost;
    ghost->room = iterator->room;
}

//  Function: createHunterThreads
//     in/ou: Pointer to array of pthread_t pointers
//   Purpose: Allocates pthread_t and stores them in given array
void createHunterThreads(pthread_t** hunterThreads) {
    for (int i = 0; i < MAX_HUNTERS; i++) {
        pthread_t* hunterThread = (pthread_t*)malloc(sizeof(pthread_t));

        // Checking if hunterThread was allocated correctly
        if (hunterThread == NULL) {
            printf("Memory allocation error: couldn't malloc new hunter pthread_t!\n");
            exit(C_MEM_ERR);
        }

        hunterThreads[i] = hunterThread;
    }
}

//  Function: getHunterNames
//     in/ou: Pointer to array of char pointers
//   Purpose: Gets hunter names and stores them in the given array of char pointers
void getHunterNames(char** names) {
    // Looping through to get all hunters name
    for (int i = 0; i < MAX_HUNTERS; i++) {
        // Allocating memory for each hunter
        char* name = (char*)calloc(MAX_STR, sizeof(char));

        // Checking if name was allocated correctly
        if (name == NULL) {
            printf("Memory allocation error: couldn't calloc new hunter name!\n");
            exit(C_MEM_ERR);
        }

        // Getting name from standard input
        printf("Enter the name of hunter %d: ", i + 1);
        scanf("%s", name);
        names[i] = name;
    }
}

//  Function: printResults
//        in: Pointer to BuildingType where simulation has ended
//   Purpose: Prints the results of a completed simulation in a given building
//  Comments: Assumes function is only called on a building whose simulation has ended
void printResults(BuildingType* building) {
    printf("All done! Let's tally the results:\n");
    for (int i = 0; i < MAX_HUNTERS; i++) {
        printHunterResult(building->hunters[i]);
    }
    for (int i = 0; i < MAX_HUNTERS; i++) {
        if (building->hunters[i]->suspicious) {
            printSuspicions(building->hunters[i]);
            break;
        }
    }
}

//  Function: printHunterResult
//        in: Pointer to hunter to print
//   Purpose: Prints the hunt results of a singular hunter
void printHunterResult(HunterType* hunter) {
    printf("    * %s ", hunter->name);
    if (hunter->fear >= 100) {
        printf("has run away in fear!\n");
    } else if (hunter->boredom <= 0) {
        printf("has run out of things to do!\n");
    } else {
        printf("uncovered the ghost!\n");
    }
}

void printSuspicions(HunterType* hunter) {
    printf("Here is the evidence that was used to uncover the ghost: \n");
    EvidenceNodeType* i = hunter->evidenceList->head;
    int emfs = 0, temps = 0, fngrprnts = 0, sounds = 0;
    while (i != NULL) {
        if (isGhostlyVal(i->evidence)) {
            switch (i->evidence->evidenceClass) {
                case EMF:
                    printf("    *   EMF with a value of %f\n", i->evidence->value);
                    emfs++;
                    break;

                case TEMPERATURE:
                    printf("    *   Temperature with a value of %f°C\n", i->evidence->value);
                    temps++;
                    break;

                case FINGERPRINTS:
                    fngrprnts++;
                    break;

                case SOUND:
                    printf("    *   Sound with a value of %f dB\n", i->evidence->value);
                    sounds++;
                    break;
            }
        }
        i = i->next;
    }
    if (fngrprnts > 0)
        printf("    *   %d set(s) of paranormal fingerprints\n", fngrprnts);
}

/*
  Function:  randInt
  Purpose:   returns a pseudo randomly generated number,
             in the range min to (max - 1), inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [min, max-1)
*/
int randInt(int min, int max) {
    return rand() % (max - min) + min;
}

/*
  Function:  randFloat
  Purpose:   returns a pseudo randomly generated number,
             in the range min to max, inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [0, max-1)
*/
float randFloat(float a, float b) {
    // Get a percentage between rand() and the maximum
    float random = ((float)rand()) / (float)RAND_MAX;
    // Scale it to the range we want, and shift it
    return random * (b - a) + a;
}