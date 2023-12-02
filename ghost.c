#include "defs.h"
/* 
    inits ghost values
        in/out: ghost - the ghost to be initialized
        in: rooms - the list of rooms that will contain one of the randomly chosen room for the ghost
*/
void init_ghost(GhostType* ghost, RoomListType* rooms){
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
        default://WRONG CASE
            ghost->evidence[0] = EV_UNKNOWN;
            ghost->evidence[1] = EV_UNKNOWN;
            ghost->evidence[2] = EV_UNKNOWN;
            break;
    }
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