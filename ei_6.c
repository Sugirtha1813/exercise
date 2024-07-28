#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char orientation[10];
    char solarPanels[10];
    int dataCollected;
} Satellite;
void initializeSatellite(Satellite* sat) {
    strcpy(sat->orientation, "North");
    strcpy(sat->solarPanels, "Inactive");
    sat->dataCollected = 0;
}
void rotate(Satellite* sat, const char* direction) {
    strcpy(sat->orientation, direction);
    printf("Rotated to %s\n", sat->orientation);
}

void activatePanels(Satellite* sat) {
    strcpy(sat->solarPanels, "Active");
    printf("Solar Panels Activated\n");
}

void deactivatePanels(Satellite* sat) {
    strcpy(sat->solarPanels, "Inactive");
    printf("Solar Panels Deactivated\n");
}

void collectData(Satellite* sat) {
    if (strcmp(sat->solarPanels, "Active") == 0) {
        sat->dataCollected += 10;
        printf("Data Collected: %d units\n", sat->dataCollected);
    } else {
        printf("Cannot collect data. Solar Panels are Inactive\n");
    }
}
void executeCommands(Satellite* sat, const char** commands, int commandCount) {
    for (int i = 0; i < commandCount; i++) {
        if (strncmp(commands[i], "rotate", 6) == 0) {
            rotate(sat, commands[i] + 7);
        } else if (strcmp(commands[i], "activatePanels") == 0) {
            activatePanels(sat);
        } else if (strcmp(commands[i], "deactivatePanels") == 0) {
            deactivatePanels(sat);
        } else if (strcmp(commands[i], "collectData") == 0) {
            collectData(sat);
        } else {
            printf("Unknown command: %s\n", commands[i]);
        }
    }
}
int main() {
    Satellite sat;
    initializeSatellite(&sat);

    const char* commands[] = {
        "rotate South",
        "activatePanels",
        "collectData",
        "rotate East",
        "deactivatePanels",
        "collectData"
    };
    int commandCount = sizeof(commands) / sizeof(commands[0]);

    executeCommands(&sat, commands, commandCount);

    printf("\nFinal State:\n");
    printf("Orientation: %s\n", sat.orientation);
    printf("Solar Panels: %s\n", sat.solarPanels);
    printf("Data Collected: %d\n", sat.dataCollected);

    return 0;
}

