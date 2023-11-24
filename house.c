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
        in/out:   name of room 
*/
RoomType* createRoom(char *name){
    RoomType* room = malloc(sizeof(RoomType));
    strcpy(room->name, name); 
    room->ghost = NULL;
    return room;
}
/*
    set roomList in room
        2 in/out:   rooms that will be connected to one another
*/
void connectRooms(RoomType* room1, RoomType* room2){
    appendRoom(room1->connectedTo, room2);
    appendRoom(room2->connectedTo, room1);
}
/*
    adds room to house
        in:         room to be added to house
        in/out:     house that will add said room
*/
void addRoom(RoomListType** roomList, RoomType* room){
    appendRoom(*roomList, room);
}
int appendRoom(RoomListType* roomList, RoomType* room) {
  if(roomList == NULL || room == NULL) {
    return C_FALSE;
  }
    NodeType* newNode = (struct Node*) malloc(sizeof(NodeType));

    newNode->data = room;
    newNode->next = NULL;

    if(roomList->head == NULL) {
      roomList->head = newNode;
    } else {
      NodeType* currentNode = roomList->head;
      while(currentNode->next != NULL) {
        currentNode = currentNode->next;
      }
      currentNode->next = newNode;
    }

    return C_TRUE;
}
/*
    initalize house values
        in:         room to be added to house
        in/out:     house that will add said room
*/
void initHouse(HouseType* house){
    for(int i = 0; i < MAX_EVIDENCE; i++){
        house->evidence[i] = NULL;
    }
    house->rooms = NULL;
    house->hunters = NULL;
}
