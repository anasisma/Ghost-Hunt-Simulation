#include "defs.h"

//   Function:  initEvidence
//         in:  Location of EvidenceType to be modified
//         in:  Attributes of EvidenceType to be added
//        out:  Modified EvidenceType
//    Purpose:  Initializes all fields of the given EvidenceType parameter
void initEvidence(EvidenceType **evidence) {
    EvidenceType *newEvid = (EvidenceType *)malloc(sizeof(EvidenceType));  // allocate mem for new ghost
    if (newEvid == NULL) {                                                 // if memory failed to allocate, shut the program down
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
    // Allocating memory for new EvidenceNodeType
    EvidenceNodeType* newEvidenceNode = (EvidenceNodeType*)malloc(sizeof(EvidenceNodeType));

    // Checking for memory allocation error
    if (newEvidenceNode == NULL) {
        printf("Memory allocation error: couldn't malloc new EvidenceNodeType!\n");
        exit(C_MEM_ERR);
    }

    // Setting attributes of newEvidenceNode
    newEvidenceNode->evidence = evidence;
    newEvidenceNode->next = NULL;

    // If list is empty, both head and tail need to point to new node
    if (list->head == NULL) {
        list->head = newEvidenceNode;
        list->tail = newEvidenceNode;

        // Otherwise add node after the tail
    } else {
        list->tail->next = newEvidenceNode;
        list->tail = newEvidenceNode;
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

    //Generating evidence value based on ghost type
    switch(ghost->ghostClass) {
        
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

    //Setting evidence's attributes
    evidence->value = value;
    evidence->evidenceClass = evidenceClass;
}

//  Function: getEMF
//     in/ou: Pointer to float used to return the value
//   Purpose: Return a randomly generated EMF value
void getEMF(float* value) {
    float val = randFloat(0,6);
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
    float val = randFloat(0, 2);
    *value = val;
}

//  Function: getSound
//     in/ou: Pointer to float used to return the value
//   Purpose: Return a randomly generated decibel value
void getSound(float* value) {
    float val = randFloat(40, 76);
    *value = val;
}