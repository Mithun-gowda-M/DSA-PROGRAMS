/**
 * ============================================================================
 * BMS INSTITUTE OF TECHNOLOGY & MANAGEMENT
 * DEPARTMENT OF ARTIFICIAL INTELLIGENCE AND MACHINE LEARNING
 * Course: Data Structures & Algorithms Laboratory (Course Code: 1BAIL306)
 * Semester: III | Scheme: 2025
 * ----------------------------------------------------------------------------
 * PART A - EXPERIMENT 1: Singly Linked List Operations
 * 
 * Objective:
 *   Implement a singly linked list in C with the following operations:
 *   1. Create a list
 *   2. Insert a node at the end
 *   3. Delete a node by value
 *   4. Search for an element
 *   5. Reverse the list
 *   6. Display the list
 *   Write a menu-driven program to demonstrate all operations.
 *   Analyze and state the time complexity of each operation. (CO1)
 * 
 * Time & Space Complexity Summary:
 *   - Create List (n elements):   Time: O(n),       Space: O(n)
 *   - Insert at End (without tail pointer): Time: O(n), Space: O(1)
 *   - Insert at End (with tail pointer):    Time: O(1), Space: O(1)
 *   - Delete by Value:            Time: O(n),       Space: O(1)
 *   - Search Element:             Time: O(n),       Space: O(1)
 *   - Reverse List (in-place):    Time: O(n),       Space: O(1)
 *   - Display List:               Time: O(n),       Space: O(1)
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

/* Node structure for Singly Linked List */
typedef struct Node {
    int data;
    struct Node* next;
} Node;

/* Function to create a new node dynamically */
Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("[Error] Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}

/* 
 * 1. Create a list with n elements
 * Time Complexity: O(n)
 * Space Complexity: O(n) for n dynamic nodes
 */
Node* createList(int n) {
    if (n <= 0) {
        printf("[Info] Invalid number of nodes. List remains empty.\n");
        return NULL;
    }
    Node* head = NULL;
    Node* tail = NULL;
    printf("Enter %d values:\n", n);
    for (int i = 0; i < n; i++) {
        int val;
        printf("  Element %d: ", i + 1);
        if (scanf("%d", &val) != 1) {
            printf("[Error] Invalid input.\n");
            return head;
        }
        Node* newNode = createNode(val);
        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    printf("[Success] Singly Linked List created with %d elements.\n", n);
    return head;
}

/* 
 * 2. Insert a node at the end of the list
 * Time Complexity: O(n) - traverses to the last node
 * Space Complexity: O(1) auxiliary
 */
Node* insertAtEnd(Node* head, int value) {
    Node* newNode = createNode(value);
    if (head == NULL) {
        printf("[Success] Node %d inserted as the head of the list.\n", value);
        return newNode;
    }
    Node* temp = head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
    printf("[Success] Node %d inserted at the end of the list.\n", value);
    return head;
}

/* 
 * 3. Delete a node by value
 * Time Complexity: O(n) - searches for the value in worst/average case
 * Space Complexity: O(1) auxiliary
 */
Node* deleteByValue(Node* head, int value) {
    if (head == NULL) {
        printf("[Warning] List is empty! Nothing to delete.\n");
        return NULL;
    }

    // Case 1: Value is at the head node
    if (head->data == value) {
        Node* temp = head;
        head = head->next;
        free(temp);
        printf("[Success] Node with value %d deleted (was head node).\n", value);
        return head;
    }

    // Case 2: Value is in middle or end
    Node* current = head;
    Node* prev = NULL;
    while (current != NULL && current->data != value) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("[Info] Element %d not found in the list. Deletion failed.\n", value);
        return head;
    }

    prev->next = current->next;
    free(current);
    printf("[Success] Node with value %d successfully deleted.\n", value);
    return head;
}

/* 
 * 4. Search for an element in the list
 * Time Complexity: O(n) - linear traversal
 * Space Complexity: O(1) auxiliary
 * Returns: 1-based position if found, -1 otherwise
 */
int searchElement(Node* head, int key) {
    Node* temp = head;
    int position = 1;
    while (temp != NULL) {
        if (temp->data == key) {
            return position;
        }
        temp = temp->next;
        position++;
    }
    return -1;
}

/* 
 * 5. Reverse the linked list in-place
 * Time Complexity: O(n) - single pass through list
 * Space Complexity: O(1) auxiliary - in-place pointer manipulation
 */
Node* reverseList(Node* head) {
    if (head == NULL || head->next == NULL) {
        printf("[Info] List has 0 or 1 element, already reversed.\n");
        return head;
    }
    Node* prev = NULL;
    Node* current = head;
    Node* nextNode = NULL;

    while (current != NULL) {
        nextNode = current->next; // Store next node
        current->next = prev;     // Reverse pointer
        prev = current;           // Move prev forward
        current = nextNode;       // Move current forward
    }
    printf("[Success] List has been reversed in-place.\n");
    return prev; // prev is the new head
}

/* 
 * 6. Display the linked list
 * Time Complexity: O(n)
 * Space Complexity: O(1)
 */
void displayList(Node* head) {
    if (head == NULL) {
        printf("List: [ EMPTY ]\n");
        return;
    }
    printf("List: ");
    Node* temp = head;
    while (temp != NULL) {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}

/* Free all allocated memory */
void freeList(Node* head) {
    Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

/* Display complexity analysis report */
void displayComplexityAnalysis(void) {
    printf("\n===============================================================\n");
    printf("        SINGLY LINKED LIST: TIME COMPLEXITY ANALYSIS           \n");
    printf("===============================================================\n");
    printf("| Operation              | Best Case | Average Case | Worst Case |\n");
    printf("|------------------------|-----------|--------------|------------|\n");
    printf("| Create (n elements)    | O(n)      | O(n)         | O(n)       |\n");
    printf("| Insert at End          | O(n)*     | O(n)*        | O(n)*      |\n");
    printf("| Delete by Value        | O(1)      | O(n)         | O(n)       |\n");
    printf("| Search for Element     | O(1)      | O(n)         | O(n)       |\n");
    printf("| Reverse List (In-Place)| O(n)      | O(n)         | O(n)       |\n");
    printf("| Display List           | O(n)      | O(n)         | O(n)       |\n");
    printf("===============================================================\n");
    printf(" * Note: Insert at end is O(1) if a tail pointer is maintained.\n");
    printf(" Space Complexity: O(1) auxiliary for all iterative operations.\n");
    printf("===============================================================\n\n");
}

int main(void) {
    Node* head = NULL;
    int choice, val, pos, n;

    printf("===============================================================\n");
    printf("   BMSIT&M - DSA LAB - EXPERIMENT 1: SINGLY LINKED LIST       \n");
    printf("===============================================================\n");

    while (1) {
        printf("\n--- MENU ---\n");
        printf("1. Create List\n");
        printf("2. Insert Node at End\n");
        printf("3. Delete Node by Value\n");
        printf("4. Search for Element\n");
        printf("5. Reverse List\n");
        printf("6. Display List\n");
        printf("7. View Time Complexity Analysis\n");
        printf("8. Exit\n");
        printf("Enter your choice (1-8): ");

        if (scanf("%d", &choice) != 1) {
            printf("[Error] Invalid input. Exiting.\n");
            break;
        }

        switch (choice) {
            case 1:
                if (head != NULL) {
                    printf("[Warning] Freeing existing list before re-creation.\n");
                    freeList(head);
                    head = NULL;
                }
                printf("Enter number of initial nodes: ");
                scanf("%d", &n);
                head = createList(n);
                displayList(head);
                break;

            case 2:
                printf("Enter integer value to insert at end: ");
                scanf("%d", &val);
                head = insertAtEnd(head, val);
                displayList(head);
                break;

            case 3:
                printf("Enter integer value to delete: ");
                scanf("%d", &val);
                head = deleteByValue(head, val);
                displayList(head);
                break;

            case 4:
                printf("Enter integer value to search: ");
                scanf("%d", &val);
                pos = searchElement(head, val);
                if (pos != -1) {
                    printf("[Found] Element %d found at position %d (1-indexed).\n", val, pos);
                } else {
                    printf("[Not Found] Element %d is not in the list.\n", val);
                }
                break;

            case 5:
                head = reverseList(head);
                displayList(head);
                break;

            case 6:
                displayList(head);
                break;

            case 7:
                displayComplexityAnalysis();
                break;

            case 8:
                printf("Freeing memory and exiting program. Goodbye!\n");
                freeList(head);
                head = NULL;
                return 0;

            default:
                printf("[Error] Invalid choice! Please select an option between 1 and 8.\n");
        }
    }

    freeList(head);
    return 0;
}
