//Libraries to include
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

//Constants and definitions
#define C_TRUE 1
#define C_FALSE 0
#define C_NO_ROOMS -5
#define C_SEM_ERR -4
#define C_ARR_ERR -3
#define C_ARR_FULL -2
#define C_MEM_ERR -1
#define MAX_STR 21
#define BOREDOM_RATE 1
#define FEAR_RATE 5
#define MAX_FEAR 100
#define MAX_HUNTERS 4
#define USLEEP_TIME 50000
#define BOREDOM_MAX 100

//Enum definitions
typedef enum { EMF,
               TEMPERATURE,
               FINGERPRINTS,
               SOUND } EvidenceClassType;
typedef enum { POLTERGEIST,
               BANSHEE,
               BULLIES,
               PHANTOM } GhostClassType;

//Struct definitions
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

// all the forward definitions for functions
void appendRoom(RoomListType*, RoomNodeType*);
void connectRooms(RoomType*, RoomType*);
void addHunter(RoomType*, HunterType*);
void removeHunter(RoomType*, HunterType*);
void communicateEvidence(HunterType*);
void* startGhost(void*);
void* startHunter(void*);
void leaveEvidence(GhostType*);
void appendEvidence(EvidenceListType*, EvidenceType*);
void removeEvidence(EvidenceListType*, EvidenceType*);

void populateRooms(BuildingType*); 
void placeGhostRandRoom(GhostType*, BuildingType*);
char* getTypeString(EvidenceClassType);
int randInt(int, int);
int checkIfFoundGhost(HunterType*);
int isGhostlyVal(EvidenceType*);
int ghostInRoom(HunterType*);
float randFloat(float, float);

void initRoom();
void initRoomList(RoomListType*);
void initGhost(GhostType**, GhostClassType);
void initHunters(pthread_t*);
void initEvidence(EvidenceType**);
void initEvidenceList(EvidenceListType**);
void initBuilding(BuildingType*);

void getEMF(float*);
void getTemperature(float*);
void getSound(float*);
void getFingerprints(float*);
void getHunterNames(char**);
void getEvidence(HunterType*);

void createEvidence(GhostClassType, EvidenceType*);
void createInitHunters(HunterType**, char**);
void createHunterThreads(pthread_t**);
void createStdEvidence(EvidenceClassType, EvidenceType*);

void printHunterResult(HunterType*);
void printSuspicions(HunterType*);
void printResults(BuildingType*);
void printWinner(BuildingType*);
void determineGhostType(BuildingType*, int, int, int, int);

void cleanupBuilding(BuildingType*);
void cleanupRoomListNodes(RoomListType*);
void cleanupEvidenceListNodes(EvidenceListType*);
void cleanupEvidenceListData(EvidenceListType*);
void cleanupHunter(HunterType*);
void cleanupRoomListData(RoomListType*);
void cleanupGhost(GhostType*);
void cleanupRoom(RoomType*);