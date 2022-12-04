#include "defs.h"

void initHunter(HunterType* hunter) {

    hunter = (HunterType*)malloc(sizeof(HunterType));

        // Checking if hunter was allocated correctly
        if (hunter == NULL) {
            printf("Memory allocation error: couldn't malloc new hunter!\n");
            exit(C_MEM_ERR);
        }
}


//   Function:  startHunter
//         in/ou:  Location of building hunter is in
//    Purpose:  Hunter thread start function
void *startHunter(void* building) {

}