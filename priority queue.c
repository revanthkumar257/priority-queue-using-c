#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct User {
    int id;
    int priority;
} User;

typedef struct Driver {
    int id;
    int priority;
} Driver;

typedef struct Node {
    void *data;
    struct Node *next;
} Node;

typedef struct PriorityQueue {
    Node *front;
} PriorityQueue;

PriorityQueue *initPriorityQueue() {
    PriorityQueue *queue = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    queue->front = NULL;
    return queue;
}

void enqueue(PriorityQueue *queue, void *data, int priority) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;

    if (queue->front == NULL || priority < ((User *)(queue->front->data))->priority) {
        newNode->next = queue->front;
        queue->front = newNode;
    } else {
        Node *current = queue->front;
        while (current->next != NULL && priority >= ((User *)(current->next->data))->priority) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

void printQueue(PriorityQueue *queue, const char *type) {
    Node *current = queue->front;

    printf("%s Queue:\n", type);
    while (current != NULL) {
        if (strcmp(type, "User") == 0) {
            User *userData = (User *)current->data;
            printf("ID: %d, Priority: %d\n", userData->id, userData->priority);
        } else if (strcmp(type, "Driver") == 0) {
            Driver *driverData = (Driver *)current->data;
            printf("ID: %d, Priority: %d\n", driverData->id, driverData->priority);
        }
        current = current->next;
    }
    printf("\n");
}

void *dequeue(PriorityQueue *queue) {
    Node *temp = NULL;
    if (queue->front == NULL) {
        return NULL; // Queue is empty
    } else {
        temp = queue->front;
        while (temp->next != NULL) {
            temp = temp->next;
        }
    }

    void *data = temp->data;
    queue->front = queue->front->next;
    free(temp);
    return data;
}

int main() {
    PriorityQueue *userQueue = initPriorityQueue();
    PriorityQueue *driverQueue = initPriorityQueue();
    char input[50];
    int id, priority;
    char userType[10];

    printf("Enter data (e.g., 'User 1 3' or 'Driver 101 5'). Enter 'exit' to finish:\n");

    while (1) {
        fgets(input, sizeof(input), stdin);
        if (strcmp(input, "exit\n") == 0) {
            break;
        }

        if (sscanf(input, "%s %d %d", userType, &id, &priority) == 3) {
            if (strcmp(userType, "User") == 0) {
                User *user = (User *)malloc(sizeof(User));
                user->id = id;
                user->priority = priority;
                enqueue(userQueue, user, user->priority);
            } else if (strcmp(userType, "Driver") == 0) {
                Driver *driver = (Driver *)malloc(sizeof(Driver));
                driver->id = id;
                driver->priority = priority;
                enqueue(driverQueue, driver, driver->priority);
            }
        } else {
            printf("Invalid input format. Please use 'User <id> <priority>' or 'Driver <id> <priority'.\n");
        }
    }

    printf("Final User Queue:\n");
    printQueue(userQueue, "User");
    
    printf("Final Driver Queue:\n");
    printQueue(driverQueue, "Driver");

    // Dequeue Users and Drivers based on priority.
    User *allocatedUser = (User *)dequeue(userQueue);
    Driver *allocatedDriver = (Driver *)dequeue(driverQueue);

    if (allocatedUser) {
        printf("Allocated User: ID %d, Priority %d\n", allocatedUser->id, allocatedUser->priority);
        free(allocatedUser);
    } else {
        printf("No allocated User.\n");
    }

    if (allocatedDriver) {
        printf("Allocated Driver: ID %d, Priority %d\n", allocatedDriver->id, allocatedDriver->priority);
        free(allocatedDriver);
    } else {
        printf("No allocated Driver.\n");
    }

    // Free memory and cleanup.
    while (userQueue->front) {
        Node *temp = userQueue->front;
        userQueue->front = userQueue->front->next;
        free(temp->data);
        free(temp);
    }
    free(userQueue);

    while (driverQueue->front) {
        Node *temp = driverQueue->front;
        driverQueue->front = driverQueue->front->next;
        free(temp->data);
        free(temp);
    }
    free(driverQueue);

    return 0;
}s