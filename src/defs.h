#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_STR 64
#define FEAR_RATE 1
#define MAX_FEAR 100
#define MAX_HUNTERS 4
#define USLEEP_TIME 50000
#define BOREDOM_MAX 99

typedef struct EvidenceNode {
    struct EvidenceNode* next;
    EvidenceType* evidence;
} EvidenceNodeType;

typedef struct EvidenceList {
    EvidenceNodeType* head;
    EvidenceNodeType* tail;
} EvidenceListType;

typedef struct RoomNode {
    struct RoomNode* next;
    RoomType* room;
} RoomNodeType;

typedef struct RoomList {
    RoomNodeType* head;
    RoomNodeType* tail;
} RoomListType;

typedef struct Building {
    GhostType* ghost;
    RoomListType* rooms;
    HunterType** hunters[MAX_HUNTERS];
} BuildingType;

typedef struct Room {
    char name[MAX_STR];
    RoomListType* connectedRooms;
    EvidenceListType* evidenceList;
    HunterType* hunters[MAX_HUNTERS];
    GhostType* ghost;
} RoomType;

typedef struct Ghost {
    int boredom;
    GhostClassType ghostClass;
    RoomType* room;
} GhostType;

typedef struct Hunter {
    RoomType* room;
    EvidenceClassType evidenceClass;
    EvidenceListType* evidenceList;
    char name[MAX_STR];
    int fear;
} HunterType;

typedef struct Evidence {
    float value;
    EvidenceClassType evidenceClass;
} EvidenceType;

typedef enum { EMF,
               TEMPERATURE,
               FINGERPRINTS,
               SOUND } EvidenceClassType;
typedef enum { POLTERGEIST,
               BANSHEE,
               BULLIES,
               PHANTOM } GhostClassType;

int randInt(int, int);          // Generates a pseudorandom integer between the parameters
float randFloat(float, float);  // Generates a pseudorandom float between the parameters

void populateRooms(BuildingType*);  // Populates the building with sample data for rooms

//all the forward definitions for functions
void initRoom();