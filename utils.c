#include "defs.h"

/*
    Returns a pseudo randomly generated number, in the range min to (max - 1), inclusively
        in:   lower end of the range of the generated number
        in:   upper end of the range of the generated number
    return:   randomly generated integer in the range [0, max-1) 
*/
int randInt(int min, int max)
{
    return (int) randFloat(min, max);
}

/*
    Returns a pseudo randomly generated floating point number.
    A few tricks to make this thread safe, just to reduce any chance of issues using random
        in:   lower end of the range of the generated number
        in:   upper end of the range of the generated number
    return:   randomly generated floating point number in the range [min, max)
*/
float randFloat(float min, float max) {
    static __thread unsigned int seed = 0;
    if (seed == 0) {
        seed = (unsigned int)time(NULL) ^ (unsigned int)pthread_self();
    }

    float random = ((float) rand_r(&seed)) / (float) RAND_MAX;
    float diff = max - min;
    float r = random * diff;
    return min + r;
}

/* 
    Returns a random enum GhostClass.
*/
enum GhostClass randomGhost() {
    return (enum GhostClass) randInt(0, GHOST_COUNT);
}

/*
    Returns the string representation of the given enum EvidenceType.
        in: type - the enum EvidenceType to convert
        out: str - the string representation of the given enum EvidenceType, minimum 16 characters
*/
void evidenceToString(enum EvidenceType type, char* str) {
    switch (type) {
        case EMF:
            strcpy(str, "EMF");
            break;
        case TEMPERATURE:
            strcpy(str, "TEMPERATURE");
            break;
        case FINGERPRINTS:
            strcpy(str, "FINGERPRINTS");
            break;
        case SOUND:
            strcpy(str, "SOUND");
            break;
        default:
            strcpy(str, "UNKNOWN");
            break;
    }
}

/* 
    Returns the string representation of the given enum GhostClass.
        in: ghost - the enum GhostClass to convert
        out: buffer - the string representation of the given enum GhostClass, minimum 16 characters
*/
void ghostToString(enum GhostClass ghost, char* buffer) {
    switch(ghost) {
        case BANSHEE:
            strcpy(buffer, "Banshee");
            break;
        case BULLIES:
            strcpy(buffer, "Bullies");
            break;
        case PHANTOM:
            strcpy(buffer, "Phantom");
            break;
        case POLTERGEIST:
            strcpy(buffer, "Poltergeist");
            break;
        default:
            strcpy(buffer, "Unknown");
            break;
        
    }
}
/*
    helper method to initialize semaphores
        in/out: roomSemaphore - semaphore array that will have its semaphores intitialized
*/
void initSemaphores(RoomListType* roomSemaphore) {
    // Initialize the semaphores for each room
    RoomNodeType* temp = roomSemaphore->head;
    while(temp != NULL) {
        sem_init(&(temp->data->mutex), 0, 1); // Initialize with value 1 (unlocked)
        temp = temp->next;
    }
}

/*
    helper method for hunters to check if they are in the same room as ghost
        in/out: hunter - will change it's boredom/fear level depending on existence of ghost
*/
void checkGhost(HunterType* hunter){
    sem_wait(&hunter->room->mutex);
    if(hunter->room->ghost != NULL){
            hunter->boredLevel = 0;
            hunter->fearLevel++;
        }
    else{
        hunter->boredLevel++;
    }
    sem_post(&hunter->room->mutex);
}
/*
    helper method to destroy semaphores
        in/out: roomSemaphore - semaphore array that will have its contents destroyed
*/
void destroyRoomSemaphores(RoomListType* roomSemaphore) {
    // Destroy the semaphores for each room
    RoomNodeType* temp = roomSemaphore->head;
    while(temp != NULL) {
        sem_destroy(&(temp->data->mutex));
        temp = temp->next;
    }
}
/*
    frees up memory for whole simulation
        in: house - house to have it's allocated memory cleaned
*/
void cleanUpHouse(HouseType house) {
    RoomListType* rooms = house.rooms;
    RoomNodeType* temp = rooms->head;
    HunterArrayType* hunters = house.hunters;
    while(temp != NULL) {
        //free the evidence list nodes then the list itself
        if (temp->data->evidence != NULL) {
            EvidenceNodeType* current = temp->data->evidence->head;
            while (current != NULL) {
                EvidenceNodeType* next = current->next;
                free(current);
                current = next;
            }
        }
        free(temp->data->evidence);
        //free the connectedTo list nodes then the list itself
        RoomNodeType* current = temp->data->connectedTo->head;
        while (current != NULL) {
            RoomNodeType* next = current->next;
            free(current);
            current = next;
        }
        free(temp->data->connectedTo);
        free(temp->data->hunters);
        free(temp->data);
        RoomNodeType* next = temp->next;
        free(temp); 
        temp = next;
        next = NULL;
    }
    free(hunters);
    free(rooms);
}
