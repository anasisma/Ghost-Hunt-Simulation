#include "defs.h"

//   Function:  initEvidence
//      in/ou:  Pointer to location of EvidenceType to initialize
//    Purpose:  Allocates and initialize the given EvidenceType
void initEvidence(EvidenceType** evidence) {
    // Allocating memory for evidence
    EvidenceType* newEvid = (EvidenceType*)malloc(sizeof(EvidenceType));

    // Checking if memory was allocated correctly
    if (newEvid == NULL) {
        printf("Memory allocation error: couldn't malloc new EvidenceType!\n");
        exit(C_MEM_ERR);
    }

    // return newEvid structure using the pointer parameter
    *evidence = newEvid;
}

//   Function:  initEvidenceList
//      in/ou:  Pointer to location of EvidenceListType to initialize
//    Purpose:  Allocates and initializes parameters of given EvidenceListType
void initEvidenceList(EvidenceListType** list) {
    //Allocating memory for list
    EvidenceListType* newList = (EvidenceListType*)malloc(sizeof(EvidenceListType));

    // Checking if list was allocated correctly
    if (newList == NULL) {
        printf("Memory allocation error: couldn't malloc new EvidenceListType!\n");
        exit(C_MEM_ERR);
    }

    //Setting attributes of new EvidenceListType
    newList->head = NULL;
    newList->tail = NULL;

    //return newList structure using the pointer parameter
    *list = newList;
}

//  Function:  appendEvidence
//     in/ou:  Location of EvidenceListType to append new room to
//        in:  Location of EvidenceType to append to list
//   Purpose:  Adds EvidenceType to tail of the given EvidenceListType
void appendEvidence(EvidenceListType* list, EvidenceType* evidence) {
    // Allocating memory for new EvidenceNodeType
    EvidenceNodeType* newNode = (EvidenceNodeType*)malloc(sizeof(EvidenceNodeType));

    // Checking if memory was allocated correctly
    if (newNode == NULL) {
        printf("Memory allocation error: couldn't malloc new EvidenceNodeType!\n");
        exit(C_MEM_ERR);
    }

    // Setting attributes of new EvidenceNodeType
    newNode->evidence = evidence;
    newNode->next = NULL;

    // If list is empty, set new node as head and tail
    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;

        // If list is not empty, set new node as tail
    } else {
        list->tail->next = newNode;
        list->tail = newNode;
    }
}

//  Function:  removeEvidence
//     in/ou:  Location of EvidenceListType to remove evidence from
//        in:  Location of EvidenceType to remove
//   Purpose:  Remove evidence from given EvidenceListType and frees memory associated with removed node
void removeEvidence(EvidenceListType* list, EvidenceType* evidence) {
    // Making temporary pointers
    EvidenceNodeType* i = list->head;
    EvidenceNodeType* iPrev = list->head;

    // Checking if list is empty
    if (i == NULL) {
        printf("Removing evidence error: EvidenceListType is empty!\n");
        exit(C_ARR_ERR);
    }

    // Checking if evidence to remove is head of list
    if (list->head->evidence == evidence) {
        list->head = list->head->next;
        free(i);
        return;
    }

    // Looping through list to find matching evidence
    while (i != NULL) {
        // Remove evidenceType that matches
        if (i->evidence == evidence) {
            // Checking if evidenceType to remove is tail
            if (i == list->tail) {
                // if we need to remove the tail, make the tail point to the prev node
                list->tail = iPrev;
                // make the next of the new tail point to null
                iPrev->next = NULL;
                // free the old tail node
                free(i);
                // exit the function
                return;
            } else {  // node to delete is not the tail
                // we just need to make the prev node point to whatever is after it
                iPrev->next = i->next;
                // free the node
                free(i);
                return;
            }
        }
        // iterator forward, while having iPrev be 1 step behind
        iPrev = i;
        i = i->next;
    }

    // List did not contain evidenceType to delete, this is undefined behaviour therefor exiting process
    printf("Removing evidence error: EvidenceType not in EvidenceListType! (at least 2 elements were in list)\n");
    exit(C_ARR_ERR);
}

//   Function:  createEvidence
//         in:  GhostClassType to create evidence from
//      in/ou:  Pointer to EvidenceType to be created
//    Purpose:  Gives given EvidenceType a class and value based on the given GhostClassType
void createEvidence(GhostClassType ghostClass, EvidenceType* evidence) {
    float value;
    int evidenceType = randInt(0, 3);
    EvidenceClassType evidenceClass;

    // Generating evidence value and class based on GhostClassType
    switch (ghostClass) {
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
//        in: Pointer to EvidenceType to evaluate
//   Purpose: Determine if the given EvidenceType's value is within the ghostly range
//    Return: Returns C_TRUE if the given evidence is ghostly, C_FALSE otherwise
int isGhostlyVal(EvidenceType* evidence) {
    // Determining validity of evidence value based on evidence's EvidenceClassType
    // return C_TRUE if value is ghostly
    switch (evidence->evidenceClass) {
        case EMF:
            if (evidence->value > 4.9) {
                return (C_TRUE);
            }
            return (C_FALSE);

        case TEMPERATURE:
            if (evidence->value < 0) {
                return (C_TRUE);
            }
            return (C_FALSE);

        case FINGERPRINTS:
            if (evidence->value == 1) {
                return (C_TRUE);
            }
            return (C_FALSE);

        case SOUND:
            if (evidence->value > 70) {
                return (C_TRUE);
            }
            return (C_FALSE);
    }
}

//  Function: getEMF
//     in/ou: Pointer to float used to return the value
//   Purpose: Returns a randomly generated EMF value in the ghost range (NOT ALWAYS GHOSTLY)
void getEMF(float* value) {
    *value = randFloat(4.7, 5.0);
}

//  Function: getTemperature
//     in/ou: Pointer to float used to return the value
//   Purpose: Returns a randomly generated temperature value in the ghost range (NOT ALWAYS GHOSTLY)
void getTemperature(float* value) {
    *value = randFloat(-10, 1);
}

//  Function: getFingerprints
//     in/ou: Pointer to float used to return the value
//   Purpose: Returns a fingerprint value in the ghost range (ALWAYS GHOSTLY)
void getFingerprints(float* value) {
    *value = 1.0;
}

//  Function: getSound
//     in/ou: Pointer to float used to return the value
//   Purpose: Returns a randomly generated sound value in the ghost range (NOT ALWAYS GHOSTLY)
void getSound(float* value) {
    *value = randFloat(65, 75);
}

// Function:  getTypeString
//       in:  EvidenceClassType to get string from
//   Return:  Returns the string representation of the given EvidenceClassType
char* getTypeString(EvidenceClassType type) {
    switch (type) {
        case EMF:
            return "EMF";

        case TEMPERATURE:
            return "TEMPERATURE";

        case FINGERPRINTS:
            return "FINGERPRINTS";

        case SOUND:
            return "SOUND";

        default:
            return "UNKNOWN";
    }
}

// Function:  cleanupEvidenceListData
//    in/ou:  Location of EvidenceListType to cleanup data
//  Purpose:  Frees all the EvidenceTypes contained in given linkedList
void cleanupEvidenceListData(EvidenceListType* list) {
    // Temporary pointers
    EvidenceNodeType* currNode = list->head;
    EvidenceNodeType* nextNode;

    // Looping through all nodes in list
    while (currNode != NULL) {
        nextNode = currNode->next;

        // Freeing evidence
        free(currNode->evidence);

        currNode = nextNode;
    }
}

// Function:  cleanupEvidenceListNodes
//    in/ou:  Location of EvidenceListType to cleanup nodes
//  Purpose:  Frees all the EvidenceNodeTypes contained in given linkedList
void cleanupEvidenceListNodes(EvidenceListType* list) {
    // Temporary pointers
    EvidenceNodeType* currNode = list->head;
    EvidenceNodeType* nextNode;

    // Looping through all nodes in list
    while (currNode != NULL) {
        nextNode = currNode->next;

        // Freeing node
        free(currNode);

        currNode = nextNode;
    }
}