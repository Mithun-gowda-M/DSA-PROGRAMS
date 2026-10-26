/**
 * ============================================================================
 * BMS INSTITUTE OF TECHNOLOGY & MANAGEMENT
 * DEPARTMENT OF ARTIFICIAL INTELLIGENCE AND MACHINE LEARNING
 * Course: Data Structures & Algorithms Laboratory (Course Code: 1BAIL306)
 * Semester: III | Scheme: 2025
 * ----------------------------------------------------------------------------
 * PART A - EXPERIMENT 5: Quick Sort - Implementation and Complexity Analysis
 * 
 * Objective:
 *   Sort n random integers using Quick Sort algorithm with in-place
 *   partitioning (last element as pivot).
 *   Run for n = 5000, 10000, 20000, 50000. Record the time taken.
 *   Plot a graph of time versus n.
 *   Compare Quick Sort performance with Merge Sort for:
 *     1. Random inputs
 *     2. Already sorted inputs
 *     3. Reverse sorted inputs
 *   Analyze worst-case, average-case, and best-case complexities. (CO2)
 * 
 * Theoretical Complexities:
 *   Quick Sort (Last element as pivot):
 *     - Best Case:    O(n log n)  [Pivot splits array into equal halves]
 *     - Average Case: O(n log n)  [Random permutations]
 *     - Worst Case:   O(n^2)      [Already sorted / Reverse sorted array]
 *     - Auxiliary Space: O(log n) call stack average, O(n) worst case
 *   Merge Sort:
 *     - All Cases:    O(n log n)  [Always splits exactly in half]
 *     - Auxiliary Space: O(n)
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

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

/* Swap utility */
static inline void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* 
 * In-place partitioning using LAST ELEMENT AS PIVOT (Lomuto Partition Scheme)
 */
int partition(int arr[], int low, int high) {
    int pivot = arr[high]; // Last element as pivot
    int i = low - 1;       // Index of smaller element

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

/* 
 * Quick Sort with tail-call optimization to protect against deep recursion stack overflow
 */
void quickSort(int arr[], int low, int high) {
    while (low < high) {
        int pi = partition(arr, low, high);

        // Recur on the smaller subarray first to bound call stack to O(log n)
        if (pi - low < high - pi) {
            quickSort(arr, low, pi - 1);
            low = pi + 1;
        } else {
            quickSort(arr, pi + 1, high);
            high = pi - 1;
        }
    }
}

/* --- Merge Sort implementation for comparison --- */
void mergeSubarrays(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));

    if (!L || !R) {
        printf("[Error] Merge allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    free(L);
    free(R);
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        mergeSubarrays(arr, left, mid, right);
    }
}

/* Array generation utilities */
void fillRandom(int arr[], int n) {
    for (int i = 0; i < n; i++) arr[i] = rand() % 1000000;
}

void fillSorted(int arr[], int n) {
    for (int i = 0; i < n; i++) arr[i] = i * 2;
}

void fillReverse(int arr[], int n) {
    for (int i = 0; i < n; i++) arr[i] = (n - i) * 2;
}

/* ASCII Graph Plotter */
void plotAsciiGraph(const int sizes[], const double times[], int count, const char* title) {
    printf("\n--- ASCII PLOT: %s ---\n", title);
    double maxTime = times[0];
    for (int i = 1; i < count; i++) {
        if (times[i] > maxTime) maxTime = times[i];
    }
    if (maxTime <= 0.0) maxTime = 1.0;

    for (int i = 0; i < count; i++) {
        int barLen = (int)((times[i] / maxTime) * 45);
        if (barLen < 1 && times[i] > 0) barLen = 1;
        printf("n = %6d | ", sizes[i]);
        for (int b = 0; b < barLen; b++) printf("#");
        printf(" (%.3f ms)\n", times[i] * 1000.0);
    }
    printf("----------------------------------------------------\n\n");
}

/* 1. Prescribed Benchmark: Quick Sort on Random Integers */
void runRandomBenchmark(void) {
    int testSizes[] = {5000, 10000, 20000, 50000};
    int count = sizeof(testSizes) / sizeof(testSizes[0]);
    double times[4];

    printf("\n===============================================================\n");
    printf("   QUICK SORT BENCHMARK ON RANDOM DATA: n = 5k, 10k, 20k, 50k \n");
    printf("===============================================================\n");
    printf("| %-8s | %-16s | %-16s |\n", "n", "Time (seconds)", "Time (ms)");
    printf("|----------|------------------|------------------|\n");

    for (int i = 0; i < count; i++) {
        int n = testSizes[i];
        int* arr = (int*)malloc(n * sizeof(int));
        fillRandom(arr, n);

        double start = get_time_seconds();
        quickSort(arr, 0, n - 1);
        double end = get_time_seconds();

        times[i] = end - start;
        printf("| %-8d | %-16.6f | %-16.3f |\n", n, times[i], times[i] * 1000.0);
        free(arr);
    }
    printf("===============================================================\n");
    plotAsciiGraph(testSizes, times, count, "Quick Sort Execution Time vs n (Random)");
}

/* 2. Comparative Analysis: Quick Sort vs Merge Sort across 3 input types */
void runComprehensiveComparison(void) {
    int testSizes[] = {5000, 10000, 20000};
    int numSizes = sizeof(testSizes) / sizeof(testSizes[0]);

    printf("\n========================================================================================\n");
    printf("     COMPREHENSIVE COMPARISON: QUICK SORT vs MERGE SORT ACROSS INPUT DISTRIBUTIONS      \n");
    printf("========================================================================================\n");
    printf("| %-6s | %-15s | %-18s | %-18s | %-14s |\n",
           "n", "Input Type", "Quick Sort (ms)", "Merge Sort (ms)", "Faster Sort");
    printf("|--------|-----------------|--------------------|--------------------|----------------|\n");

    const char* inputTypes[] = {"Random", "Already Sorted", "Reverse Sorted"};

    for (int t = 0; t < 3; t++) {
        for (int s = 0; s < numSizes; s++) {
            int n = testSizes[s];
            int* qArr = (int*)malloc(n * sizeof(int));
            int* mArr = (int*)malloc(n * sizeof(int));

            if (t == 0) {
                fillRandom(qArr, n);
            } else if (t == 1) {
                fillSorted(qArr, n);
            } else {
                fillReverse(qArr, n);
            }
            memcpy(mArr, qArr, n * sizeof(int));

            // Benchmark Quick Sort
            double qStart = get_time_seconds();
            quickSort(qArr, 0, n - 1);
            double qEnd = get_time_seconds();
            double qTimeMs = (qEnd - qStart) * 1000.0;

            // Benchmark Merge Sort
            double mStart = get_time_seconds();
            mergeSort(mArr, 0, n - 1);
            double mEnd = get_time_seconds();
            double mTimeMs = (mEnd - mStart) * 1000.0;

            const char* winner = (qTimeMs < mTimeMs) ? "Quick Sort" : "Merge Sort";

            printf("| %-6d | %-15s | %-18.3f | %-18.3f | %-14s |\n",
                   n, inputTypes[t], qTimeMs, mTimeMs, winner);

            free(qArr);
            free(mArr);
        }
        printf("|--------|-----------------|--------------------|--------------------|----------------|\n");
    }

    printf("\nKey Empirical Insights:\n");
    printf("1. Random Data: Quick Sort is typically faster than Merge Sort due to smaller\n");
    printf("   constant factors, cache locality, and lack of auxiliary array copying.\n");
    printf("2. Already Sorted / Reverse Sorted: With last element as pivot, Quick Sort suffers\n");
    printf("   worst-case partitioning O(n^2), causing dramatic slowdown compared to Merge Sort's\n");
    printf("   steady O(n log n) performance!\n");
    printf("========================================================================================\n\n");
}

/* Interactive Quick Sort with user array */
void runInteractiveDemo(void) {
    int n;
    printf("\nEnter number of elements (e.g. 5 to 20): ");
    if (scanf("%d", &n) != 1 || n <= 0) return;

    int* arr = (int*)malloc(n * sizeof(int));
    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++) scanf("%d", &arr[i]);

    printf("\nOriginal Array : ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");

    quickSort(arr, 0, n - 1);

    printf("Sorted Array   : ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");

    free(arr);
}

int main(void) {
    srand((unsigned int)time(NULL));
    int choice;

    printf("===============================================================\n");
    printf("  BMSIT&M - DSA LAB - EXPERIMENT 5: QUICK SORT & ANALYSIS      \n");
    printf("===============================================================\n");

    while (1) {
        printf("\n--- MENU ---\n");
        printf("1. Run Quick Sort Benchmark (5k, 10k, 20k, 50k) & Plot Graph\n");
        printf("2. Compare Quick Sort vs Merge Sort (Random, Sorted, Reverse)\n");
        printf("3. Interactive Quick Sort (Small Array)\n");
        printf("4. View Theoretical Complexity Analysis\n");
        printf("5. Exit\n");
        printf("Enter choice (1-5): ");

        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
            case 1:
                runRandomBenchmark();
                break;
            case 2:
                runComprehensiveComparison();
                break;
            case 3:
                runInteractiveDemo();
                break;
            case 4:
                printf("\n====================================================================\n");
                printf("                QUICK SORT: THEORETICAL ANALYSIS                   \n");
                printf("====================================================================\n");
                printf("Pivot Strategy: Lomuto Partitioning (Last Element as Pivot)\n");
                printf("Recurrence Relations:\n");
                printf("  - Best Case: T(n) = 2T(n/2) + O(n) => O(n log n)\n");
                printf("    Occurs when pivot always splits array into equal subproblems.\n");
                printf("  - Worst Case: T(n) = T(n-1) + O(n) => O(n^2)\n");
                printf("    Occurs on already sorted or reverse sorted arrays when last element\n");
                printf("    is chosen as pivot, producing partitions of size 0 and n - 1.\n");
                printf("  - Average Case: O(n log n) with a tight constant factor ~1.39 n ln n\n\n");
                printf("Comparison with Merge Sort:\n");
                printf("  - Merge Sort is O(n log n) guaranteed, but uses O(n) auxiliary space.\n");
                printf("  - Quick Sort sorts IN-PLACE (O(log n) stack space), but worst case is O(n^2).\n");
                printf("====================================================================\n\n");
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
