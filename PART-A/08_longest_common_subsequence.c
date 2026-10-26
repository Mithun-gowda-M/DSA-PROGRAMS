/**
 * ============================================================================
 * BMS INSTITUTE OF TECHNOLOGY & MANAGEMENT
 * DEPARTMENT OF ARTIFICIAL INTELLIGENCE AND MACHINE LEARNING
 * Course: Data Structures & Algorithms Laboratory (Course Code: 1BAIL306)
 * Semester: III | Scheme: 2025
 * ----------------------------------------------------------------------------
 * PART A - EXPERIMENT 8: Longest Common Subsequence (LCS) using DP
 * 
 * Objective:
 *   Implement the Longest Common Subsequence (LCS) problem using DP.
 *   Given two strings:
 *     1. Find the LCS length.
 *     2. Print the LCS string.
 *     3. Optimize space complexity from O(mn) to O(min(m, n)).
 *     4. Run for string lengths up to 1000.
 *     5. Plot a graph of time taken versus mn.
 *     6. Analyze time and space complexity. (CO2)
 * 
 * Complexities:
 *   - Standard 2D DP: Time O(m*n), Space O(m*n) (allows full string reconstruction)
 *   - Space Optimized: Time O(m*n), Space O(min(m, n)) (length computation)
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

static inline int maxVal(int a, int b) {
    return (a > b) ? a : b;
}

/* 
 * 1. Standard 2D DP Table Implementation
 * Computes LCS length and reconstructs the LCS string
 * Time Complexity: O(m * n)
 * Space Complexity: O(m * n)
 */
int lcsStandard2D(const char* X, const char* Y, char* lcsOut, int printTable) {
    int m = (int)strlen(X);
    int n = (int)strlen(Y);

    // Allocate dp table [m+1][n+1]
    int** dp = (int**)malloc((m + 1) * sizeof(int*));
    for (int i = 0; i <= m; i++) {
        dp[i] = (int*)calloc(n + 1, sizeof(int));
    }

    // Build DP table
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (X[i - 1] == Y[j - 1]) {
                dp[i][j] = 1 + dp[i - 1][j - 1];
            } else {
                dp[i][j] = maxVal(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    int lcsLen = dp[m][n];

    // Optional table visualization for small inputs
    if (printTable && m <= 15 && n <= 15) {
        printf("\n--- 2D DP Matrix ---\n    ");
        for (int j = 0; j < n; j++) printf(" %c ", Y[j]);
        printf("\n");
        for (int i = 0; i <= m; i++) {
            if (i > 0) printf("%c |", X[i - 1]);
            else printf("  |");
            for (int j = 0; j <= n; j++) {
                printf("%2d ", dp[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }

    // Reconstruct LCS string by backtracking from dp[m][n]
    if (lcsOut != NULL) {
        int index = lcsLen;
        lcsOut[index] = '\0';

        int i = m, j = n;
        while (i > 0 && j > 0) {
            if (X[i - 1] == Y[j - 1]) {
                lcsOut[index - 1] = X[i - 1];
                i--;
                j--;
                index--;
            } else if (dp[i - 1][j] > dp[i][j - 1]) {
                i--;
            } else {
                j--;
            }
        }
    }

    for (int i = 0; i <= m; i++) free(dp[i]);
    free(dp);

    return lcsLen;
}

/* 
 * 2. Space-Optimized LCS Length computation
 * Uses 2 rolling rows of size min(m, n) + 1
 * Time Complexity: O(m * n)
 * Space Complexity: O(min(m, n))
 */
int lcsSpaceOptimized(const char* X, const char* Y) {
    const char* str1 = X;
    const char* str2 = Y;
    int len1 = (int)strlen(str1);
    int len2 = (int)strlen(str2);

    // Ensure str2 is the shorter string to minimize space
    if (len1 < len2) {
        const char* temp = str1;
        str1 = str2;
        str2 = temp;
        int t = len1;
        len1 = len2;
        len2 = t;
    }

    // We only need two rows of size (len2 + 1)
    int* prevRow = (int*)calloc(len2 + 1, sizeof(int));
    int* currRow = (int*)calloc(len2 + 1, sizeof(int));

    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                currRow[j] = 1 + prevRow[j - 1];
            } else {
                currRow[j] = maxVal(prevRow[j], currRow[j - 1]);
            }
        }
        // Copy current row to previous row
        memcpy(prevRow, currRow, (len2 + 1) * sizeof(int));
    }

    int lcsLen = currRow[len2];
    free(prevRow);
    free(currRow);

    return lcsLen;
}

/* Generate random DNA-like sequence of given length */
void generateRandomString(char* str, int length) {
    const char alphabet[] = "ACGT";
    for (int i = 0; i < length; i++) {
        str[i] = alphabet[rand() % 4];
    }
    str[length] = '\0';
}

/* Plot ASCII graph of Time vs (m * n) */
void plotAsciiGraph(const long long mnList[], const double times[], int count) {
    printf("\n===============================================================\n");
    printf("         ASCII PLOT: EXECUTION TIME (ms) vs (m * n)            \n");
    printf("===============================================================\n");

    double maxTime = times[0];
    for (int i = 1; i < count; i++) {
        if (times[i] > maxTime) maxTime = times[i];
    }
    if (maxTime <= 0) maxTime = 1.0;

    for (int i = 0; i < count; i++) {
        int barLen = (int)((times[i] / maxTime) * 45);
        if (barLen < 1 && times[i] > 0) barLen = 1;

        printf("m*n = %9lld | ", mnList[i]);
        for (int b = 0; b < barLen; b++) printf("*");
        printf(" (%.3f ms)\n", times[i] * 1000.0);
    }
    printf("---------------------------------------------------------------\n");
    printf("Graph demonstrates linear proportionality to (m * n).\n");
    printf("===============================================================\n\n");
}

/* 
 * Run benchmark for string lengths up to 1000
 * Tested sizes: 100, 250, 500, 1000
 */
void runLcsBenchmark(void) {
    int testLengths[] = {100, 250, 500, 1000};
    int count = sizeof(testLengths) / sizeof(testLengths[0]);
    long long mnList[4];
    double times2D[4];
    double timesOpt[4];

    printf("\n========================================================================================\n");
    printf("      LCS BENCHMARK: STRING LENGTHS UP TO 1000 (m = n = 100, 250, 500, 1000)            \n");
    printf("========================================================================================\n");
    printf("| %-6s | %-6s | %-12s | %-16s | %-16s | %-8s |\n",
           "m", "n", "m * n", "2D DP Time (ms)", "1D DP Time (ms)", "LCS Len");
    printf("|--------|--------|--------------|------------------|------------------|----------|\n");

    for (int i = 0; i < count; i++) {
        int len = testLengths[i];
        char* str1 = (char*)malloc(len + 1);
        char* str2 = (char*)malloc(len + 1);

        generateRandomString(str1, len);
        generateRandomString(str2, len);

        mnList[i] = (long long)len * len;

        // Measure 2D DP
        double t1 = get_time_seconds();
        int len2D = lcsStandard2D(str1, str2, NULL, 0);
        double t2 = get_time_seconds();
        times2D[i] = t2 - t1;

        // Measure Space Optimized DP
        double t3 = get_time_seconds();
        int lenOpt = lcsSpaceOptimized(str1, str2);
        double t4 = get_time_seconds();
        timesOpt[i] = t4 - t3;

        printf("| %-6d | %-6d | %-12lld | %-16.3f | %-16.3f | %-8d |\n",
               len, len, mnList[i], times2D[i] * 1000.0, timesOpt[i] * 1000.0, len2D);
        (void)lenOpt; // Explicitly silence unused variable in benchmark loop

        free(str1);
        free(str2);
    }
    printf("========================================================================================\n");

    plotAsciiGraph(mnList, times2D, count);
}

/* Interactive mode */
void runInteractive(void) {
    char str1[256];
    char str2[256];
    char lcsString[256];

    printf("\nEnter String 1 (e.g. LONGEST or AGGTAB): ");
    if (scanf("%255s", str1) != 1) return;

    printf("Enter String 2 (e.g. STONE or GXTXAYB): ");
    if (scanf("%255s", str2) != 1) return;

    int len2D = lcsStandard2D(str1, str2, lcsString, 1);
    int lenOpt = lcsSpaceOptimized(str1, str2);

    printf("[Result]\n");
    printf("  String 1      : %s (Length: %zu)\n", str1, strlen(str1));
    printf("  String 2      : %s (Length: %zu)\n", str2, strlen(str2));
    printf("  LCS Length    : %d\n", len2D);
    printf("  LCS Subsequence: %s\n", lcsString);
    printf("  Space-Optimized Length Check: %d\n\n", lenOpt);
}

int main(void) {
    srand((unsigned int)time(NULL));
    int choice;

    printf("===============================================================\n");
    printf("  BMSIT&M - DSA LAB - EXPERIMENT 8: LONGEST COMMON SUBSEQUENCE \n");
    printf("===============================================================\n");

    while (1) {
        printf("\n--- MENU ---\n");
        printf("1. Run Benchmark up to Length 1000 & Plot Graph Time vs (m*n)\n");
        printf("2. Interactive LCS Solver (Print LCS Subsequence)\n");
        printf("3. View Time and Space Complexity Analysis\n");
        printf("4. Exit\n");
        printf("Enter choice (1-4): ");

        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
            case 1:
                runLcsBenchmark();
                break;
            case 2:
                runInteractive();
                break;
            case 3:
                printf("\n===============================================================\n");
                printf("                   LCS: COMPLEXITY ANALYSIS                    \n");
                printf("===============================================================\n");
                printf("Time Complexity:\n");
                printf("  - Matrix construction: m * n cell computations.\n");
                printf("  - Each cell takes O(1) comparison and max operation.\n");
                printf("  - Backtracking string reconstruction: at most (m + n) steps.\n");
                printf("  - Total Time Complexity: O(m * n)\n\n");
                printf("Space Complexity:\n");
                printf("  - Full 2D Table: (m + 1) * (n + 1) integers -> O(m * n).\n");
                printf("    (Required if the actual LCS string needs to be reconstructed)\n");
                printf("  - Space-Optimized Rolling Rows: 2 * (min(m, n) + 1) integers\n");
                printf("    -> O(min(m, n)).\n");
                printf("    Massive space savings: For m=1000, n=1000,\n");
                printf("    2D table needs 1,000,000 ints (~4MB), optimized needs only ~2000 ints (~8KB)!\n");
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
