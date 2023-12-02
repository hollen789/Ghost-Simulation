#include "defs.h"
#include <pthread.h>

int main()
{
    //INTIALIZING ALL OBJECTS
    EvidenceType sharedEvidence[MAX_EVIDENCE];
    for(int i = 0; i < MAX_EVIDENCE; i++){
        sharedEvidence[i] = EV_UNKNOWN;
    }
    HouseType house;
    initHouse(&house, sharedEvidence);
    populateRooms(&house);
    HunterArrayType* hunters = (struct HunterArray*) malloc(sizeof(HunterType)*HUNTERS);
    for(int i = 0; i < HUNTERS; i++){
        char input[MAX_STR]; // Allocate enough memory for the input string
        int equipmentInput = EV_UNKNOWN;
        HunterType* hunter = (struct Hunter*) malloc(sizeof(HunterType));
        printf("Please enter Hunter %d name: ", i+1);
        scanf("%s", input);
        printf("Please enter their equipment[EMF(0), TEMPERATURE(1), FINGERPRINTS(2), SOUND(3), RANDOM(OTHER)]: ");
        scanf("%d", &equipmentInput);
        while(equipmentInput != "\n");
        hunterInit(input, equipmentInput, hunter, sharedEvidence);
        hunter->id = i;
        l_hunterInit(input, hunter->equipment);
        hunters->hunterList[i] = hunter;
        hunter->room = house.rooms->head->data;
        printf("Hunter %s is in %s and is ready to hunt\n", hunter->name, hunter->room->name);
    }
    RoomNodeType* temp = house.rooms->head->data->connectedTo->head;

    for(int i = 0; i < house.rooms->head->data->connectedTo->size; i++){
        printf("%s\n", temp->data->name);
        temp = temp->next;
    }
    //RUNNING SIMULATION
    srand(time(NULL));
    GhostType ghost;  
    init_ghost(&ghost, house.rooms);

    // Define a semaphore for each room
    initSemaphores(house.rooms);
    pthread_t ghostThread;
    pthread_create(&ghostThread, NULL, ghostUpdate, &ghost);
    pthread_t hunterThreads[HUNTERS];
    for(int i = 0; i<HUNTERS; i++){
        pthread_create(&hunterThreads[i], NULL, hunterUpdate, hunters->hunterList[i]);
    }  
    pthread_join(ghostThread, NULL);
    for (int i = 0; i < HUNTERS; i++) {
        pthread_join(hunterThreads[i], NULL);
    }
    //FINALIZE PROCESS
    finalResults(hunters, &ghost);
    destroyRoomSemaphores(house.rooms);
    cleanUpHouse(house);
    free(hunters);
    return 0;
}
/* 
    function to be run by hunter threads dictating the actions of each individual hunter.
        in: args - the hunter that will be simulated
*/
void *hunterUpdate(void* args){
    HunterType* hunter = args;   
    int alive = C_TRUE;
    while(alive){
        usleep(HUNTER_WAIT);
        checkGhost(hunter);
        int choice = randInt(0, 3);
        switch (choice)
        {
            case 0:
                hunterCollect(hunter);
                break;
            case 1:
                hunterMove(hunter);
                break;
            default:
                hunterReview(hunter);
                break;
        }
        
        if(hunter->boredLevel == BOREDOM_MAX){
            l_hunterExit(hunter->name, LOG_BORED);
            sem_wait(&hunter->room->mutex);
            hunter->room->hunters->hunterList[hunter->id] = NULL;
            sem_post(&hunter->room->mutex);
            pthread_exit(NULL);
        }

        if(hunter->fearLevel == FEAR_MAX){
            l_hunterExit(hunter->name, LOG_FEAR);
            sem_wait(&hunter->room->mutex);
            hunter->room->hunters->hunterList[hunter->id] = NULL;
            sem_post(&hunter->room->mutex);
            pthread_exit(NULL);
        }
    }
    
    return NULL;
}
/* 
    function to be run by the ghost thread dictating the actions of the ghost.
        in: args - the ghost that will be simulated
*/
void *ghostUpdate(void* args){
    int haunting = C_TRUE;
    GhostType* ghost = (GhostType*) args;
    while(haunting){
        int found = C_FALSE;
        usleep(GHOST_WAIT);
        sem_wait(&ghost->room->mutex);
        for(int i = 0; i<HUNTERS; i++){
            if (ghost->room->hunters->hunterList[i] != NULL) {
                ghost->boredomLevel = 0;
                found = C_TRUE;
                break;
            }
        }
        sem_post(&ghost->room->mutex);
        if(found == C_FALSE){
            ghost->boredomLevel++;
            if(ghost->boredomLevel == BOREDOM_MAX){
                l_ghostExit(LOG_BORED);
                pthread_exit(NULL);
            }
        }
        int choice = randInt(0, 3);
        switch (choice)
        {
        case 0:
            ghostMove(ghost);
            break;
        case 1:
            ghostEvidence(ghost);
            break;
        
        default:
            printf("Ghost is waiting\n");
            break;
        }
    }
    return NULL;
      
}
