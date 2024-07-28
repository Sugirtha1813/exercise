#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_USERS 10
#define MAX_ROOMS 10
#define MAX_MESSAGE_LEN 256
#define MAX_USERNAME_LEN 50

typedef struct User {
    char username[MAX_USERNAME_LEN];
    int socket; // This will represent the user's connection
} User;

typedef struct Message {
    char username[MAX_USERNAME_LEN];
    char text[MAX_MESSAGE_LEN];
} Message;

typedef struct ChatRoom {
    char id[MAX_USERNAME_LEN];
    User* users[MAX_USERS];
    int userCount;
    Message* messages[MAX_USERS * 10];
    int messageCount;
} ChatRoom;

typedef struct ChatRoomManager {
    ChatRoom* rooms[MAX_ROOMS];
    int roomCount;
    pthread_mutex_t mutex;
} ChatRoomManager;

ChatRoomManager* getChatRoomManager() {
    static ChatRoomManager instance;
    static int initialized = 0;
    if (!initialized) {
        instance.roomCount = 0;
        pthread_mutex_init(&instance.mutex, NULL);
        initialized = 1;
    }
    return &instance;
}
ChatRoom* createChatRoom(const char* id) {
    ChatRoom* room = (ChatRoom*)malloc(sizeof(ChatRoom));
    strcpy(room->id, id);
    room->userCount = 0;
    room->messageCount = 0;
    return room;
}

void addUserToRoom(ChatRoom* room, User* user) {
    if (room->userCount < MAX_USERS) {
        room->users[room->userCount++] = user;
    }
}

void removeUserFromRoom(ChatRoom* room, User* user) {
    int i;
    for (i = 0; i < room->userCount; i++) {
        if (room->users[i] == user) {
            for (int j = i; j < room->userCount - 1; j++) {
                room->users[j] = room->users[j + 1];
            }
            room->userCount--;
            break;
        }
    }
}

void addMessageToRoom(ChatRoom* room, const char* username, const char* text) {
    if (room->messageCount < MAX_USERS * 10) {
        Message* msg = (Message*)malloc(sizeof(Message));
        strcpy(msg->username, username);
        strcpy(msg->text, text);
        room->messages[room->messageCount++] = msg;
    }
}

void printChatRoomStatus(ChatRoom* room) {
    printf("Chat Room ID: %s\n", room->id);
    printf("Active Users:\n");
    for (int i = 0; i < room->userCount; i++) {
        printf(" - %s\n", room->users[i]->username);
    }
    printf("Messages:\n");
    for (int i = 0; i < room->messageCount; i++) {
        printf("[%s]: %s\n", room->messages[i]->username, room->messages[i]->text);
    }
}
ChatRoom* findChatRoom(ChatRoomManager* manager, const char* id) {
    for (int i = 0; i < manager->roomCount; i++) {
        if (strcmp(manager->rooms[i]->id, id) == 0) {
            return manager->rooms[i];
        }
    }
    return NULL;
}

ChatRoom* createOrGetChatRoom(ChatRoomManager* manager, const char* id) {
    pthread_mutex_lock(&manager->mutex);
    ChatRoom* room = findChatRoom(manager, id);
    if (!room) {
        if (manager->roomCount < MAX_ROOMS) {
            room = createChatRoom(id);
            manager->rooms[manager->roomCount++] = room;
        }
    }
    pthread_mutex_unlock(&manager->mutex);
    return room;
}
int main() {
    ChatRoomManager* manager = getChatRoomManager();

    // Simulating creating chat rooms and adding users
    ChatRoom* room1 = createOrGetChatRoom(manager, "Room123");
    ChatRoom* room2 = createOrGetChatRoom(manager, "Room456");

    User* user1 = (User*)malloc(sizeof(User));
    strcpy(user1->username, "Alice");
    user1->socket = 1;

    User* user2 = (User*)malloc(sizeof(User));
    strcpy(user2->username, "Bob");
    user2->socket = 2;

    addUserToRoom(room1, user1);
    addUserToRoom(room1, user2);

    addMessageToRoom(room1, "Alice", "Hello, everyone!");
    addMessageToRoom(room1, "Bob", "How's it going?");

    printChatRoomStatus(room1);

    removeUserFromRoom(room1, user1);
    printChatRoomStatus(room1);

    free(user1);
    free(user2);
    return 0;
}

