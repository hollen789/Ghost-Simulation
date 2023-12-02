#include "defs.h"
/* 
    inits hunter values
        in: name - the hunter name to log
        in: equipment - the hunter's equipment
        in/out: hunter - the hunter that will be initilized
        in: evidence - the evidenceLog that will be shared among other hunters and the house
*/
void hunterInit(char* name, EvidenceType equipment, HunterType* hunter, EvidenceType* evidence) {
    strcpy(hunter->name , name);
    if(equipment >= 0 && equipment < 4){
        hunter->equipment = equipment;
    }
    else{
        hunter->equipment = randInt(0, 4);
    }
    hunter->fearLevel = 0;
    hunter->boredLevel = 0;
    hunter->evidenceLog = evidence;
    sem_init(&hunter->mutex, 0, 1);
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