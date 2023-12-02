#include "defs.h"

/* 
    Logs the hunter being created.
        in: hunter - the hunter name to log
        in: equipment - the hunter's equipment
*/
void l_hunterInit(char* hunter, enum EvidenceType equipment) {
    if (!LOGGING) return;
    char ev_str[MAX_STR];
    evidenceToString(equipment, ev_str);
    // printf(ev_str, MAX_STR);
    printf("[HUNTER INIT] [%s] is a [%s] hunter\n", hunter, ev_str); 
}
/*
    Logs the hunter moving into a new room.
        in: hunter - the hunter name to log
        in: room - the room name to log
*/
void l_hunterMove(char* hunter, char* room) {
    if (!LOGGING) return;
    printf("%-18s| [%s]%s[%s]\n","[HUNTER MOVE]", hunter, " has moved into ", room);
}

/*
    Logs the hunter exiting the house.
        in: hunter - the hunter name to log
        in: reason - the reason for exiting, either LOG_FEAR, LOG_BORED, or LOG_EVIDENCE
*/
void l_hunterExit(char* hunter, enum LoggerDetails reason) {
    if (!LOGGING) return;
    printf("%-18s| [%s]%s","[HUNTER EXIT]", hunter, " exited because ");
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
    printf("%-18s| [%s]%s","[HUNTER REVIEW]", hunter," reviewed evidence and found ");
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
    printf("%-18s| [%s]%s[%s]%s[%s]%s","[HUNTER EVIDENCE]", hunter," found ",ev_str," in ", room, " and [COLLECTED]\n");
}

/*
    Logs the ghost moving into a new room.
        in: room - the room name to log
*/
void l_ghostMove(char* room) {
    if (!LOGGING) return;
    printf("%-18s|%s[%s]\n","[GHOST MOVE]", " Ghost has moved into ",room);
}

/*
    Logs the ghost exiting the house.
        in: reason - the reason for exiting, either LOG_FEAR, LOG_BORED, or LOG_EVIDENCE
*/
void l_ghostExit(enum LoggerDetails reason) {
    if (!LOGGING) return;
    printf("%-18s|%s","[GHOST EXIT]", " Exited because ");
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
    printf("%-18s|%s[%s]%s[%s]\n","[GHOST EVIDENCE]"," Ghost left ",ev_str," in ", room);
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
    printf("%-18s|%s[%s]%s[%s]\n","[GHOST INIT]", "Ghost is a ", ghost_str," in room ",room);
}

void l_hunterSwitch(char* hunter, EvidenceType equipment){
    if (!LOGGING) return;
    char c[MAX_STR];
    evidenceToString(equipment, c);
    printf("%-18s| [%s]%s[%s]\n","[HUNTER SWITCH]", hunter," has switched their equipment to ", c);
}

/*
    Logs the ghost being created.
        in: ghost - the ghost type to log
        in: room - the room name that the ghost is starting in
*/
void finalResults(HunterArrayType* hunters, GhostType* ghost){
    printf("\n\n--------------------------------------------------------------\n");
    printf("FINAL RESULTS:\n");
    printf("--------------------------------------------------------------\n");

    int count = 0;
    int anyAfraid= C_FALSE;
    int anyBored = C_FALSE;
    printf("List of hunters that fell victim to the ghost and ran in fear:\n\n");
    for(int i=0; i<HUNTERS; i++){
       if(hunters->hunterList[i] != NULL){
            if(hunters->hunterList[i]->fearLevel >=FEAR_MAX){
                anyAfraid = C_TRUE;
                count++;
                printf("Hunter %s has fled\n", hunters->hunterList[i]->name);
            }
       }
    }
    if(anyAfraid == C_FALSE){
        printf("No hunters have fled\n");
    }
    printf("--------------------------------------------------------------\n");
    printf("List of hunters that got bored and left the house:\n\n");
    for(int i=0; i<HUNTERS; i++){
       if(hunters->hunterList[i] != NULL){
            if(hunters->hunterList[i]->boredLevel >=BOREDOM_MAX){
                anyBored = C_TRUE;
                count++;
                printf("Hunter %s has grown tired of this game\n", hunters->hunterList[i]->name);
            }
       }
    }
    if(anyBored == C_FALSE){
        printf("No hunters got bored\n");
    }
    printf("--------------------------------------------------------------\n");
    if(count == 4){
        printf("The ghost has won. All hunters have left the house\n");
    }
    else{
        printf("The ghost has lost and the hunters have won!\nThe hunters have collected enough evidence to identify the ghost\n");
        printf("The hunters have collected the following evidence:\n");
        for(int i=0; i<MAX_EVIDENCE; i++){
            if(hunters->hunterList[0]->evidenceLog[i] != EV_UNKNOWN){
                char ev_str[MAX_STR];
                evidenceToString(hunters->hunterList[0]->evidenceLog[i], ev_str);
                printf("%s\n", ev_str);
            }
        }
        char name[MAX_STR];
        ghostToString(ghost->ghostType,name);
        printf("Ghost has been found. It was a %s\n",name);
    }
    
    for(int i=0; i<HUNTERS; i++){
        if(hunters->hunterList[i] != NULL){
            sem_destroy(&hunters->hunterList[i]->mutex);
            free(hunters->hunterList[i]);
        }
    }
}
