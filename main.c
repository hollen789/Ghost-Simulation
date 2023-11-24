#include "defs.h"
int main()
{
    HunterArrayType* hunters = (struct HunterArray*) malloc(sizeof(HunterType)*HUNTERS);
    char* input = "";
    for(int i = 0; i < 4; i++){
        HunterType* hunter = (struct Hunter*) malloc(sizeof(HunterType));
        printf("Please enter Hunter %d: ", i+1);
        scanf("%s", input);
        hunterInit(input, i, hunter);
        l_hunterInit(input, i);
        hunters->hunterList[i] = hunter;
    }
    srand(time(NULL));

    // Create the house: You may change this, but it's here for demonstration purposes
    // Note: This code will not compile until you have implemented the house functions and structures
    HouseType house;
    initHouse(&house);
    populateRooms(&house);

    return 0;
}

