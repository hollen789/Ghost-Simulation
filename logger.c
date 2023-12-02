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
        in: name - the hunter name to log
        in: equipment - the hunter's equipment
        in/out: hunter - the hunter that will be initilized
        in: evidence - the evidenceLog that will be shared among other hunters and the house
*/
void hunterInit(char* name, enum EvidenceType equipment, HunterType* hunter, EvidenceType* evidence) {
    strcpy(hunter->name , name);
    hunter->equipment = equipment;
    hunter->fearLevel = 0;
    hunter->boredLevel = 0;
    hunter->evidenceLog = evidence;
    sem_init(&hunter->mutex, 0, 1);
    //hunter->room = house.rooms->head->data;
}

/* 
    inits ghost values
        in/out: ghost - the ghost to be initialized
        in: rooms - the list of rooms that will contain one of the randomly chosen room for the ghost
*/
void init_ghost(GhostType* ghost, RoomListType* rooms){
    //printf("entering init ghost");
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
            printf("[BAD LUCK]\n");
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
/*
    Moves the ghost to a random adjacent room
        in/out: ghost - will change it's room attribute
*/
void ghostMove(GhostType* ghost){
    int choice = randInt(0, ghost->room->connectedTo->size);
    RoomNodeType* temp = ghost->room->connectedTo->head;
    for(int i=0; i<choice; i++){
        temp = temp->next;
    }
    sem_wait(&ghost->room->mutex);
    ghost->room->ghost = NULL;
    sem_post(&ghost->room->mutex);
    ghost->room = temp->data;
    sem_wait(&ghost->room->mutex);
    ghost->room->ghost = ghost;
    sem_post(&ghost->room->mutex);
    l_ghostMove(ghost->room->name);
}
/*
    makes the ghost leave one of it's random evidences in its current room
        in: ghost - the ghost that will leave one of it's evidence randomly 
*/
void printEvidenceInRoom(RoomType* room){
    EvidenceNodeType* temp = room->evidence->head;
    printf("CHECK 2: room %s evidence %ld:\n", room->name, (long)(room->evidence->head));
    while(temp != NULL){
        printf("%d, ", temp->data);
        temp = temp->next;
    }
}
/*d
    makes the ghost leave one of it's random evidences in its current room
        in: ghost - the ghost that will leave one of it's evidence randomly 
*/
void ghostEvidence(GhostType* ghost){
    int choice = randInt(0, 3);
    EvidenceNodeType* newNode = (struct EvidenceNode*) malloc(sizeof(EvidenceNodeType));
    sem_wait(&ghost->room->mutex);
    EvidenceListType* evidenceList = ghost->room->evidence;
    newNode->data = ghost->evidence[choice];
    newNode->next = NULL;
    if(evidenceList->head == NULL) {
        evidenceList->head = newNode;
    } else {
      EvidenceNodeType* currentNode = evidenceList->head;
      while(currentNode->next != NULL) {
        currentNode = currentNode->next;
      }
      currentNode->next = newNode;
    }
    evidenceList->size++;
    sem_post(&ghost->room->mutex);
    l_ghostEvidence(ghost->evidence[choice], ghost->room->name);
}
/*
    makes hunter check if the current room contains an evidence that can be collected by said hunter, if so remove it and place it in their evidenceLog
        in: hunter - the hunter that will attempt to collect evidence
*/
void hunterCollect(HunterType* hunter) {

    EvidenceType canCollect = hunter->equipment;
    int alreadyCollected = C_FALSE;
    int somethingToCollect = C_FALSE;
    for (int i = 0; i < MAX_EVIDENCE; i++) {
        if (hunter->evidenceLog[i] == canCollect) {
            alreadyCollected = C_TRUE;
            break;
        }
    } 
    sem_wait(&hunter->room->mutex);
    EvidenceNodeType* temp = hunter->room->evidence->head;
    EvidenceNodeType* prev = temp;
    while (temp != NULL) {
        if (temp->data == canCollect) {
            somethingToCollect=C_TRUE;
            // Remove node from list
            if ( temp == hunter->room->evidence->head) {
                hunter->room->evidence->head = temp->next;
            }else{
                prev->next = temp->next;
            }
            free(temp);
            temp = NULL;
            hunter->room->evidence->size--;
            break;
        }
        prev = temp;
        temp = temp->next;
    }
    sem_post(&hunter->room->mutex);
    sem_wait(&hunter->mutex);
    if (alreadyCollected == C_FALSE && somethingToCollect == C_TRUE) {
        int i = 0;
        while (hunter->evidenceLog[i] != EV_UNKNOWN && i < MAX_EVIDENCE) {
            i++;
        }
        hunter->evidenceLog[i] = canCollect;
        l_hunterCollect(hunter->name, hunter->equipment, hunter->room->name);
    }
    sem_post(&hunter->mutex);
    if(somethingToCollect == C_FALSE){
        printf("Hunter %s found nothing to collect\n",hunter->name);
    }
    else{
        printf("Hunter %s already collected this evidence(%d)\n",hunter->name, canCollect);
    }

}
/*
    makes hunter check if there is sufficient evidence in evidenceLog so the ghost can be identified
        in: hunter - the hunter that will review collected evidences
        out: potentially confirmation that sufficient evidence has been collected
*/
void hunterReview(HunterType* hunter){
    if(hunter->evidenceLog[MAX_EVIDENCE-1]!=EV_UNKNOWN){
        l_hunterReview(hunter->name, LOG_SUFFICIENT);
        // ghostToString(ghost->ghostType,name);
        // printf("Ghost has been found. It was a %s\n",name);
         sem_wait(&hunter->room->mutex);
         hunter->room->hunters->hunterList[hunter->id] = NULL;
         sem_post(&hunter->room->mutex);
         pthread_exit(NULL);
    }
    else{
        l_hunterReview(hunter->name, LOG_INSUFFICIENT);
    }
}
/*
    makes hunter move to an adjacent room
        in: hunter - the hunter that will change it's room attribute
*/
void hunterMove(HunterType* hunter){
    int choice = randInt(0, hunter->room->connectedTo->size);
    RoomNodeType* temp = hunter->room->connectedTo->head;
    for(int i=0; i<choice; i++){
        temp = temp->next;
    }
    sem_wait(&hunter->room->mutex);
    hunter->room->hunters->hunterList[hunter->id] = NULL;
    sem_post(&hunter->room->mutex);
    hunter->room = temp->data;
    sem_wait(&temp->data->mutex);
    hunter->room->hunters->hunterList[hunter->id] = hunter;
    sem_post(&temp->data->mutex);
    
    l_hunterMove(hunter->name, hunter->room->name);
}

