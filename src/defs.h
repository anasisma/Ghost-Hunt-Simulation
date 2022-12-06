#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define C_TRUE 1
#define C_FALSE 0
#define C_NO_ROOMS -5
#define C_SEM_ERR -4
#define C_ARR_ERR -3
#define C_ARR_FULL -2
#define C_MEM_ERR -1
#define MAX_STR 64
#define FEAR_RATE 5
#define MAX_FEAR 100
#define MAX_HUNTERS 4
#define USLEEP_TIME 50000
#define BOREDOM_MAX 99

typedef enum { EMF,
               TEMPERATURE,
               FINGERPRINTS,
               SOUND } EvidenceClassType;
typedef enum { POLTERGEIST,
               BANSHEE,
               BULLIES,
               PHANTOM } GhostClassType;

typedef struct EvidenceType {
    float value;
    EvidenceClassType evidenceClass;
} EvidenceType;

typedef struct EvidenceNodeType {
    struct EvidenceNodeType* next;
    EvidenceType* evidence;
} EvidenceNodeType;

typedef struct EvidenceListType {
    EvidenceNodeType* head;
    EvidenceNodeType* tail;
} EvidenceListType;

typedef struct RoomNodeType {
    struct RoomNodeType* next;
    struct RoomType* room;
} RoomNodeType;

typedef struct RoomListType {
    RoomNodeType* head;
    RoomNodeType* tail;
    int roomCount;
} RoomListType;

typedef struct RoomType {
    char name[MAX_STR];
    RoomListType connectedRooms;
    EvidenceListType evidenceList;
    struct HunterType* hunters[MAX_HUNTERS];
    struct GhostType* ghost;
    int hunterCount;
    sem_t mutex;
} RoomType;

typedef struct GhostType {
    int boredom;
    GhostClassType ghostClass;
    RoomType* room;
} GhostType;

typedef struct HunterType {
    struct BuildingType* building;
    RoomType* room;
    EvidenceClassType evidenceClass;
    EvidenceListType* evidenceList;
    char name[MAX_STR];
    int fear;
    int boredom;
    int suspicious;
    sem_t mutex;
} HunterType;

typedef struct BuildingType {
    GhostType* ghost;
    RoomListType rooms;
    EvidenceListType evidenceList;
    HunterType* hunters[MAX_HUNTERS];
    sem_t mutex;
} BuildingType;

int randInt(int, int);          // Generates a pseudorandom integer between the parameters
float randFloat(float, float);  // Generates a pseudorandom float between the parameters

void populateRooms(BuildingType*);  // Populates the building with sample data for rooms

// all the forward definitions for functions
void initRoom();
void initRoomList(RoomListType*);
void appendRoom(RoomListType*, RoomNodeType*);
void connectRooms(RoomType*, RoomType*);
void addHunter(RoomType*, HunterType*);
void removeHunter(RoomType*, HunterType*);
int checkIfFoundGhost(HunterType*);
void cleanupRoom(RoomType*);
void getEvidence(HunterType*);
void communicateEvidence(HunterType*);
void initEvidenceList(EvidenceListType*);

void initGhost(int, GhostClassType, GhostType**);
void *startGhost(void*);
void *startHunter(void*);
void leaveEvidence(GhostType*);
void cleanupGhost(GhostType*);
int ghostInRoom(HunterType*);

void initBuilding(BuildingType*);
void printResults(BuildingType*);

void initEvidence(EvidenceType**);
void createEvidence(GhostType*, EvidenceType*);
void appendEvidence(EvidenceListType*, EvidenceType*);
void removeEvidence(EvidenceListType*, EvidenceType*);
void getEMF(float*);
void getTemperature(float*);
void getSound(float*);
void getFingerprints(float*);
int isGhostlyVal(EvidenceType*);

void getHunterNames(char**);
void initHunters(pthread_t*);
void createInitHunters(HunterType**, char**);
void createHunterThreads(pthread_t**);
void createStdEvidence(HunterType*, EvidenceType*);
void placeGhostRandRoom(GhostType*, BuildingType*);
void printHunterResult(HunterType*);
void printSuspicions(HunterType*);

void cleanupBuilding(BuildingType*);
void cleanupRoomList(RoomListType*);
void cleanupEvidenceListNodes(EvidenceListType*);
void cleanupEvidenceListData(EvidenceListType*);
void cleanupHunter(HunterType*);
void cleanupRoomListData(RoomListType*);