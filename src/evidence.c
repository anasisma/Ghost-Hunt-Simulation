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
    EvidenceNodeType* newNode = (EvidenceNodeType*)malloc(sizeof(EvidenceNodeType));

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
    EvidenceNodeType* i = list->head;
    EvidenceNodeType* iPrev = list->head;

    if (i == NULL) {  // If head is null then the list is empty
        printf("Removing evidence error: EvidenceListType is empty!\n");
        exit(C_ARR_ERR);
    }

    if (list->head->evidence == evidence) {
        list->head = list->head->next;
        free(i);
        return;
    }

    while (i != NULL) {
        if (i->evidence == evidence) {
            if (i == list->tail) {
                list->tail = iPrev;
                iPrev->next = NULL;
                free(i);
                return;
            } else {
                iPrev->next = i->next;
                free(i);
                return;
            }
        }
        iPrev = i;
        i = i->next;
    }
    // If we reach this point it means the evidence was not in the list
    printf("Removing evidence error: EvidenceType not in EvidenceListType! (at least 2 elements were in list)\n");
    exit(C_ARR_ERR);
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
//   Purpose: Return a randomly generated EMF value
void getEMF(float* value) {
    float val = randFloat(4.7, 5.0);
    *value = val;
}

//  Function: getTemperature
//     in/ou: Pointer to float used to return the value
//   Purpose: Return a randomly generated temperature value
void getTemperature(float* value) {
    float val = randFloat(-10, 1);
    *value = val;
}

//  Function: getFingerprints
//     in/ou: Pointer to float used to return the value
//   Purpose: Return a randomly generated fingerprint value
void getFingerprints(float* value) {
    *value = 1.0;
}

//  Function: getSound
//     in/ou: Pointer to float used to return the value
//   Purpose: Return a randomly generated decibel value
void getSound(float* value) {
    float val = randFloat(65, 75);
    *value = val;
}

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
            return "UNKNOW";
    }
}

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