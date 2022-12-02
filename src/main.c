#include "defs.h"

int main(int argc, char* argv[]) {
    // Initialize a random seed for the random number generators
    srand(time(NULL));

    // Creating array of hunter names
    char* names[MAX_HUNTERS];

    // Getting names from user
    printf("Ghost and hunters simulation!");
    getHunterNames(names);

    // Creating building, initilizing it, and populating it with rooms
    BuildingType building;
    initBuilding(&building);
    populateRooms(&building);

    // Creating and initlizing hunters
    HunterType* hunters[MAX_HUNTERS];
    createHunters(hunters);

    // Creating and initializing ghost
    GhostType* ghost;
    int ghostClass = randInt(0, 4);
    initGhost(BOREDOM_MAX, ghostClass, ghost);

    // Placing hunters into the van
    for (int i = 0; i < MAX_HUNTERS; i++) {
        addHunter(building.rooms->head, hunters[i]);
    }
    
    // Placing ghost in random room (other than van)
    placeGhostRandRoom(ghost, &building);

    // Creating and initilizing hunter threads
    pthread_t* hunterThreads[MAX_HUNTERS];
    createHunterThreads(hunters);

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

    // Freeing memory
    cleanupBuilding(&building);

    for (int i = 0; i < MAX_HUNTERS; i++) {
        free(hunterThreads[i]);
    }

    for (int i = 0; i < MAX_HUNTERS; i++) {
        free(names[i]);
    }

    return 0;
}
// Function: placeGhostRandRoom
//       in: Pointer to ghost to put in building
//    in/ou: Pointer to building where ghost will be added
//  Purpose: Puts ghost in a random room in the building (other than the van)
void placeGhostRandRoom(GhostType* ghost, BuildingType* building) {
    int roomNum = randInt(1, building->rooms->roomCount);
    RoomNodeType iterator = building->rooms->head;
    for (int i = 0; i < roomNum; i++) {
        iterator = iterator->next;
    }
    iterator->ghost = ghost;
}

//  Function: createInitHunters
//     in/ou: Pointer to array of HunterType pointers
//   Purpose: Allocates and initilizes HunterTypes and stores them in given array
void createInitHunters(HunterType* hunters) {
    for (int i = 0; i < MAX_HUNTERS; i++) {
        HunterType* hunter = (HunterType*)malloc(sizeof(HunterType));

        // Checking if hunter was allocated correctly
        if (hunter == NULL) {
            printf("Memory allocation error: couldn't malloc new hunter!\n");
            exit(C_MEM_ERR);
        }

        initHunter(hunter);
        hunters[i] = hunter;
    }
}

//  Function: createHunterThreads
//     in/ou: Pointer to array of pthread_t pointers
//   Purpose: Allocates pthread_t and stores them in given array
void createHunterThreads(pthread_t* hunterThreads) {
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
        printf("\nEnter the name of hunter %d: ", i + 1);
        scanf("%s", name);
        names[i] = name;
    }
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