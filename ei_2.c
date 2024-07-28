#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int roomId;
    int isBooked;
    int isOccupied;
    time_t bookingTime;
    time_t occupancyTime;
} Room;
#define MAX_ROOMS 100
#define MAX_OBSERVERS 10

typedef struct {
    Room rooms[MAX_ROOMS];
    int roomCount;
} Office;

Office* getOffice() {
    static Office office;
    static int initialized = 0;
    if (!initialized) {
        office.roomCount = 0;
        initialized = 1;
    }
    return &office;
}

void configureOffice(int numRooms) {
    Office* office = getOffice();
    office->roomCount = numRooms;
    for (int i = 0; i < numRooms; i++) {
        office->rooms[i].roomId = i + 1;
        office->rooms[i].isBooked = 0;
        office->rooms[i].isOccupied = 0;
    }
}
typedef struct Observer {
    void (*update)(Room* room);
} Observer;

void sensorUpdate(Room* room) {
    if (room->isOccupied) {
        printf("Sensor: Room %d is now occupied.\n", room->roomId);
    } else {
        printf("Sensor: Room %d is now unoccupied.\n", room->roomId);
    }
}

Observer* createSensor() {
    Observer* observer = (Observer*)malloc(sizeof(Observer));
    observer->update = sensorUpdate;
    return observer;
}

void controlSystemUpdate(Room* room) {
    if (room->isOccupied) {
        printf("ControlSystem: Turning on lights and AC in room %d.\n", room->roomId);
    } else {
        printf("ControlSystem: Turning off lights and AC in room %d.\n", room->roomId);
    }
}

Observer* createControlSystem() {
    Observer* observer = (Observer*)malloc(sizeof(Observer));
    observer->update = controlSystemUpdate;
    return observer;
}
typedef struct Command {
    void (*execute)();
} Command;

typedef struct {
    Command command;
    int roomId;
} BookingCommand;

void bookRoomExecute() {
    Office* office = getOffice();
    for (int i = 0; i < office->roomCount; i++) {
        if (office->rooms[i].roomId == roomId && !office->rooms[i].isBooked) {
            office->rooms[i].isBooked = 1;
            office->rooms[i].bookingTime = time(NULL);
            printf("Room %d booked successfully.\n", roomId);
            return;
        }
    }
    printf("Error: Room %d is already booked or does not exist.\n", roomId);
}

Command* createBookingCommand(int roomId) {
    BookingCommand* bookingCommand = (BookingCommand*)malloc(sizeof(BookingCommand));
    bookingCommand->command.execute = bookRoomExecute;
    bookingCommand->roomId = roomId;
    return (Command*)bookingCommand;
}

typedef struct {
    Command command;
    int roomId;
} CancelCommand;

void cancelRoomExecute() {
    Office* office = getOffice();
    for (int i = 0; i < office->roomCount; i++) {
        if (office->rooms[i].roomId == roomId && office->rooms[i].isBooked) {
            office->rooms[i].isBooked = 0;
            printf("Room %d booking cancelled successfully.\n", roomId);
            return;
        }
    }
    printf("Error: Room %d is not booked or does not exist.\n", roomId);
}

Command* createCancelCommand(int roomId) {
    CancelCommand* cancelCommand = (CancelCommand*)malloc(sizeof(CancelCommand));
    cancelCommand->command.execute = cancelRoomExecute;
    cancelCommand->roomId = roomId;
    return (Command*)cancelCommand;
}

typedef struct {
    Command command;
    int roomId;
    int isOccupied;
} StatusUpdateCommand;

void updateRoomStatusExecute() {
    Office* office = getOffice();
    for (int i = 0; i < office->roomCount; i++) {
        if (office->rooms[i].roomId == roomId) {
            office->rooms[i].isOccupied = isOccupied;
            if (isOccupied) {
                office->rooms[i].occupancyTime = time(NULL);
            }
            for (int j = 0; j < observerCount; j++) {
                observers[j]->update(&office->
Observer* observers[MAX_OBSERVERS];
int observerCount = 0;

void addObserver(Observer* observer) {
    if (observerCount < MAX_OBSERVERS) {
        observers[observerCount++] = observer;
    }
}

void notifyObservers(Room* room) {
    for (int i = 0; i < observerCount; i++) {
        observers[i]->update(room);
    }
}
void autoReleaseBookings() {
    Office* office = getOffice();
    time_t currentTime = time(NULL);
    for (int i = 0; i < office->roomCount; i++) {
        if (office->rooms[i].isBooked && !office->rooms[i].isOccupied &&
            difftime(currentTime, office->rooms[i].bookingTime) >= 300) {
            office->rooms[i].isBooked = 0;
            printf("Booking for room %d released due to no occupancy within 5 minutes.\n", office->rooms[i].roomId);
        }
    }
}
int main() {
    configureOffice(5);

    addObserver(createSensor());
    addObserver(createControlSystem());

    Command* bookingCommand = createBookingCommand(1);
    bookingCommand->execute();

    Command* statusUpdateCommand = createStatusUpdateCommand(1, 1);
    statusUpdateCommand->execute();

    Command* cancelCommand = createCancelCommand(1);
    cancelCommand->execute();

    autoReleaseBookings();

    return 0;
}

