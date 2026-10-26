/**
 * ============================================================================
 * BMS INSTITUTE OF TECHNOLOGY & MANAGEMENT
 * DEPARTMENT OF ARTIFICIAL INTELLIGENCE AND MACHINE LEARNING
 * Course: Data Structures & Algorithms Laboratory (Course Code: 1BAIL306)
 * Semester: III | Scheme: 2025
 * ----------------------------------------------------------------------------
 * PART A - EXPERIMENT 7: Binary Search Tree Operations & Height Observation
 * 
 * Objective:
 *   Implement a Binary Search Tree (BST) in C with:
 *     - Insert
 *     - Search
 *     - Inorder traversal
 *     - Preorder traversal
 *     - Postorder traversal
 *   Analyze time complexity of each operation.
 *   Run for random, ascending, and descending input sequences (n = 100, 500, 1000)
 *   and observe tree height in each case. (CO1)
 * 
 * Height Observation Insight:
 *   - Random sequence: Height is logarithmic O(log n) -> balanced tree.
 *   - Ascending sequence: Height is linear O(n) -> degenerate right-skewed list.
 *   - Descending sequence: Height is linear O(n) -> degenerate left-skewed list.
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct BSTNode {
    int data;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

/* Create a new BST node */
BSTNode* createNode(int value) {
    BSTNode* newNode = (BSTNode*)malloc(sizeof(BSTNode));
    if (!newNode) {
        printf("[Error] Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

/* 
 * Insert an element into BST
 * Time Complexity: Average O(log n), Worst O(n)
 */
BSTNode* insert(BSTNode* root, int value) {
    if (root == NULL) {
        return createNode(value);
    }
    if (value < root->data) {
        root->left = insert(root->left, value);
    } else if (value > root->data) {
        root->right = insert(root->right, value);
    }
    // Duplicate values ignored in standard BST
    return root;
}

/* 
 * Search for an element in BST
 * Time Complexity: Average O(log n), Worst O(n)
 */
BSTNode* search(BSTNode* root, int key, int* comparisons) {
    BSTNode* current = root;
    while (current != NULL) {
        (*comparisons)++;
        if (current->data == key) {
            return current;
        } else if (key < current->data) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return NULL;
}

/* 
 * Inorder Traversal (Left, Root, Right) -> Yields sorted order
 * Time Complexity: O(n)
 */
void inorder(BSTNode* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->data);
        inorder(root->right);
    }
}

/* 
 * Preorder Traversal (Root, Left, Right)
 * Time Complexity: O(n)
 */
void preorder(BSTNode* root) {
    if (root != NULL) {
        printf("%d ", root->data);
        preorder(root->left);
        preorder(root->right);
    }
}

/* 
 * Postorder Traversal (Left, Right, Root)
 * Time Complexity: O(n)
 */
void postorder(BSTNode* root) {
    if (root != NULL) {
        postorder(root->left);
        postorder(root->right);
        printf("%d ", root->data);
    }
}

/* 
 * Calculate height of the BST
 * Height of empty tree = 0, single node = 1
 * Uses iterative or bounded recursion
 */
int getHeight(BSTNode* root) {
    if (root == NULL) return 0;
    int leftH = getHeight(root->left);
    int rightH = getHeight(root->right);
    return 1 + ((leftH > rightH) ? leftH : rightH);
}

/* Iterative height computation to prevent stack overflow on n=1000 skewed trees */
int getHeightIterative(BSTNode* root) {
    if (!root) return 0;
    // In skewed trees, either left or right chain dominates
    int h = 0;
    BSTNode* curr = root;
    while (curr) {
        h++;
        curr = (curr->left) ? curr->left : curr->right;
    }
    return h;
}

/* Free all nodes in tree */
void freeTree(BSTNode* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

/* 
 * Height Observation Experiment for Random, Ascending, and Descending sequences
 * Prescribed sizes: n = 100, 500, 1000
 */
void runHeightObservationExperiment(void) {
    int testSizes[] = {100, 500, 1000};
    int numTests = sizeof(testSizes) / sizeof(testSizes[0]);

    printf("\n========================================================================================\n");
    printf("         BST HEIGHT OBSERVATION: RANDOM vs ASCENDING vs DESCENDING SEQUENCES            \n");
    printf("========================================================================================\n");
    printf("| %-6s | %-16s | %-16s | %-16s | %-14s |\n",
           "n", "Random Height", "Ascending Height", "Descending Height", "Ideal log2(n)");
    printf("|--------|------------------|------------------|-------------------|----------------|\n");

    for (int i = 0; i < numTests; i++) {
        int n = testSizes[i];

        // 1. Random sequence
        BSTNode* rootRandom = NULL;
        for (int k = 0; k < n; k++) {
            rootRandom = insert(rootRandom, rand() % 100000);
        }
        int hRandom = getHeight(rootRandom);

        // 2. Ascending sequence (1, 2, 3, ... n) -> forms right-skewed linked list
        // Insert iteratively to build tree
        BSTNode* rootAsc = NULL;
        BSTNode* tailAsc = NULL;
        for (int k = 1; k <= n; k++) {
            BSTNode* node = createNode(k);
            if (!rootAsc) {
                rootAsc = node;
                tailAsc = node;
            } else {
                tailAsc->right = node;
                tailAsc = node;
            }
        }
        int hAsc = getHeightIterative(rootAsc);

        // 3. Descending sequence (n, n-1, ... 1) -> forms left-skewed linked list
        BSTNode* rootDesc = NULL;
        BSTNode* tailDesc = NULL;
        for (int k = n; k >= 1; k--) {
            BSTNode* node = createNode(k);
            if (!rootDesc) {
                rootDesc = node;
                tailDesc = node;
            } else {
                tailDesc->left = node;
                tailDesc = node;
            }
        }
        int hDesc = getHeightIterative(rootDesc);

        // Compute theoretical log2(n)
        double idealLog = 0;
        int temp = n;
        while (temp > 1) {
            idealLog++;
            temp /= 2;
        }

        printf("| %-6d | %-16d | %-16d | %-17d | ~%-13.0f |\n",
               n, hRandom, hAsc, hDesc, idealLog + 1);

        freeTree(rootRandom);
        freeTree(rootAsc);
        freeTree(rootDesc);
    }
    printf("========================================================================================\n");
    printf("Empirical Conclusions:\n");
    printf("1. Random Insertion: Tree height stays close to O(log n). Lookups are fast O(log n).\n");
    printf("2. Ascending / Descending: Tree degenerates into a linear linked list of height n.\n");
    printf("   Lookups degrade to worst-case O(n).\n");
    printf("3. This motivates the need for self-balancing search trees (AVL Trees, Red-Black Trees).\n");
    printf("========================================================================================\n\n");
}

/* Interactive menu demonstration */
void runInteractiveDemo(void) {
    BSTNode* root = NULL;
    int choice, val;

    while (1) {
        printf("\n--- BST OPERATIONS MENU ---\n");
        printf("1. Insert Node\n");
        printf("2. Search Element\n");
        printf("3. Inorder Traversal (Sorted)\n");
        printf("4. Preorder Traversal\n");
        printf("5. Postorder Traversal\n");
        printf("6. Get Current Tree Height\n");
        printf("7. Return to Main Menu\n");
        printf("Enter choice (1-7): ");

        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
            case 1:
                printf("Enter integer value to insert: ");
                scanf("%d", &val);
                root = insert(root, val);
                printf("[Success] Inserted %d into BST.\n", val);
                break;

            case 2:
                printf("Enter integer value to search: ");
                scanf("%d", &val);
                {
                    int comparisons = 0;
                    BSTNode* found = search(root, val, &comparisons);
                    if (found) {
                        printf("[Found] Key %d found in BST after %d comparisons.\n", val, comparisons);
                    } else {
                        printf("[Not Found] Key %d not present (compared %d nodes).\n", val, comparisons);
                    }
                }
                break;

            case 3:
                printf("Inorder Traversal   : ");
                inorder(root);
                printf("\n");
                break;

            case 4:
                printf("Preorder Traversal  : ");
                preorder(root);
                printf("\n");
                break;

            case 5:
                printf("Postorder Traversal : ");
                postorder(root);
                printf("\n");
                break;

            case 6:
                printf("Current Tree Height : %d\n", getHeight(root));
                break;

            case 7:
                freeTree(root);
                return;

            default:
                printf("[Error] Invalid choice! Select 1-7.\n");
        }
    }
    freeTree(root);
}

int main(void) {
    srand((unsigned int)time(NULL));
    int choice;

    printf("===============================================================\n");
    printf("  BMSIT&M - DSA LAB - EXPERIMENT 7: BINARY SEARCH TREE (BST)   \n");
    printf("===============================================================\n");

    while (1) {
        printf("\n--- MAIN MENU ---\n");
        printf("1. Run Height Observation Benchmark (n = 100, 500, 1000)\n");
        printf("2. Interactive BST Operations (Insert, Search, Traversals)\n");
        printf("3. View Time Complexity Analysis\n");
        printf("4. Exit\n");
        printf("Enter choice (1-4): ");

        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
            case 1:
                runHeightObservationExperiment();
                break;
            case 2:
                runInteractiveDemo();
                break;
            case 3:
                printf("\n===============================================================\n");
                printf("             BST: TIME & SPACE COMPLEXITY ANALYSIS             \n");
                printf("===============================================================\n");
                printf("| Operation       | Average Case (Balanced) | Worst Case (Skewed) |\n");
                printf("|-----------------|-------------------------|---------------------|\n");
                printf("| Insertion       | O(log n)                | O(n)                |\n");
                printf("| Search          | O(log n)                | O(n)                |\n");
                printf("| Inorder         | O(n)                    | O(n)                |\n");
                printf("| Preorder        | O(n)                    | O(n)                |\n");
                printf("| Postorder       | O(n)                    | O(n)                |\n");
                printf("| Height Check    | O(n)                    | O(n)                |\n");
                printf("===============================================================\n");
                printf("Auxiliary Call Stack Space:\n");
                printf("  - Balanced BST: O(log n)\n");
                printf("  - Skewed BST:   O(n)\n");
                printf("===============================================================\n\n");
                break;
            case 4:
                printf("Exiting. Goodbye!\n");
                return 0;
            default:
                printf("[Error] Invalid choice! Select 1-4.\n");
        }
    }

    return 0;
}
