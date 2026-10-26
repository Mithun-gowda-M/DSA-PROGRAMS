/**
 * ============================================================================
 * BMS INSTITUTE OF TECHNOLOGY & MANAGEMENT
 * DEPARTMENT OF ARTIFICIAL INTELLIGENCE AND MACHINE LEARNING
 * Course: Data Structures & Algorithms Laboratory (Course Code: 1BAIL306)
 * Semester: III | Scheme: 2025
 * ----------------------------------------------------------------------------
 * PART A - EXPERIMENT 6: 0/1 Knapsack using Dynamic Programming
 * 
 * Objective:
 *   Implement the 0/1 Knapsack problem using dynamic programming.
 *   Given n items with weights and values, and a knapsack capacity W,
 *   find the maximum total value.
 *   1. Implement using a 2D DP table
 *   2. Optimize space to a 1D array
 *   3. Print the selected items
 *   4. Run for n = 10, 20, 50 and different W values
 *   5. Analyze time complexity O(nW) and space complexity O(W)
 *   6. Compare the DP approach with the greedy approach and explain why greedy fails. (CO2)
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
double get_time_seconds(void) {
    LARGE_INTEGER freq, count;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&count);
    return (double)count.QuadPart / (double)freq.QuadPart;
}
#else
double get_time_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}
#endif

typedef struct {
    int id;
    int weight;
    int value;
    double ratio;
} Item;

int maxVal(int a, int b) {
    return (a > b) ? a : b;
}

/* 
 * 1. 2D Dynamic Programming Implementation with Item Reconstruction
 * Time Complexity: O(n * W)
 * Space Complexity: O(n * W)
 */
int knapsack2D(const Item items[], int n, int W, int printSelection) {
    // Allocate 2D table dp[n+1][W+1]
    int** dp = (int**)malloc((n + 1) * sizeof(int*));
    for (int i = 0; i <= n; i++) {
        dp[i] = (int*)calloc(W + 1, sizeof(int));
    }

    // Fill DP table
    for (int i = 1; i <= n; i++) {
        int wt = items[i - 1].weight;
        int val = items[i - 1].value;

        for (int w = 0; w <= W; w++) {
            if (wt <= w) {
                dp[i][w] = maxVal(dp[i - 1][w], val + dp[i - 1][w - wt]);
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    int maxProfit = dp[n][W];

    // Backtrack to identify selected items
    if (printSelection) {
        printf("\n--- Selected Items for Maximum Profit ---\n");
        printf("| %-8s | %-10s | %-10s |\n", "Item ID", "Weight", "Value");
        printf("|----------|------------|------------|\n");

        int currentW = W;
        int totalSelectedWeight = 0;
        int selectedCount = 0;

        for (int i = n; i > 0 && maxProfit > 0; i--) {
            // If value is different from upper row, this item was included
            if (dp[i][currentW] != dp[i - 1][currentW]) {
                printf("| %-8d | %-10d | %-10d |\n",
                       items[i - 1].id, items[i - 1].weight, items[i - 1].value);
                totalSelectedWeight += items[i - 1].weight;
                currentW -= items[i - 1].weight;
                selectedCount++;
            }
        }
        printf("|----------|------------|------------|\n");
        printf("Total Selected Items: %d\n", selectedCount);
        printf("Total Weight Used   : %d / %d\n", totalSelectedWeight, W);
        printf("Maximum Profit      : %d\n\n", dp[n][W]);
    }

    for (int i = 0; i <= n; i++) {
        free(dp[i]);
    }
    free(dp);

    return maxProfit;
}

/* 
 * 2. 1D Space-Optimized Dynamic Programming Implementation
 * Time Complexity: O(n * W)
 * Space Complexity: O(W)
 */
int knapsack1DOptimized(const Item items[], int n, int W) {
    int* dp = (int*)calloc(W + 1, sizeof(int));
    if (!dp) return -1;

    for (int i = 0; i < n; i++) {
        int wt = items[i].weight;
        int val = items[i].value;

        // Traverse backwards to avoid using the current item multiple times
        for (int w = W; w >= wt; w--) {
            dp[w] = maxVal(dp[w], val + dp[w - wt]);
        }
    }

    int result = dp[W];
    free(dp);
    return result;
}

/* Compare item by value-to-weight ratio for greedy algorithm */
int compareRatio(const void* a, const void* b) {
    const Item* itemA = (const Item*)a;
    const Item* itemB = (const Item*)b;
    if (itemB->ratio > itemA->ratio) return 1;
    if (itemB->ratio < itemA->ratio) return -1;
    return 0;
}

/* 
 * 3. Greedy Approach (for 0/1 Knapsack) to show why it fails
 */
int knapsackGreedy(Item items[], int n, int W) {
    Item* copy = (Item*)malloc(n * sizeof(Item));
    for (int i = 0; i < n; i++) {
        copy[i] = items[i];
        copy[i].ratio = (double)copy[i].value / copy[i].weight;
    }

    qsort(copy, n, sizeof(Item), compareRatio);

    int totalWeight = 0;
    int totalValue = 0;

    for (int i = 0; i < n; i++) {
        if (totalWeight + copy[i].weight <= W) {
            totalWeight += copy[i].weight;
            totalValue += copy[i].value;
        }
    }

    free(copy);
    return totalValue;
}

/* Demonstration of why greedy fails for 0/1 Knapsack */
void demonstrateWhyGreedyFails(void) {
    printf("\n========================================================================\n");
    printf("   PROOF & COUNTEREXAMPLE: WHY THE GREEDY APPROACH FAILS FOR 0/1 KNAPSACK\n");
    printf("========================================================================\n");
    printf("Consider Knapsack Capacity W = 50 and 3 items:\n");
    printf("  Item 1: Weight = 10, Value = 60  -> Ratio = 6.0\n");
    printf("  Item 2: Weight = 20, Value = 100 -> Ratio = 5.0\n");
    printf("  Item 3: Weight = 30, Value = 120 -> Ratio = 4.0\n\n");

    Item sampleItems[3] = {
        {1, 10, 60, 6.0},
        {2, 20, 100, 5.0},
        {3, 30, 120, 4.0}
    };
    int W = 50;

    int greedyVal = knapsackGreedy(sampleItems, 3, W);
    int optimalVal = knapsack2D(sampleItems, 3, W, 0);

    printf("1. Greedy by Ratio Strategy:\n");
    printf("   - Picks Item 1 (Weight 10, Value 60). Remaining capacity = 40.\n");
    printf("   - Picks Item 2 (Weight 20, Value 100). Remaining capacity = 20.\n");
    printf("   - Cannot pick Item 3 (Weight 30 > 20 remaining).\n");
    printf("   -> Greedy Total Value = %d (Weight used: 30/50, wasted capacity: 20)\n\n", greedyVal);

    printf("2. Dynamic Programming Optimal Strategy:\n");
    printf("   - Skips Item 1.\n");
    printf("   - Picks Item 2 (Weight 20, Value 100).\n");
    printf("   - Picks Item 3 (Weight 30, Value 120).\n");
    printf("   -> DP Total Value = %d (Weight used: 50/50, optimal utilization!)\n\n", optimalVal);

    printf("Analysis:\n");
    printf("  Greedy fails because in 0/1 Knapsack, items cannot be broken into fractions.\n");
    printf("  Choosing higher ratio items may leave empty capacity that cannot fit other\n");
    printf("  valuable items. Dynamic Programming evaluates all subproblems to find the\n");
    printf("  global optimum.\n");
    printf("========================================================================\n\n");
}

/* Run benchmark for n = 10, 20, 50 with varying capacities W */
void runKnapsackBenchmark(void) {
    int testN[] = {10, 20, 50};
    int testW[] = {50, 100, 200};
    int countN = sizeof(testN) / sizeof(testN[0]);
    int countW = sizeof(testW) / sizeof(testW[0]);

    printf("\n========================================================================================\n");
    printf("     0/1 KNAPSACK BENCHMARK: n = 10, 20, 50 across Capacities W = 50, 100, 200          \n");
    printf("========================================================================================\n");
    printf("| %-4s | %-5s | %-15s | %-15s | %-12s | %-12s |\n",
           "n", "W", "2D DP Time (ms)", "1D DP Time (ms)", "2D Max Val", "1D Max Val");
    printf("|------|-------|-----------------|-----------------|--------------|--------------|\n");

    for (int i = 0; i < countN; i++) {
        int n = testN[i];
        Item* items = (Item*)malloc(n * sizeof(Item));
        for (int k = 0; k < n; k++) {
            items[k].id = k + 1;
            items[k].weight = 5 + (rand() % 30);
            items[k].value = 10 + (rand() % 100);
            items[k].ratio = (double)items[k].value / items[k].weight;
        }

        for (int j = 0; j < countW; j++) {
            int W = testW[j];

            double t1 = get_time_seconds();
            int res2D = knapsack2D(items, n, W, 0);
            double t2 = get_time_seconds();
            double time2D = (t2 - t1) * 1000.0;

            double t3 = get_time_seconds();
            int res1D = knapsack1DOptimized(items, n, W);
            double t4 = get_time_seconds();
            double time1D = (t4 - t3) * 1000.0;

            printf("| %-4d | %-5d | %-15.4f | %-15.4f | %-12d | %-12d |\n",
                   n, W, time2D, time1D, res2D, res1D);
        }
        printf("|------|-------|-----------------|-----------------|--------------|--------------|\n");
        free(items);
    }
    printf("\nTime Complexity: O(n * W) operations.\n");
    printf("Space Complexity: 2D table uses O(n * W) space, 1D array uses O(W) space.\n");
    printf("========================================================================================\n\n");
}

/* Interactive mode */
void runInteractive(void) {
    int n, W;
    printf("\nEnter number of items (n): ");
    if (scanf("%d", &n) != 1 || n <= 0) return;

    printf("Enter knapsack capacity (W): ");
    if (scanf("%d", &W) != 1 || W <= 0) return;

    Item* items = (Item*)malloc(n * sizeof(Item));
    printf("Enter weight and value for each item:\n");
    for (int i = 0; i < n; i++) {
        items[i].id = i + 1;
        printf("  Item %d (Weight Value): ", i + 1);
        scanf("%d %d", &items[i].weight, &items[i].value);
        items[i].ratio = (double)items[i].value / items[i].weight;
    }

    printf("\n--- 2D Dynamic Programming with Item Selection ---");
    knapsack2D(items, n, W, 1);

    int val1D = knapsack1DOptimized(items, n, W);
    printf("Result from 1D Space-Optimized DP: %d\n", val1D);

    int valGreedy = knapsackGreedy(items, n, W);
    printf("Result from Greedy (Value/Weight ratio): %d\n", valGreedy);

    free(items);
}

int main(void) {
    srand((unsigned int)time(NULL));
    int choice;

    printf("===============================================================\n");
    printf("    BMSIT&M - DSA LAB - EXPERIMENT 6: 0/1 KNAPSACK (DP)        \n");
    printf("===============================================================\n");

    while (1) {
        printf("\n--- MENU ---\n");
        printf("1. Run Benchmark (n = 10, 20, 50 and W = 50, 100, 200)\n");
        printf("2. Demonstrate Why Greedy Fails for 0/1 Knapsack\n");
        printf("3. Interactive Knapsack Solver (Custom Items & Capacity)\n");
        printf("4. View Complexity Analysis (Time O(nW), Space O(W))\n");
        printf("5. Exit\n");
        printf("Enter choice (1-5): ");

        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
            case 1:
                runKnapsackBenchmark();
                break;
            case 2:
                demonstrateWhyGreedyFails();
                break;
            case 3:
                runInteractive();
                break;
            case 4:
                printf("\n===============================================================\n");
                printf("                0/1 KNAPSACK: COMPLEXITY ANALYSIS             \n");
                printf("===============================================================\n");
                printf("Time Complexity:\n");
                printf("  - 2D Table filling: Nested loops run n * W times -> O(n * W).\n");
                printf("  - 1D Array filling: Inner loop runs W down to wt -> O(n * W).\n");
                printf("  - Item Backtracking: At most n steps -> O(n).\n");
                printf("  - Overall Time Complexity: O(n * W) (Pseudo-polynomial time).\n\n");
                printf("Space Complexity:\n");
                printf("  - Standard 2D Table: Matrix of size (n + 1) * (W + 1) -> O(n * W).\n");
                printf("  - Optimized 1D Array: Single buffer of size (W + 1) -> O(W).\n");
                printf("    Space is reduced by a factor of n!\n");
                printf("===============================================================\n\n");
                break;
            case 5:
                printf("Exiting. Goodbye!\n");
                return 0;
            default:
                printf("[Error] Invalid choice! Select 1-5.\n");
        }
    }
    return 0;
}
