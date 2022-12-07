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
    GhostClassType ghostClass = randInt(0, 4);
    initGhost(&ghost, ghostClass);
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

    // Freeing dynamically allocated hunter threads
    for (int i = 0; i < MAX_HUNTERS; i++) {
        free(hunterThreads[i]);
    }

    // Freeing names
    for (int i = 0; i < MAX_HUNTERS; i++) {
        free(names[i]);
    }

    printf("\n");
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
    // call function to print the result of each hunter in the building's hunter array
    for (int i = 0; i < MAX_HUNTERS; i++) {
        printHunterResult(building->hunters[i]);
    }
    printWinner(building);
    for (int i = 0; i < MAX_HUNTERS; i++) {
        // if the hunter array has a hunter that suspects a ghost, print their suspicions
        if (building->hunters[i]->suspicious) {
            printSuspicions(building->hunters[i]);
            // break, since in a large majority of cases, the hunters that win will have the same evidence
            break;
        }
    }
}

void printWinner(BuildingType* building) {
    int winners = C_FALSE;
    for (int i = 0; i < MAX_HUNTERS; i++) {
        if (building->hunters[i]->suspicious) {
            winners = C_TRUE;
        }
    }
    switch (winners) {
        case C_TRUE:
            printf("Hunters win! ");
            break;

        default:
            printf("Ghost wins! ");
            break;
    }
}

//  Function: printHunterResult
//        in: Pointer to hunter to print
//   Purpose: Prints the hunt results of a singular hunter
void printHunterResult(HunterType* hunter) {
    // print hunter's name, then the reason they left the building
    printf("    * %s ", hunter->name);
    if (hunter->fear >= 100) {
        printf("has run away in fear!\n");
    } else if (hunter->boredom <= 0) {
        printf("has run out of things to do!\n");
    } else {
        printf("uncovered the ghost!\n");
    }
}

// Function: printSuspicions
//       in: Pointer to hunter, to get the evidence list
//  Purpose: Find and print all the ghostly evidence values the hunter has
void printSuspicions(HunterType* hunter) {
    printf("Here is the evidence that was used to uncover the ghost: \n");
    // Node to iterate through evidence list
    EvidenceNodeType* i = hunter->evidenceList->head;
    // ints to store amount of each type of evidence found
    int emfs = 0, temps = 0, fngrprnts = 0, sounds = 0;
    // while it is possible to iterator
    while (i != NULL) {
        // if the current evidence's value is ghostly
        if (isGhostlyVal(i->evidence)) {
            // switch case to determine what to do based on evidenceClass
            switch (i->evidence->evidenceClass) {
                case EMF:
                    printf("    * EMF with a value of %f\n", i->evidence->value);
                    emfs++;
                    break;

                case TEMPERATURE:
                    printf("    * Temperature with a value of %f°C\n", i->evidence->value);
                    temps++;
                    break;

                case FINGERPRINTS:
                    fngrprnts++;
                    break;

                case SOUND:
                    printf("    * Sound with a value of %f dB\n", i->evidence->value);
                    sounds++;
                    break;
            }
        }
        // go to next node in the list
        i = i->next;
    }
    /* Since the fingerprints have values of 1 or 0
    we decided to print the amount of fingerprints rather than their values */
    if (fngrprnts > 0)
        printf("    * %d set(s) of paranormal fingerprints\n", fngrprnts);

    // call function to find out which type of ghost it was
    determineGhostType(hunter->building, emfs, temps, fngrprnts, sounds);
}

// Function: determineGhostType
//       in: Pointer BuildingType to check building's actual ghost type
//       in: int representing amount of EMF type ghostly evidences
//       in: int representing amount of temperature type ghostly evidences
//       in: int representing amount of fingerprint type ghostly evidences
//       in: int representing amount of sound type ghostly evidences
//  Purpose: Determine and print the type of the ghost, then check if that is the correct type
void determineGhostType(BuildingType* building, int emfs, int temps, int fingers, int sounds) {
    // int to store the type we determine from the evidence, values are from the enum GhostClassType
    int type = -1;
    printf("\nAccording to the evidence collected, it would seem that the ghost is a ");
    // if statements to determine the type of ghost based on evidence
    if (emfs > 0 && temps > 0 && fingers > 0) {
        printf("POLTERGEIST!\n");
        type = 0;
    } else if (emfs > 0 && temps > 0 && sounds > 0) {
        printf("BANSHEE!\n");
        type = 1;
    } else if (emfs > 0 && sounds > 0 && fingers > 0) {
        printf("BULLY!\n");
        type = 2;
    } else if (sounds > 0 && temps > 0 && fingers > 0) {
        printf("PHANTOM!\n");
        type = 3;
    }
    // statement to check if the ghostClass of the ghost in the building is equivalent to what we just determined
    if (building->ghost->ghostClass == type)
        printf("This deduction was correct!");
    else
        printf("This deduction was incorrect. :(");
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