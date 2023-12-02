#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define MAX_STR         64
#define MAX_RUNS        50
#define BOREDOM_MAX     100
#define C_TRUE          1
#define C_FALSE         0
#define HUNTER_WAIT     5000
#define GHOST_WAIT      600
#define NUM_HUNTERS     4
#define FEAR_MAX        10
#define LOGGING         C_TRUE
//new defines
#define MAX_EVIDENCE    3
#define ROOMS           30
#define HUNTERS         4

typedef enum EvidenceType EvidenceType;
typedef enum GhostClass GhostClass;

enum EvidenceType { EMF, TEMPERATURE, FINGERPRINTS, SOUND, EV_COUNT, EV_UNKNOWN};
enum GhostClass { POLTERGEIST, BANSHEE, BULLIES, PHANTOM, GHOST_COUNT, GH_UNKNOWN };
enum LoggerDetails { LOG_FEAR, LOG_BORED, LOG_EVIDENCE, LOG_SUFFICIENT, LOG_INSUFFICIENT, LOG_UNKNOWN };
//new classes
typedef   struct Room       RoomType;
typedef   struct RoomList   RoomListType;
typedef   struct RoomNode   RoomNodeType;
typedef   struct Ghost      GhostType;
typedef   struct Hunter     HunterType;
typedef   struct HunterArray  HunterArrayType;
typedef   struct House      HouseType; 
typedef   struct EvidenceList EvidenceListType;
typedef   struct EvidenceNode EvidenceNodeType;

// Helper Utilies
int randInt(int,int);        // Pseudo-random number generator function
float randFloat(float, float);  // Pseudo-random float generator function
enum GhostClass randomGhost();  // Return a randomly selected a ghost type
void ghostToString(enum GhostClass, char*); // Convert a ghost type to a string, stored in output paremeter
void evidenceToString(enum EvidenceType, char*); // Convert an evidence type to a string, stored in output parameter

// Logging Utilities
void l_hunterInit(char* name, enum EvidenceType equipment);
void l_hunterMove(char* name, char* room);
void l_hunterReview(char* name, enum LoggerDetails reviewResult);
void l_hunterCollect(char* name, enum EvidenceType evidence, char* room);
void l_hunterExit(char* name, enum LoggerDetails reason);
void l_ghostInit(enum GhostClass type, char* room);
void l_ghostMove(char* room);
void l_ghostEvidence(enum EvidenceType evidence, char* room);
void l_ghostExit(enum LoggerDetails reason);
//new declarations
void hunterInit(char*, enum EvidenceType, HunterType*, EvidenceType*); 
RoomType* createRoom(char*);
void connectRooms(RoomType*, RoomType*);
void addRoom(RoomListType**, RoomType*);
int appendRoom(RoomListType*, RoomType*); 
void populateRooms(HouseType*);
void initHouse(HouseType*, EvidenceType*);
void init_ghost(GhostType*, RoomListType*);
void* ghostUpdate(void*);
void* hunterUpdate(void*);
void ghostMove(GhostType*);
void ghostEvidence(GhostType*);
void hunterCollect(HunterType*);
void hunterReview(HunterType*);
void hunterMove(HunterType*);
void initSemaphores(RoomListType*);
void destroySemaphores(sem_t*);
void checkGhost(HunterType*);
void finalResults(HunterArrayType*, GhostType*);

// classes
struct Room {
  char    name[MAX_STR];
  GhostType* ghost;
  RoomListType* connectedTo;
  EvidenceListType* evidence;
  HunterArrayType* hunters;
  sem_t  mutex;
};

struct EvidenceList {
  EvidenceNodeType* head;
  int size;
};

struct EvidenceNode {
  EvidenceType data;
  struct EvidenceNode* next;
};

struct RoomList {
  RoomNodeType* head;
  RoomNodeType* tail;
  int size;
};

struct RoomNode {
  RoomType* data;
  struct RoomNode* next;
};

struct Ghost {
  EvidenceType evidence[MAX_EVIDENCE];
  GhostClass ghostType;
  struct Room   *room;
  int boredomLevel;
};

struct Hunter {
  int     id;
  EvidenceType equipment;
  char    name[MAX_STR];
  int     fearLevel;
  int     boredLevel;
  EvidenceType* evidenceLog;
  sem_t   mutex;
  RoomType* room;
};

struct HunterArray {
  HunterType* hunterList[HUNTERS];
} ;

struct House {
  RoomListType* rooms;
  HunterArrayType* hunters;
  EvidenceType* evidenceLog;
} ;

