#include "defs.h"

//   Function:  initEvidence
//         in:  Location of EvidenceType to be modified
//         in:  Attributes of EvidenceType to be added
//        out:  Modified EvidenceType
//    Purpose:  Initializes all fields of the given EvidenceType parameter
void initEvidence(EvidenceType** evidence) {
    EvidenceType* newEvid = (EvidenceType*)malloc(sizeof(EvidenceType));  // allocate mem for new ghost
    if (newEvid == NULL) {                                                // if memory failed to allocate, shut the program down
        printf("Memory allocation error\n");
        exit(C_MEM_ERR);
    }

    *evidence = newEvid;  // return this new structure using the evidence parameter
}

/*   Function:  initEvidenceList                                        */
/*         in:  Location of EvidenceListType to be modified            */
/*        out:  Modified EvidenceListType                               */
/*    Purpose:  initializes both fields of the given list parameter to default values      */
void initEvidenceList(EvidenceListType* list) {
    list->head = NULL;
    list->tail = NULL;  // set both head and tail (list starts as empty)
}

//  Function:  appendEvidence
//     in/ou:  Location of EvidenceListType to append new room to
//        in:  Location of EvidenceType to append to list
//   Purpose:  Adds EvidenceType to tail of the given EvidenceListType
void appendEvidence(EvidenceListType* list, EvidenceType* evidence) {
    EvidenceNodeType* newNode = (EvidenceNodeType*)calloc(1, sizeof(EvidenceNodeType));

    // if memory failed to allocate, shut the program down
    if (newNode == NULL) {
        printf("Memory allocation error\n");
        exit(0);
    }
    newNode->evidence = evidence;
    newNode->next = NULL;

    if (list->head == NULL) {  // if list is empty, both head and tail need to point to same node
        list->head = newNode;
        list->tail = newNode;
    } else {
        EvidenceNodeType* iterator = list->head;
        while (iterator->next != NULL) {
            iterator = iterator->next;
        }
        list->tail->next = newNode;
        list->tail = newNode;
    }
}

//  Function:  removeEvidence
//     in/ou:  Location of EvidenceListType to remove evidence from
//        in:  Location of EvidenceType to remove
//   Purpose:  Remove evidence from given EvidenceListType
void removeEvidence(EvidenceListType* list, EvidenceType* evidence) {
    // // Creating temporary node pointer
    // EvidenceNodeType* currNode;
    // currNode = list->head;

    // while (currNode != NULL) {
    //     printf("%p\n", currNode->evidence);
    //     currNode = currNode->next;
    // }

    // currNode = list->head;

    // // If list is empty, there's nothing to delete
    // if (list->head == NULL) {
    //     printf("Removing evidence error: EvidenceListType is empty!\n");
    //     exit(C_ARR_ERR);
    // }

    // // Checking if head matches evidence to delete
    // if (list->head->evidence == evidence) {

    //     if (list->head->next != NULL) {
    //          list->head = list->head->next;
    //     } else {
    //         list->head = NULL;
    //     }
    //     // Deleting head
    //     free(currNode);
    //     return;
    // }

    // // Looping through list
    // while (currNode->next != NULL) {
    //     // Checking if currNode matches evidence to delete
    //     if (currNode->next->evidence == evidence) {

    //         EvidenceNodeType* delNode = currNode->next;

    //         //Checking if node to delete is tail
    //         if (currNode->next == list->tail) {
    //             list->tail = currNode;
    //             currNode->next = NULL;
    //         }
            
    //         // Deleting node
    //         currNode->next = delNode->next;

    //         // Deleting head
    //         free(currNode);
    //         return;
    //     }

    //     currNode = currNode->next;
    // }

    // // If we reach this point it means the evidence was not in the list
    // printf("Removing evidence error: EvidenceType not in EvidenceListType!\n");
    // exit(C_ARR_ERR);

    EvidenceNodeType* iterator = list->head;
    if (iterator == NULL) {  // If head is null then the list is empty
        printf("Removing evidence error: EvidenceListType is empty!\n");
        exit(C_ARR_ERR);
    } else if (iterator->next == NULL) {  // If only the head/tail is non-null
        if (!(iterator->evidence == evidence)) {
            // If we reach this point it means the evidence was not in the list
            printf("Removing evidence error: EvidenceType not in EvidenceListType! (only head was left)\n");
            exit(C_ARR_ERR);
        }
        // If only the head/tail exist, free the node and make list's pointers point to null
        free(iterator);
        list->head = NULL;
        list->tail = NULL;
    } else {  
        // If the list's head is the one we want to remove
        if (iterator->evidence == evidence) {
            // Changing head
            list->head = iterator->next;

            // Deleting head
            free(iterator);
            return;
        } else {
            // Loop until evidence is found in the list
            while (iterator->next != NULL) {
                if (iterator->next->evidence == evidence) {
                    // Make a temp node to free later
                    EvidenceNodeType* delNode = iterator->next;
                    // Change pointers to skip deleted node
                    iterator->next = iterator->next->next;
                    // Freeing deleted node
                    free(delNode);
                    break;
                }
                iterator = iterator->next;
            }
        }
        printf("value we wanted to delete is %p\n", evidence);
        // If we reach this point it means the evidence was not in the list
        printf("Removing evidence error: EvidenceType not in EvidenceListType! (at least 2 elements were in list)\n");
        exit(C_ARR_ERR);
    }
}

//   Function:  createEvidence
//         in:  Pointer to GhostType to determine ghost class
//      in/ou:  Pointer to EvidenceType to be created
//    Purpose:  Determines the class of the evidence as well as its value
void createEvidence(GhostType* ghost, EvidenceType* evidence) {
    float value;
    int evidenceType = randInt(0, 3);
    EvidenceClassType evidenceClass;

    // Generating evidence value based on ghost type
    switch (ghost->ghostClass) {
        case POLTERGEIST:
            if (evidenceType == 0) {
                getEMF(&value);
                evidenceClass = EMF;
            } else if (evidenceType == 1) {
                getTemperature(&value);
                evidenceClass = TEMPERATURE;
            } else {
                getFingerprints(&value);
                evidenceClass = FINGERPRINTS;
            }
            break;

        case BANSHEE:
            if (evidenceType == 0) {
                getEMF(&value);
                evidenceClass = EMF;
            } else if (evidenceType == 1) {
                getTemperature(&value);
                evidenceClass = TEMPERATURE;
            } else {
                getSound(&value);
                evidenceClass = SOUND;
            }
            break;

        case BULLIES:
            if (evidenceType == 0) {
                getEMF(&value);
                evidenceClass = EMF;
            } else if (evidenceType == 1) {
                getFingerprints(&value);
                evidenceClass = FINGERPRINTS;
            } else {
                getSound(&value);
                evidenceClass = SOUND;
            }
            break;

        case PHANTOM:
            if (evidenceType == 0) {
                getTemperature(&value);
                evidenceClass = TEMPERATURE;
            } else if (evidenceType == 1) {
                getFingerprints(&value);
                evidenceClass = FINGERPRINTS;
            } else {
                getSound(&value);
                evidenceClass = SOUND;
            }
            break;
    }

    // Setting evidence's attributes
    evidence->value = value;
    evidence->evidenceClass = evidenceClass;
}

//  Function: isGhostlyVal
//        in: Pointer to evidence to evalute
//   Purpose: Determine if the evidence's value is within the "ghostly" range
int isGhostlyVal(EvidenceType* evidence) {
    // Determining validity of evidence value based on evidence type
    switch (evidence->evidenceClass) {
        case EMF:
            if (evidence->value > 4.9 && evidence->value <= 5) {
                return (C_TRUE);
            }
            return (C_FALSE);

        case TEMPERATURE:
            if (evidence->value >= -10 && evidence->value < 0) {
                return (C_TRUE);
            }
            return (C_FALSE);

        case FINGERPRINTS:
            if (evidence->value == 1) {
                return (C_TRUE);
            }
            return (C_FALSE);

        case SOUND:
            if (evidence->value > 70 && evidence->value <= 75) {
                return (C_TRUE);
            }
            return (C_FALSE);
    }
}

//  Function: getEMF
//     in/ou: Pointer to float used to return the value
//   Purpose: Return a randomly generated EMF value
void getEMF(float* value) {
    float val = randFloat(0, 6);
    *value = val;
}

//  Function: getTemperature
//     in/ou: Pointer to float used to return the value
//   Purpose: Return a randomly generated temperature value
void getTemperature(float* value) {
    float val = randFloat(-10, 28);
    *value = val;
}

//  Function: getFingerprints
//     in/ou: Pointer to float used to return the value
//   Purpose: Return a randomly generated fingerprint value
void getFingerprints(float* value) {
    int val = randInt(1, 2);
    float floatVal = (float)val;
    *value = floatVal;
}

//  Function: getSound
//     in/ou: Pointer to float used to return the value
//   Purpose: Return a randomly generated decibel value
void getSound(float* value) {
    float val = randFloat(40, 76);
    *value = val;
}