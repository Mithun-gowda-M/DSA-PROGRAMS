/**
 * ============================================================================
 * BMS INSTITUTE OF TECHNOLOGY & MANAGEMENT
 * DEPARTMENT OF ARTIFICIAL INTELLIGENCE AND MACHINE LEARNING
 * Course: Data Structures & Algorithms Laboratory (Course Code: 1BAIL306)
 * Semester: III | Scheme: 2025
 * ----------------------------------------------------------------------------
 * PART A - EXPERIMENT 4: Merge Sort - Implementation and Complexity Analysis
 * 
 * Objective:
 *   Sort n random integers using Merge Sort algorithm.
 *   Run the program for n = 5000, 10000, 20000, 50000.
 *   Record the time taken to sort.
 *   Plot a graph of time taken versus n.
 *   Analyze and compare the worst-case, average-case, and best-case
 *   time complexities. (CO2)
 * 
 * Theoretical Complexity:
 *   - Recurrence: T(n) = 2T(n/2) + Theta(n)
 *   - Best Case:    O(n log n)
 *   - Average Case: O(n log n)
 *   - Worst Case:   O(n log n)
 *   - Space Complexity: O(n) auxiliary array for merging.
 *   - Stability: Stable sort
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
/* High-resolution timer for Windows */
double get_time_seconds(void) {
    LARGE_INTEGER freq, count;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&count);
    return (double)count.QuadPart / (double)freq.QuadPart;
}
#else
/* High-resolution timer for POSIX */
double get_time_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}
#endif

/* 
 * Merge two sorted subarrays: arr[left..mid] and arr[mid+1..right]
 * Time Complexity: O(n)
 * Space Complexity: O(n) auxiliary
 */
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Allocate temporary buffers
    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));

    if (L == NULL || R == NULL) {
        printf("[Error] Memory allocation failed during merge step!\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n1; i++) {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) {
        arr[k++] = L[i++];
    }

    while (j < n2) {
        arr[k++] = R[j++];
    }

    free(L);
    free(R);
}

/* 
 * Recursive Merge Sort algorithm (Divide and Conquer)
 * Time Complexity: O(n log n) in all cases
 */
void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

/* Helper to verify if an array is sorted correctly */
int isSorted(const int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
    }
    return 1;
}

/* Fill array with pseudo-random integers */
void generateRandomArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000000;
    }
}

/* Render an ASCII graph of time vs n in console */
void plotAsciiGraph(const int sizes[], const double times[], int count) {
    printf("\n===============================================================\n");
    printf("         ASCII PLOT: EXECUTION TIME (ms) vs INPUT SIZE (n)     \n");
    printf("===============================================================\n");

    double maxTime = times[0];
    for (int i = 1; i < count; i++) {
        if (times[i] > maxTime) maxTime = times[i];
    }
    if (maxTime <= 0.0) maxTime = 1.0;

    int maxBarWidth = 45;

    for (int i = 0; i < count; i++) {
        double time_ms = times[i] * 1000.0;
        int barLength = (int)((times[i] / maxTime) * maxBarWidth);
        if (barLength < 1 && times[i] > 0) barLength = 1;

        printf("n = %6d | ", sizes[i]);
        for (int b = 0; b < barLength; b++) {
            printf("*");
        }
        printf(" (%.3f ms)\n", time_ms);
    }
    printf("---------------------------------------------------------------\n");
    printf("Notice the nearly linear-logarithmic growth O(n log n).\n");
    printf("===============================================================\n\n");
}

/* Run the prescribed benchmark for n = 5000, 10000, 20000, 50000 */
void runPrescribedBenchmark(void) {
    int testSizes[] = {5000, 10000, 20000, 50000};
    int numTests = sizeof(testSizes) / sizeof(testSizes[0]);
    double measuredTimes[4];

    printf("\n===============================================================\n");
    printf("    MERGE SORT BENCHMARK: n = 5000, 10000, 20000, 50000       \n");
    printf("===============================================================\n");
    printf("| %-8s | %-16s | %-16s | %-10s |\n", "n", "Time (seconds)", "Time (ms)", "Status");
    printf("|----------|------------------|------------------|------------|\n");

    for (int i = 0; i < numTests; i++) {
        int n = testSizes[i];
        int* arr = (int*)malloc(n * sizeof(int));
        if (arr == NULL) {
            printf("[Error] Cannot allocate array of size %d\n", n);
            continue;
        }

        generateRandomArray(arr, n);

        double startTime = get_time_seconds();
        mergeSort(arr, 0, n - 1);
        double endTime = get_time_seconds();

        measuredTimes[i] = endTime - startTime;
        int sorted = isSorted(arr, n);

        printf("| %-8d | %-16.6f | %-16.3f | %-10s |\n",
               n, measuredTimes[i], measuredTimes[i] * 1000.0,
               sorted ? "VERIFIED" : "FAILED");

        free(arr);
    }
    printf("===============================================================\n");

    // Display ASCII Graph
    plotAsciiGraph(testSizes, measuredTimes, numTests);
}

/* Interactive Demonstration with small custom array */
void runInteractiveDemo(void) {
    int n;
    printf("\nEnter number of elements to sort (e.g. 5 to 20): ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("[Error] Invalid input.\n");
        return;
    }

    int* arr = (int*)malloc(n * sizeof(int));
    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++) {
        printf("  arr[%d]: ", i);
        scanf("%d", &arr[i]);
    }

    printf("\nOriginal Array : ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");

    mergeSort(arr, 0, n - 1);

    printf("Sorted Array   : ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");

    free(arr);
}

/* Detailed Complexity Analysis Display */
void displayAnalysis(void) {
    printf("\n========================================================================\n");
    printf("                   MERGE SORT: COMPLEXITY ANALYSIS                     \n");
    printf("========================================================================\n");
    printf("1. Recurrence Relation:\n");
    printf("   T(n) = 2 * T(n/2) + c*n,  with T(1) = O(1)\n");
    printf("   By Master Theorem (Case 2: a = 2, b = 2, f(n) = n):\n");
    printf("   n^(log_b a) = n^(log_2 2) = n^1 = Theta(n)\n");
    printf("   Therefore: T(n) = Theta(n * log_2 n)\n\n");
    printf("2. Case-by-Case Breakdown:\n");
    printf("   - Best Case Time    : O(n log n) (Array already sorted still splits)\n");
    printf("   - Average Case Time : O(n log n) (Random distributions)\n");
    printf("   - Worst Case Time   : O(n log n) (Reverse sorted or alternating)\n");
    printf("   - Space Complexity  : O(n) auxiliary for scratch merge buffers\n");
    printf("   - Stability         : STABLE (Preserves original order of duplicates)\n\n");
    printf("3. Comparison Insight:\n");
    printf("   Unlike Quick Sort whose worst-case degenerates to O(n^2),\n");
    printf("   Merge Sort GUARANTEES O(n log n) even in the worst imaginable case!\n");
    printf("========================================================================\n\n");
}

int main(void) {
    srand((unsigned int)time(NULL));
    int choice;

    printf("===============================================================\n");
    printf("  BMSIT&M - DSA LAB - EXPERIMENT 4: MERGE SORT & ANALYSIS      \n");
    printf("===============================================================\n");

    while (1) {
        printf("\n--- MENU ---\n");
        printf("1. Run Prescribed Benchmark (n = 5000, 10000, 20000, 50000) & Plot Graph\n");
        printf("2. Run Interactive Merge Sort (Small Array)\n");
        printf("3. View Detailed Theoretical Complexity Analysis\n");
        printf("4. Exit\n");
        printf("Enter your choice (1-4): ");

        if (scanf("%d", &choice) != 1) {
            printf("[Error] Invalid input. Exiting.\n");
            break;
        }

        switch (choice) {
            case 1:
                runPrescribedBenchmark();
                break;
            case 2:
                runInteractiveDemo();
                break;
            case 3:
                displayAnalysis();
                break;
            case 4:
                printf("Exiting program. Goodbye!\n");
                return 0;
            default:
                printf("[Error] Invalid choice! Select 1-4.\n");
        }
    }
    return 0;
}
