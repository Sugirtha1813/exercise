#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char description[100];
    char startTime[6];
    char endTime[6];
    char priority[10];
    int completed;
} Task;
Task* createTask(const char* description, const char* startTime, const char* endTime, const char* priority) {
    Task* task = (Task*)malloc(sizeof(Task));
    strcpy(task->description, description);
    strcpy(task->startTime, startTime);
    strcpy(task->endTime, endTime);
    strcpy(task->priority, priority);
    task->completed = 0;
    return task;
}
typedef struct Observer {
    void (*update)(const char* message);
} Observer;

void taskObserverUpdate(const char* message) {
    printf("Notification: %s\n", message);
}

Observer* createTaskObserver() {
    Observer* observer = (Observer*)malloc(sizeof(Observer));
    observer->update = taskObserverUpdate;
    return observer;
}
#define MAX_TASKS 100

typedef struct {
    Task* tasks[MAX_TASKS];
    int taskCount;
    Observer* observer;
} ScheduleManager;

ScheduleManager* getScheduleManager() {
    static ScheduleManager manager;
    static int initialized = 0;
    if (!initialized) {
        manager.taskCount = 0;
        manager.observer = createTaskObserver();
        initialized = 1;
    }
    return &manager;
}
int compareTime(const char* time1, const char* time2) {
    int h1, m1, h2, m2;
    sscanf(time1, "%d:%d", &h1, &m1);
    sscanf(time2, "%d:%d", &h2, &m2);
    if (h1 != h2) return h1 - h2;
    return m1 - m2;
}

int validateTask(ScheduleManager* manager, Task* newTask) {
    for (int i = 0; i < manager->taskCount; i++) {
        Task* task = manager->tasks[i];
        if (compareTime(newTask->startTime, task->endTime) < 0 &&
            compareTime(newTask->endTime, task->startTime) > 0) {
            return 0;
        }
    }
    return 1;
}

void addTask(ScheduleManager* manager, Task* task) {
    if (manager->taskCount >= MAX_TASKS) {
        manager->observer->update("Error: Task list is full.");
        return;
    }
    if (!validateTask(manager, task)) {
        manager->observer->update("Error: Task conflicts with an existing task.");
        return;
    }
    manager->tasks[manager->taskCount++] = task;
    manager->observer->update("Task added successfully. No conflicts.");
}

void removeTask(ScheduleManager* manager, const char* description) {
    for (int i = 0; i < manager->taskCount; i++) {
        if (strcmp(manager->tasks[i]->description, description) == 0) {
            free(manager->tasks[i]);
            for (int j = i; j < manager->taskCount - 1; j++) {
                manager->tasks[j] = manager->tasks[j + 1];
            }
            manager->taskCount--;
            manager->observer->update("Task removed successfully.");
            return;
        }
    }
    manager->observer->update("Error: Task not found.");
}

void viewTasks(ScheduleManager* manager) {
    if (manager->taskCount == 0) {
        printf("No tasks scheduled for the day.\n");
        return;
    }
    for (int i = 0; i < manager->taskCount; i++) {
        Task* task = manager->tasks[i];
        printf("%s - %s: %s [%s] %s\n", task->startTime, task->endTime, task->description, task->priority,
               task->completed ? "(Completed)" : "");
    }
}
void editTask(ScheduleManager* manager, const char* oldDescription, Task* newTask) {
    removeTask(manager, oldDescription);
    addTask(manager, newTask);
}

void markTaskCompleted(ScheduleManager* manager, const char* description) {
    for (int i = 0; i < manager->taskCount; i++) {
        if (strcmp(manager->tasks[i]->description, description) == 0) {
            manager->tasks[i]->completed = 1;
            manager->observer->update("Task marked as completed.");
            return;
        }
    }
    manager->observer->update("Error: Task not found.");
}

void viewTasksByPriority(ScheduleManager* manager, const char* priority) {
    int found = 0;
    for (int i = 0; i < manager->taskCount; i++) {
        Task* task = manager->tasks[i];
        if (strcmp(task->priority, priority) == 0) {
            printf("%s - %s: %s [%s] %s\n", task->startTime, task->endTime, task->description, task->priority,
                   task->completed ? "(Completed)" : "");
            found = 1;
        }
    }
    if (!found) {
        printf("No tasks with priority %s scheduled for the day.\n", priority);
    }
}
int main() {
    ScheduleManager* manager = getScheduleManager();

    addTask(manager, createTask("Morning Exercise", "07:00", "08:00", "High"));
    addTask(manager, createTask("Team Meeting", "09:00", "10:00", "Medium"));
    viewTasks(manager);

    removeTask(manager, "Morning Exercise");
    viewTasks(manager);

    addTask(manager, createTask("Training Session", "09:30", "10:30", "High"));
    addTask(manager, createTask("Lunch Break", "12:00", "13:00", "Low"));
    viewTasks(manager);

    markTaskCompleted(manager, "Team Meeting");
    viewTasks(manager);

    viewTasksByPriority(manager, "High");

    return 0;
}

