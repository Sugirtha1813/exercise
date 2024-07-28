#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    char stage[20];
    double fuel;
    double altitude;
    double speed;
    int checksComplete;
    int missionFailed;
} Rocket;
void initializeRocket(Rocket *rocket) {
    strcpy(rocket->stage, "Pre-Launch");
    rocket->fuel = 100.0;
    rocket->altitude = 0.0;
    rocket->speed = 0.0;
    rocket->checksComplete = 0;
    rocket->missionFailed = 0;
}
void preLaunchChecks(Rocket *rocket) {
    if (strcmp(rocket->stage, "Pre-Launch") == 0) {
        rocket->checksComplete = 1;
        printf("All systems are 'Go' for launch.\n");
    } else {
        printf("Cannot perform pre-launch checks in current stage.\n");
    }
}
void launchRocket(Rocket *rocket) {
    if (rocket->checksComplete) {
        strcpy(rocket->stage, "Stage 1");
        printf("Launch initiated...\n");
    } else {
        printf("Please complete pre-launch checks before launch.\n");
    }
}

void updateStage(Rocket *rocket) {
    if (strcmp(rocket->stage, "Stage 1") == 0) {
        rocket->fuel -= 10;
        rocket->altitude += 10;
        rocket->speed += 1000;

        if (rocket->fuel <= 0) {
            rocket->missionFailed = 1;
            printf("Mission Failed due to insufficient fuel.\n");
        } else if (rocket->altitude >= 100) {
            strcpy(rocket->stage, "Stage 2");
            printf("Stage 1 complete. Separating stage. Entering Stage 2.\n");
        } else {
            printf("Stage: 1, Fuel: %.1f%%, Altitude: %.1f km, Speed: %.1f km/h\n", rocket->fuel, rocket->altitude, rocket->speed);
        }
    } else if (strcmp(rocket->stage, "Stage 2") == 0) {
        rocket->fuel -= 5;
        rocket->altitude += 20;
        rocket->speed += 500;

        if (rocket->fuel <= 0) {
            rocket->missionFailed = 1;
            printf("Mission Failed due to insufficient fuel.\n");
        } else if (rocket->altitude >= 200) {
            strcpy(rocket->stage, "Orbit");
            printf("Orbit achieved! Mission Successful.\n");
        } else {
            printf("Stage: 2, Fuel: %.1f%%, Altitude: %.1f km, Speed: %.1f km/h\n", rocket->fuel, rocket->altitude, rocket->speed);
        }
    }
}
void fastForward(Rocket *rocket, int seconds) {
    for (int i = 0; i < seconds; i++) {
        if (rocket->missionFailed || strcmp(rocket->stage, "Orbit") == 0) {
            break;
        }
        updateStage(rocket);
        sleep(1);
    }
}

