#include "defs.h"
#include <pthread.h>

int main()
{
    printf("line 1");
    EvidenceType sharedEvidence[MAX_EVIDENCE];
    for(int i = 0; i < MAX_EVIDENCE; i++){
        sharedEvidence[i] = EV_UNKNOWN;
    }
    HouseType house;
    // printf("line 1");
    initHouse(&house, sharedEvidence);
    // printf("line 2");
    populateRooms(&house);
    // NodeType* temp = house.rooms->head;
    // printf("Rooms:\n");
    // printf("%s\n", temp->data->name);
    // while(temp != NULL){
    //     printf("%s\n", temp->data->name);
    //     temp = temp->next;
    // }
    HunterArrayType* hunters = (struct HunterArray*) malloc(sizeof(HunterType)*HUNTERS);
    for(int i = 0; i < 4; i++){
        char input[MAX_STR]; // Allocate enough memory for the input string
        HunterType* hunter = (struct Hunter*) malloc(sizeof(HunterType));
        printf("Please enter Hunter %d: ", i+1);
        scanf("%s", input);
        hunterInit(input, i, hunter, sharedEvidence);
        hunter->id = i;
        l_hunterInit(input, i);
        // printf("line 16");
        hunters->hunterList[i] = hunter;
        // printf("line 17");
        hunter->room = house.rooms->head->data;
        // l_hunterMove(hunter->name, hunter->room->name);
        printf("Hunter %s is in %s and is ready to hunt\n", hunter->name, hunter->room->name);
    }
    srand(time(NULL));
    GhostType ghost;  
    // init_ghost(&ghost, house.rooms);
    // Define a semaphore for each room
    // sem_t roomSemaphore[ROOMS];
    // initSemaphores(roomSemaphore);
    
    printf("thread creation");
    // sleep(5);
    // pthread_t ghostThread;
    // pthread_create(&ghostThread, NULL, ghostUpdate, &ghost);

    
    // pthread_t hunterThreads[4];
    // for(int i = 0; i<4; i++){
    //     pthread_create(&hunterThreads[i], NULL, hunterUpdate, &hunters->hunterList[i]);
    // }  
    
    // for (int i = 0; i < 4; i++) {
    //     pthread_join(hunterThreads[i], NULL);
    // }
    // pthread_join(ghostThread, NULL);
    //might not work


    return 0;
}

void *hunterUpdate(void* args){
    // ThreadDataType* threadData = (ThreadDataType*)data;

    // HunterArrayType* hunters = threadData->hunters;
    // GhostType* ghost = threadData->ghost;
    HunterType* hunter = args;   
    // HouseType* house = threadData->house;
    int alive = C_TRUE;
    usleep(HUNTER_WAIT);
    while(alive){
        if(hunter->room->ghost != NULL){
            hunter->boredLevel = 0;
            hunter->fearLevel++;
        }
        else{
            hunter->boredLevel++;
        }
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
            pthread_exit(NULL);
        }

        if(hunter->fearLevel == FEAR_MAX){
            l_hunterExit(hunter->name, LOG_FEAR);
            pthread_exit(NULL);
        }
    }
 
    return NULL;


}

void *ghostUpdate(void* args){
    int haunting = C_TRUE;
    int found = C_FALSE;
    GhostType* ghost = (GhostType*) args;
    // GhostType* ghost = ((ThreadDataType*)data)->ghost;
    //HunterArrayType* hunters = ((ThreadDataType*)data)->hunters;
    while(haunting){
        usleep(GHOST_WAIT);
        for(int i = 0; i<4; i++){
            if (ghost->room == ghost->room->hunters->hunterList[i]->room) {
                ghost->boredomLevel = 0;
                found = C_TRUE;
                break;
            }
        }
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


