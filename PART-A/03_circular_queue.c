/**
 * ============================================================================
 * BMS INSTITUTE OF TECHNOLOGY & MANAGEMENT
 * DEPARTMENT OF ARTIFICIAL INTELLIGENCE AND MACHINE LEARNING
 * Course: Data Structures & Algorithms Laboratory (Course Code: 1BAIL306)
 * Semester: III | Scheme: 2025
 * ----------------------------------------------------------------------------
 * PART A - EXPERIMENT 3: Circular Queue Implementation
 * 
 * Objective:
 *   Implement a circular queue using an array in C.
 *   Perform enqueue, dequeue, and display operations.
 *   Demonstrate the working of the circular queue by inserting and deleting elements.
 *   Show how a circular queue overcomes the limitation of a linear queue. (CO1)
 * 
 * Conceptual Understanding:
 *   - Linear Queue Limitation: In a linear queue, once 'rear' reaches MAX - 1,
 *     no new elements can be enqueued even if elements have been dequeued from
 *     the front (free memory slots exist). This causes "False Overflow" and
 *     wastes allocated memory.
 *   - Circular Queue Solution: By using modulo arithmetic `(rear + 1) % MAX`,
 *     the rear pointer wraps around to index 0, allowing reuse of vacant slots.
 * 
 * Time & Space Complexity:
 *   - Enqueue: Time: O(1), Space: O(1)
 *   - Dequeue: Time: O(1), Space: O(1)
 *   - Display: Time: O(n), Space: O(1)
 *   - Space Complexity: Fixed array of size MAX -> O(MAX)
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 5

typedef struct {
    int items[MAX_SIZE];
    int front;
    int rear;
    int count;
} CircularQueue;

/* Initialize circular queue */
void initQueue(CircularQueue* q) {
    q->front = -1;
    q->rear = -1;
    q->count = 0;
}

/* Check if queue is empty */
int isEmpty(CircularQueue* q) {
    return (q->front == -1);
}

/* Check if queue is full */
int isFull(CircularQueue* q) {
    return ((q->rear + 1) % MAX_SIZE == q->front);
}

/* Enqueue operation */
int enqueue(CircularQueue* q, int value) {
    if (isFull(q)) {
        printf("[Overflow] Circular Queue is FULL! Cannot enqueue %d.\n", value);
        return 0;
    }

    if (isEmpty(q)) {
        q->front = 0;
        q->rear = 0;
    } else {
        q->rear = (q->rear + 1) % MAX_SIZE;
    }

    q->items[q->rear] = value;
    q->count++;
    printf("[Enqueue] %d inserted at index [%d]. (Front=%d, Rear=%d, Count=%d/%d)\n",
           value, q->rear, q->front, q->rear, q->count, MAX_SIZE);
    return 1;
}

/* Dequeue operation */
int dequeue(CircularQueue* q, int* deletedVal) {
    if (isEmpty(q)) {
        printf("[Underflow] Circular Queue is EMPTY! Nothing to dequeue.\n");
        return 0;
    }

    *deletedVal = q->items[q->front];
    printf("[Dequeue] Element %d removed from index [%d]. ", *deletedVal, q->front);

    if (q->front == q->rear) {
        // Last element deleted, reset queue
        q->front = -1;
        q->rear = -1;
    } else {
        q->front = (q->front + 1) % MAX_SIZE;
    }

    q->count--;
    printf("(New Front=%d, Rear=%d, Count=%d/%d)\n", q->front, q->rear, q->count, MAX_SIZE);
    return 1;
}

/* Display circular queue items and raw array slots */
void display(CircularQueue* q) {
    if (isEmpty(q)) {
        printf("\nQueue State: [ EMPTY ]\n");
        printf("Array Slots: [ _ , _ , _ , _ , _ ] (Front = %d, Rear = %d)\n\n", q->front, q->rear);
        return;
    }

    printf("\n--- Logical Queue Elements ---\n");
    printf("Front -> ");
    int i = q->front;
    while (1) {
        printf("[%d] ", q->items[i]);
        if (i == q->rear) break;
        i = (i + 1) % MAX_SIZE;
    }
    printf("<- Rear\n");

    printf("\n--- Physical Array Memory Slots (0 to %d) ---\n", MAX_SIZE - 1);
    for (int idx = 0; idx < MAX_SIZE; idx++) {
        int isActive = 0;
        if (!isEmpty(q)) {
            if (q->front <= q->rear) {
                if (idx >= q->front && idx <= q->rear) isActive = 1;
            } else {
                if (idx >= q->front || idx <= q->rear) isActive = 1;
            }
        }
        if (isActive) {
            printf("  Slot[%d]: %4d", idx, q->items[idx]);
            if (idx == q->front && idx == q->rear) printf("  <-- FRONT & REAR");
            else if (idx == q->front) printf("  <-- FRONT");
            else if (idx == q->rear) printf("  <-- REAR");
            printf("\n");
        } else {
            printf("  Slot[%d]: [EMPTY / VACATED]\n", idx);
        }
    }
    printf("Total items: %d / %d\n\n", q->count, MAX_SIZE);
}

/* 
 * Dedicated demonstration showing how circular queue overcomes linear queue limitation
 */
void demonstrateLimitationOvercome(void) {
    CircularQueue q;
    initQueue(&q);
    int dummy;

    printf("\n===============================================================\n");
    printf(" DEMONSTRATION: OVERCOMING THE LIMITATION OF A LINEAR QUEUE    \n");
    printf("===============================================================\n");
    printf("Max Capacity: %d elements.\n\n", MAX_SIZE);

    printf("Step 1: Fill the queue completely with 5 items (10, 20, 30, 40, 50)...\n");
    for (int i = 1; i <= 5; i++) {
        enqueue(&q, i * 10);
    }
    display(&q);

    printf("Step 2: Try to insert one more element (60)...\n");
    enqueue(&q, 60); // Fails (Queue Full)

    printf("\nStep 3: Dequeue 2 elements from the front (releasing slots 0 and 1)...\n");
    dequeue(&q, &dummy);
    dequeue(&q, &dummy);
    display(&q);

    printf(">>> ANALYSIS AT THIS STAGE:\n");
    printf("    In a standard LINEAR QUEUE:\n");
    printf("      - 'rear' is at index %d (MAX - 1).\n", q.rear);
    printf("      - Even though slots 0 and 1 are empty, linear queue checks:\n");
    printf("        'if (rear == MAX - 1) return Overflow;'\n");
    printf("      - Result: Inserting new items FAILS (False Overflow)!\n\n");
    printf("    In this CIRCULAR QUEUE:\n");
    printf("      - 'rear' wraps around to index 0 using: (rear + 1) %% %d\n", MAX_SIZE);
    printf("      - Vacated memory slots 0 and 1 CAN BE REUSED!\n\n");

    printf("Step 4: Enqueue element 60 and element 70 into circular queue...\n");
    enqueue(&q, 60);
    enqueue(&q, 70);
    display(&q);

    printf("Conclusion: Circular Queue successfully re-utilized slots 0 and 1.\n");
    printf("Memory utilization is 100%% without shifting array elements!\n");
    printf("===============================================================\n\n");
}

int main(void) {
    CircularQueue q;
    initQueue(&q);
    int choice, val;

    printf("===============================================================\n");
    printf("    BMSIT&M - DSA LAB - EXPERIMENT 3: CIRCULAR QUEUE           \n");
    printf("===============================================================\n");

    while (1) {
        printf("--- CIRCULAR QUEUE MENU (Capacity: %d) ---\n", MAX_SIZE);
        printf("1. Enqueue Element\n");
        printf("2. Dequeue Element\n");
        printf("3. Display Queue State\n");
        printf("4. Run Demonstration: Overcoming Linear Queue Limitation\n");
        printf("5. View Complexity Analysis\n");
        printf("6. Exit\n");
        printf("Enter choice (1-6): ");

        if (scanf("%d", &choice) != 1) {
            printf("[Error] Invalid input. Exiting.\n");
            break;
        }

        switch (choice) {
            case 1:
                printf("Enter integer to enqueue: ");
                scanf("%d", &val);
                enqueue(&q, val);
                break;

            case 2:
                dequeue(&q, &val);
                break;

            case 3:
                display(&q);
                break;

            case 4:
                demonstrateLimitationOvercome();
                break;

            case 5:
                printf("\n===============================================================\n");
                printf("              CIRCULAR QUEUE: COMPLEXITY ANALYSIS             \n");
                printf("===============================================================\n");
                printf("Operations:\n");
                printf("  - Enqueue: O(1) time, O(1) space\n");
                printf("  - Dequeue: O(1) time, O(1) space\n");
                printf("  - Display: O(n) time, O(1) space\n");
                printf("Space Efficiency:\n");
                printf("  - Linear Queue memory waste: O(k) unused slots after k dequeues\n");
                printf("  - Circular Queue memory waste: 0 slots (optimal buffer reuse)\n");
                printf("===============================================================\n\n");
                break;

            case 6:
                printf("Exiting program. Goodbye!\n");
                return 0;

            default:
                printf("[Error] Invalid choice! Select 1-6.\n");
        }
    }

    return 0;
}
