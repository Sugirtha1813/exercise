#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CLASSES 100
#define MAX_STUDENTS 1000
#define MAX_ASSIGNMENTS 1000

typedef struct {
    char name[50];
} Classroom;

typedef struct {
    char id[50];
    char className[50];
} Student;

typedef struct {
    char className[50];
    char details[100];
} Assignment;

typedef struct {
    Classroom classrooms[MAX_CLASSES];
    int classroomCount;
    Student students[MAX_STUDENTS];
    int studentCount;
    Assignment assignments[MAX_ASSIGNMENTS];
    int assignmentCount;
} VirtualClassroomManager;
void initializeManager(VirtualClassroomManager *manager) {
    manager->classroomCount = 0;
    manager->studentCount = 0;
    manager->assignmentCount = 0;
}
void addClassroom(VirtualClassroomManager *manager, const char *className) {
    if (manager->classroomCount < MAX_CLASSES) {
        strcpy(manager->classrooms[manager->classroomCount].name, className);
        manager->classroomCount++;
        printf("Classroom %s has been created.\n", className);
    } else {
        printf("Maximum number of classrooms reached.\n");
    }
}
void addStudent(VirtualClassroomManager *manager, const char *studentId, const char *className) {
    if (manager->studentCount < MAX_STUDENTS) {
        strcpy(manager->students[manager->studentCount].id, studentId);
        strcpy(manager->students[manager->studentCount].className, className);
        manager->studentCount++;
        printf("Student %s has been enrolled in %s.\n", studentId, className);
    } else {
        printf("Maximum number of students reached.\n");
    }
}
void scheduleAssignment(VirtualClassroomManager *manager, const char *className, const char *details) {
    if (manager->assignmentCount < MAX_ASSIGNMENTS) {
        strcpy(manager->assignments[manager->assignmentCount].className, className);
        strcpy(manager->assignments[manager->assignmentCount].details, details);
        manager->assignmentCount++;
        printf("Assignment for %s has been scheduled.\n", className);
    } else {
        printf("Maximum number of assignments reached.\n");
    }
}
void submitAssignment(VirtualClassroomManager *manager, const char *studentId, const char *className, const char *details) {
    for (int i = 0; i < manager->assignmentCount; i++) {
        if (strcmp(manager->assignments[i].className, className) == 0 && strcmp(manager->assignments[i].details, details) == 0) {
            printf("Assignment submitted by Student %s in %s.\n", studentId, className);
            return;
        }
    }
    printf("Assignment not found for %s in %s.\n", details, className);
}
int main() {
    VirtualClassroomManager manager;
    initializeManager(&manager);

    char command[256];
    while (1) {
        printf("Enter command: ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0; // Remove newline character

        char *token = strtok(command, " ");
        if (strcmp(token, "add_classroom") == 0) {
            token = strtok(NULL, " ");
            if (token) {
                addClassroom(&manager, token);
            } else {
                printf("Please provide a class name.\n");
            }
        } else if (strcmp(token, "add_student") == 0) {
            char *studentId = strtok(NULL, " ");
            char *className = strtok(NULL, " ");
            if (studentId && className) {
                addStudent(&manager, studentId, className);
            } else {
                printf("Please provide a student ID and class name.\n");
            }
        } else if (strcmp(token, "schedule_assignment") == 0) {
            char *className = strtok(NULL, " ");
            char *details = strtok(NULL, "");
            if (className && details) {
                scheduleAssignment(&manager, className, details);
            } else {
                printf("Please provide a class name and assignment details.\n");
            }
        } else if (strcmp(token, "submit_assignment") == 0) {
            char *studentId = strtok(NULL, " ");
            char *className = strtok(NULL, " ");
            char *details = strtok(NULL, "");
            if (studentId && className && details) {
                submitAssignment(&manager, studentId, className, details);
            } else {
                printf("Please provide a student ID, class name, and assignment details.\n");
            }
        } else {
            printf("Unknown command: %s\n", command);
        }
    }

    return 0;
}

