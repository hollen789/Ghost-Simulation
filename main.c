#include "defs.h"
int main()
{
    HouseType house;
    printf("line 1");
    initHouse(&house);
    printf("line 2");
    populateRooms(&house);
    NodeType* temp = house.rooms->head;
    printf("Rooms:\n");
    printf("%s\n", temp->data->name);
    while(temp != NULL){
        printf("%s\n", temp->data->name);
        temp = temp->next;
    }

    HunterArrayType* hunters = (struct HunterArray*) malloc(sizeof(HunterType)*HUNTERS);
    for(int i = 0; i < 4; i++){
        char input[MAX_STR]; // Allocate enough memory for the input string
        HunterType* hunter = (struct Hunter*) malloc(sizeof(HunterType));
        printf("Please enter Hunter %d: ", i+1);
        scanf("%s", input);
        hunterInit(input, i, hunter);
        l_hunterInit(input, i);
        printf("line 16");
        hunters->hunterList[i] = hunter;
        printf("line 17");
        hunter->room = house.rooms->head->data;
        // l_hunterMove(hunter->name, hunter->room->name);
        // printf("Hunter %s is in %s and is ready to hunt\n", hunter->name, hunter->room->name);
    }
    srand(time(NULL));

    // Create the house: You may change this, but it's here for demonstration purposes
    // Note: This code will not compile until you have implemented the house functions and structures
//this all needs to go before the Hunters are created so the hunters can be placed in the van room

    return 0;
}

