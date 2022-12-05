#include "defs.h"

void initHunter(HunterType** hunter) {

    HunterType* newHunter = (HunterType*)malloc(sizeof(HunterType));

    // Checking if hunter was allocated correctly
    if (newHunter == NULL) {
        printf("Memory allocation error: couldn't malloc new hunter!\n");
        exit(C_MEM_ERR);
    }

    newHunter->room = NULL;
    *hunter = newHunter;  
}

//  Function: createInitHunters
//     in/ou: Pointer to array of HunterType pointers
//   Purpose: Creates and initilizes HunterTypes and stores them in given array
void createInitHunters(HunterType* hunters, char** names) {
    for (int i = 0; i < MAX_HUNTERS; i++) {
        HunterType* hunter;
        initHunter(&hunter);
        int class = randInt(0, 4);
        hunter->evidenceClass = class;
        strcpy(hunter->name, names[i]);
        hunters[i] = *hunter;
    }
}

//   Function:  startHunter
//         in/ou:  Location of building hunter is in
//    Purpose:  Hunter thread start function
void *startHunter(void* building) {

}