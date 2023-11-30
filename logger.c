#include "defs.h"

/* 
    Logs the hunter being created.
    in: hunter - the hunter name to log
    in: equipment - the hunter's equipment
*/
void l_hunterInit(char* hunter, enum EvidenceType equipment) {
    if (!LOGGING) return;
    char ev_str[MAX_STR];
    //printf("test");
    evidenceToString(equipment, ev_str);
    printf(ev_str, MAX_STR);
    printf("[HUNTER INIT] [%s] is a [%s] hunter\n", hunter, ev_str); 
}

/* 
    inits hunter values
    in: hunter - the hunter name to log
    in: equipment - the hunter's equipment
*/
void hunterInit(char* name, enum EvidenceType equipment, HunterType* hunter, EvidenceType* evidence) {
    strcpy(hunter->name , name);
    hunter->equipment = equipment;
    hunter->fearLevel = 0;
    hunter->boredLevel = 0;
    (*hunter->evidence) = evidence;
    sem_init(&hunter->mutex, 0, 1);
    //hunter->room = house.rooms->head->data;
}

void init_ghost(GhostType* ghost, RoomListType* rooms){
    printf("entering init ghost");
    ghost->ghostType = randomGhost();
    RoomNodeType* temp = rooms->head;
    ghost->boredomLevel = 0;
    int randomIndex = randInt(1, rooms->size);
    for (int i = 0; i < randomIndex && temp != NULL; i++) {
        temp = temp->next;  
    }
    ghost->room = temp->data;
    ghost->room->ghost = ghost;

    switch(ghost->ghostType){
        case POLTERGEIST:
            ghost->evidence[0] = EMF;
            ghost->evidence[1] = TEMPERATURE;
            ghost->evidence[2] = FINGERPRINTS;
            break;
        case BANSHEE:
            ghost->evidence[0] = EMF;
            ghost->evidence[1] = TEMPERATURE;
            ghost->evidence[2] = SOUND;
            break;
        case BULLIES:
            ghost->evidence[0] = EMF;
            ghost->evidence[1] = FINGERPRINTS;
            ghost->evidence[2] = SOUND;
            break;
        case PHANTOM:
            ghost->evidence[0] = TEMPERATURE;
            ghost->evidence[1] = FINGERPRINTS;
            ghost->evidence[2] = SOUND;
            break;
        default:
            ghost->evidence[0] = EV_UNKNOWN;
            ghost->evidence[1] = EV_UNKNOWN;
            ghost->evidence[2] = EV_UNKNOWN;
            break;
    }
}

/*
    Logs the hunter moving into a new room.
    in: hunter - the hunter name to log
    in: room - the room name to log
*/
void l_hunterMove(char* hunter, char* room) {
    if (!LOGGING) return;
    printf("[HUNTER MOVE] [%s] has moved into [%s]\n", hunter, room);
}

/*
    Logs the hunter exiting the house.
    in: hunter - the hunter name to log
    in: reason - the reason for exiting, either LOG_FEAR, LOG_BORED, or LOG_EVIDENCE
*/
void l_hunterExit(char* hunter, enum LoggerDetails reason) {
    if (!LOGGING) return;
    printf("[HUNTER EXIT] [%s] exited because ", hunter);
    switch (reason) {
        case LOG_FEAR:
            printf("[FEAR]\n");
            break;
        case LOG_BORED:
            printf("[BORED]\n");
            break;
        case LOG_EVIDENCE:
            printf("[EVIDENCE]\n");
            break;
        default:
            printf("[UNKNOWN]\n");
    }
}

/*
    Logs the hunter reviewing evidence.
    in: hunter - the hunter name to log
    in: result - the result of the review, either LOG_SUFFICIENT or LOG_INSUFFICIENT
*/
void l_hunterReview(char* hunter, enum LoggerDetails result) {
    if (!LOGGING) return;
    printf("[HUNTER REVIEW] [%s] reviewed evidence and found ", hunter);
    switch (result) {
        case LOG_SUFFICIENT:
            printf("[SUFFICIENT]\n");
            break;
        case LOG_INSUFFICIENT:
            printf("[INSUFFICIENT]\n");
            break;
        default:
            printf("[UNKNOWN]\n");
    }
}

/*
    Logs the hunter collecting evidence.
    in: hunter - the hunter name to log
    in: evidence - the evidence type to log
    in: room - the room name to log
*/
void l_hunterCollect(char* hunter, enum EvidenceType evidence, char* room) {
    if (!LOGGING) return;
    char ev_str[MAX_STR];
    evidenceToString(evidence, ev_str);
    printf("[HUNTER EVIDENCE] [%s] found [%s] in [%s] and [COLLECTED]\n", hunter, ev_str, room);
}

/*
    Logs the ghost moving into a new room.
    in: room - the room name to log
*/
void l_ghostMove(char* room) {
    if (!LOGGING) return;
    printf("[GHOST MOVE] Ghost has moved into [%s]\n", room);
}

/*
    Logs the ghost exiting the house.
    in: reason - the reason for exiting, either LOG_FEAR, LOG_BORED, or LOG_EVIDENCE
*/
void l_ghostExit(enum LoggerDetails reason) {
    if (!LOGGING) return;
    printf("[GHOST EXIT] Exited because ");
    switch (reason) {
        case LOG_FEAR:
            printf("[FEAR]\n");
            break;
        case LOG_BORED:
            printf("[BORED]\n");
            break;
        case LOG_EVIDENCE:
            printf("[EVIDENCE]\n");
            break;
        default:
            printf("[UNKNOWN]\n");
    }
}

/*
    Logs the ghost leaving evidence in a room.
    in: evidence - the evidence type to log
    in: room - the room name to log
*/
void l_ghostEvidence(enum EvidenceType evidence, char* room) {
    if (!LOGGING) return;
    char ev_str[MAX_STR];
    evidenceToString(evidence, ev_str);
    printf("[GHOST EVIDENCE] Ghost left [%s] in [%s]\n", ev_str, room);
}

/*
    Logs the ghost being created.
    in: ghost - the ghost type to log
    in: room - the room name that the ghost is starting in
*/
void l_ghostInit(enum GhostClass ghost, char* room) {
    if (!LOGGING) return;
    char ghost_str[MAX_STR];
    ghostToString(ghost, ghost_str);
    printf("[GHOST INIT] Ghost is a [%s] in room [%s]\n", ghost_str, room);
}

void ghostMove(GhostType* ghost){
    int choice = randInt(0, ghost->room->connectedTo->size);
    RoomNodeType* temp = ghost->room->connectedTo->head;
    for(int i=0; i<choice; i++){
        temp = temp->next;
    }
    // sem_wait(&ghost->room->mutex);
    // sem_wait(&temp->data->mutex);
    ghost->room->ghost = NULL;
    ghost->room = temp->data;
    ghost->room->ghost = ghost;
    // sem_post(&temp->data->mutex);
    // sem_post(&ghost->room->mutex);

    l_ghostMove(ghost->room->name);
}

void ghostEvidence(GhostType* ghost){
    int choice = randInt(0, 3);
    EvidenceNodeType* newNode = (struct EvidenceNode*) malloc(sizeof(EvidenceNodeType));
    sem_wait(&(ghost->room->mutex));
    EvidenceListType* evidenceList = ghost->room->evidence;
    newNode->data = ghost->evidence+choice;
    if(evidenceList->head == NULL) {
        evidenceList->head = newNode;
        newNode->prev = NULL;
    } else {
      EvidenceNodeType* currentNode = evidenceList->head;
      while(currentNode->next != NULL) {
        currentNode = currentNode->next;
      }
      newNode->prev = currentNode;
      currentNode->next = newNode;
    }
    evidenceList->size++;
    sem_post(&(ghost->room->mutex));
    l_ghostEvidence(ghost->evidence[choice], ghost->room->name);
}

void hunterCollect(HunterType* hunter) {
    EvidenceType canCollect = hunter->equipment;
    int alreadyCollected = C_FALSE;
    for (int i = 0; i < MAX_EVIDENCE; i++) {
        if (hunter->evidence[i]!=NULL && (*hunter->evidence[i]) == canCollect) {
            alreadyCollected = C_TRUE;
            break;
        }
    } 
    sem_wait(&hunter->room->mutex);
    EvidenceNodeType* temp = hunter->room->evidence->head;
    EvidenceType* evidence = NULL;
    while (temp != NULL) {
        if ((*temp->data) == canCollect) {
            evidence = temp->data;
            // Remove node from list
            if (temp->prev != NULL) {
                temp->prev->next = temp->next;
            }
            if (temp->next != NULL) {
                temp->next->prev = temp->prev;
            }
            if (temp->prev == NULL) {
                hunter->room->evidence->head = temp->next;
            }
            free(temp);
            hunter->room->evidence->size--;
            break;
        }
        temp = temp->next;
    }
    sem_post(&hunter->room->mutex);
    sem_wait(&hunter->mutex);
    if (alreadyCollected == C_FALSE) {
        int i = 0;
        while (hunter->evidence[i] != NULL) {
            i++;
        }
        hunter->evidence[i] = evidence;
    }
    sem_post(&hunter->mutex);

    l_hunterCollect(hunter->name, hunter->equipment, hunter->room->name);
}

void hunterReview(HunterType* hunter){
    if(hunter->evidence[MAX_EVIDENCE-1]!=NULL){
        l_hunterReview(hunter->name, LOG_SUFFICIENT);
        
        // ghostToString(ghost->ghostType,name);
        // printf("Ghost has been found. It was a %s\n",name);
        pthread_exit(NULL);
    }
    else{
        l_hunterReview(hunter->name, LOG_INSUFFICIENT);
    }
}

void hunterMove(HunterType* hunter){
    int choice = randInt(0, hunter->room->connectedTo->size);
    RoomNodeType* temp = hunter->room->connectedTo->head;
    for(int i=0; i<choice; i++){
        temp = temp->next;
    }
    sem_wait(&hunter->mutex);
    hunter->room->hunters->hunterList[hunter->id-1] = NULL;
    hunter->room = temp->data;
    hunter->room->hunters->hunterList[hunter->id-1] = hunter;
    sem_post(&hunter->mutex);
    l_hunterMove(hunter->name, hunter->room->name);
}
