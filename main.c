#include "defs.h"
#include <pthread.h>

int main()
{
    HouseType house;
    // printf("line 1");
    initHouse(&house);
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
        hunterInit(input, i, hunter);
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
    init_ghost(&ghost, house.rooms);
    // printf("Ghost is in %s\n", ghost.room->name);
    // for(int i=0; i<4; i++){
    //     printf("%d\n", ghost.evidence[i]);
    // }
    // char ghostType[50];
    // ghostToString(ghost.ghostType, ghostType);
    // printf("%s", ghostType);
    // Create the house: You may change this, but it's here for demonstration purposes
    // Note: This code will not compile until you have implemented the house functions and structures
//this all needs to go before the Hunters are created so the hunters can be placed in the van room
    pthread_t ghostThread;

    ThreadDataType data;
    data.ghost = &ghost;
    data.hunters = hunters;
    data.house = &house;
    // data->rooms = house.rooms;

    pthread_create(&ghostThread, NULL, ghostUpdate, &data);

    pthread_t hunterThreads[4];
    for(int i = 0; i<4; i++){
        data.hunter = hunters->hunterList[i];
        pthread_create(&hunterThreads[i], NULL, hunterUpdate, &data);
    }
   
    for (int i = 0; i < 4; i++) {
        pthread_join(hunterThreads[i], NULL);
    }
    pthread_join(ghostThread, NULL);
    //might not work


    return 0;
}

void *hunterUpdate(void* data){
    ThreadDataType* threadData = (ThreadDataType*)data;
    // HunterArrayType* hunters = threadData->hunters;
    GhostType* ghost = threadData->ghost;
    HunterType* hunter = threadData->hunter;   
    HouseType* house = threadData->house;
    int alive = C_TRUE;
    while(alive){
        if(hunter->room == ghost->room){
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
                hunterCollect(hunter, house);
                break;
            case 1:
                hunterMove(hunter);
                break;
            default:
                hunterReview(house,hunter);
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

void *ghostUpdate(void* data){
    int haunting = C_TRUE;
    int found = C_FALSE;
    GhostType* ghost = ((ThreadDataType*)data)->ghost;
    HunterArrayType* hunters = ((ThreadDataType*)data)->hunters;
    while(haunting){
        for(int i = 0; i<4; i++){
            if (ghost->room == hunters->hunterList[i]->room) {
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


