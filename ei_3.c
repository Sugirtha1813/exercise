#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    NORTH,
    EAST,
    SOUTH,
    WEST
} Direction;

typedef struct {
    int x;
    int y;
    Direction direction;
} Position;
typedef struct Command {
    void (*execute)(void*);
} Command;

typedef struct {
    Command command;
    void* rover;
} MoveCommand;

typedef struct {
    Command command;
    void* rover;
} LeftCommand;

typedef struct {
    Command command;
    void* rover;
} RightCommand;

void moveExecute(void* rover);
void leftExecute(void* rover);
void rightExecute(void* rover);

Command* createMoveCommand(void* rover) {
    MoveCommand* moveCommand = (MoveCommand*)malloc(sizeof(MoveCommand));
    moveCommand->command.execute = moveExecute;
    moveCommand->rover = rover;
    return (Command*)moveCommand;
}

Command* createLeftCommand(void* rover) {
    LeftCommand* leftCommand = (LeftCommand*)malloc(sizeof(LeftCommand));
    leftCommand->command.execute = leftExecute;
    leftCommand->rover = rover;
    return (Command*)leftCommand;
}

Command* createRightCommand(void* rover) {
    RightCommand* rightCommand = (RightCommand*)malloc(sizeof(RightCommand));
    rightCommand->command.execute = rightExecute;
    rightCommand->rover = rover;
    return (Command*)rightCommand;
}
typedef struct {
    Position position;
    int gridWidth;
    int gridHeight;
    int** obstacles;
} Rover;

void moveExecute(void* rover) {
    Rover* r = (Rover*)rover;
    int newX = r->position.x;
    int newY = r->position.y;

    switch (r->position.direction) {
        case NORTH: newY++; break;
        case EAST:  newX++; break;
        case SOUTH: newY--; break;
        case WEST:  newX--; break;
    }

    if (newX >= 0 && newX < r->gridWidth && newY >= 0 && newY < r->gridHeight && r->obstacles[newX][newY] == 0) {
        r->position.x = newX;
        r->position.y = newY;
    } else {
        printf("Obstacle detected or out of bounds at (%d, %d). Cannot move.\n", newX, newY);
    }
}

void leftExecute(void* rover) {
    Rover* r = (Rover*)rover;
    r->position.direction = (r->position.direction + 3) % 4; // Equivalent to (direction - 1) % 4 without if-else
}

void rightExecute(void* rover) {
    Rover* r = (Rover*)rover;
    r->position.direction = (r->position.direction + 1) % 4; // Equivalent to (direction + 1) % 4 without if-else
}

Rover* createR
int** createGrid(int width, int height, int obstacleCount, int obstacles[][2]) {
    int** grid = (int**)malloc(width * sizeof(int*));
    for (int i = 0; i < width; i++) {
        grid[i] = (int*)calloc(height, sizeof(int));
    }

    for (int i = 0; i < obstacleCount; i++) {
        int x = obstacles[i][0];
        int y = obstacles[i][1];
        grid[x][y] = 1;
    }

    return grid;
}

void freeGrid(int** grid, int width) {
    for (int i = 0; i < width; i++) {
        free(grid[i]);
    }
    free(grid);
}
int main() {
    int gridWidth = 10;
    int gridHeight = 10;
    int obstacles[][2] = { {2, 2}, {3, 5} };
    int obstacleCount = sizeof(obstacles) / sizeof(obstacles[0]);
    int** grid = createGrid(gridWidth, gridHeight, obstacleCount, obstacles);

    Rover* rover = createRover(0, 0, NORTH, gridWidth, gridHeight, grid);

    Command* commands[] = {
        createMoveCommand(rover),
        createMoveCommand(rover),
        createRightCommand(rover),
        createMoveCommand(rover),
        createLeftCommand(rover),
        createMoveCommand(rover)
    };

    for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        commands[i]->execute(rover);
    }

    printf("Rover final position: (%d, %d), facing %d\n", rover->position.x, rover->position.y, rover->position.direction);

    freeGrid(grid, gridWidth);
    free(rover);
    for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        free(commands[i]);
    }

    return 0;
}

