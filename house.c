#include "defs.h"

/*
    Dynamically allocates several rooms and populates the provided house.
    Note: You may modify this as long as room names and connections are maintained.
        out: house - the house to populate with rooms. Assumes house has been initialized.
*/
void populateRooms(HouseType* house) {
    // First, create each room

    // createRoom assumes that we dynamically allocate a room, initializes the values, and returns a RoomType*
    // create functions are pretty typical, but it means errors are harder to return aside from NULL
    struct Room* van                = createRoom("Van");
    struct Room* hallway            = createRoom("Hallway");
    struct Room* master_bedroom     = createRoom("Master Bedroom");
    struct Room* boys_bedroom       = createRoom("Boy's Bedroom");
    struct Room* bathroom           = createRoom("Bathroom");
    struct Room* basement           = createRoom("Basement");
    struct Room* basement_hallway   = createRoom("Basement Hallway");
    struct Room* right_storage_room = createRoom("Right Storage Room");
    struct Room* left_storage_room  = createRoom("Left Storage Room");
    struct Room* kitchen            = createRoom("Kitchen");
    struct Room* living_room        = createRoom("Living Room");
    struct Room* garage             = createRoom("Garage");
    struct Room* utility_room       = createRoom("Utility Room");

    // This adds each room to each other's room lists
    // All rooms are two-way connections
    connectRooms(van, hallway);
    connectRooms(hallway, master_bedroom);
    connectRooms(hallway, boys_bedroom);
    connectRooms(hallway, bathroom);
    connectRooms(hallway, kitchen);
    connectRooms(hallway, basement);
    connectRooms(basement, basement_hallway);
    connectRooms(basement_hallway, right_storage_room);
    connectRooms(basement_hallway, left_storage_room);
    connectRooms(kitchen, living_room);
    connectRooms(kitchen, garage);
    connectRooms(garage, utility_room);

    // Add each room to the house's room list
    addRoom(&house->rooms, van);
    addRoom(&house->rooms, hallway);
    addRoom(&house->rooms, master_bedroom);
    addRoom(&house->rooms, boys_bedroom);
    addRoom(&house->rooms, bathroom);
    addRoom(&house->rooms, basement);
    addRoom(&house->rooms, basement_hallway);
    addRoom(&house->rooms, right_storage_room);
    addRoom(&house->rooms, left_storage_room);
    addRoom(&house->rooms, kitchen);
    addRoom(&house->rooms, living_room);
    addRoom(&house->rooms, garage);
    addRoom(&house->rooms, utility_room);
}
/*
    creates room
        in:   name of room to be initialized
*/
RoomType* createRoom(char *name){
    RoomType* room = malloc(sizeof(RoomType));
    strcpy(room->name, name); 
    room->ghost = NULL;
    room->connectedTo = malloc(sizeof(RoomListType));
    room->connectedTo->head = NULL;
    room->connectedTo->tail = NULL;
    room->evidence = malloc(sizeof(EvidenceListType)); //not being freed
    room->evidence->head = NULL;
    room->evidence->size = 0;

    room->hunters = malloc(sizeof(HunterArrayType));
    for(int i = 0;i<HUNTERS;i++){
        room->hunters->hunterList[i] = NULL;
    }
    return room;
}
/*
    connects to rooms to one another
        2 in/outs:   rooms that will be connected to one another
*/
void connectRooms(RoomType* room1, RoomType* room2){
    addRoom(&room1->connectedTo, room2);
    addRoom(&room2->connectedTo, room1);
}
/*
    adds rooms to a room list
        in/out:   roomList - room list to be added
        in:  room - room to be added to list
*/
void addRoom(RoomListType** roomList, RoomType* room){
    RoomNodeType* newNode = (struct RoomNode*) malloc(sizeof(RoomNodeType)); //not being freed?
    newNode->data = room;
    newNode->next = NULL;

    if((*roomList)->head == NULL) {
      (*roomList)->head = newNode;
    } else {
      RoomNodeType* currentNode = (*roomList)->head;
      while(currentNode->next != NULL) {
        currentNode = currentNode->next;
      }
      currentNode->next = newNode;
    }
    (*roomList)->size++;
}
/*
    initalize house values
        in/out:  house - house to be initialized
        in: evidence - evidenceLog to be placed in house's evidence's log
*/
void initHouse(HouseType* house,EvidenceType* evidence){
    house->evidenceLog = evidence;
    house->rooms = malloc(sizeof(RoomListType));
    house->rooms->head = NULL;
    house->rooms->tail = NULL;
    house->hunters = malloc(sizeof(HunterArrayType));
    for(int i = 0;i<HUNTERS;i++){
        house->hunters->hunterList[i] = NULL;
    }
}
