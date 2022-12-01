#include "defs.h"

//   Function:  initGhost
//         in:  Location of GhostType to be modified
//         in:  Attributes of GhostType to be added
//        out:  Modified GhostType
//    Purpose:  Initializes all fields of the given GhostType parameter to parameters      */
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