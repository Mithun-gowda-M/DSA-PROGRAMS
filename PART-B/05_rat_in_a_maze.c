/**
 * ============================================================================
 * BMS INSTITUTE OF TECHNOLOGY & MANAGEMENT
 * DEPARTMENT OF ARTIFICIAL INTELLIGENCE AND MACHINE LEARNING
 * Course: Data Structures & Algorithms Laboratory (Course Code: 1BAIL306)
 * Semester: III | Scheme: 2025
 * ----------------------------------------------------------------------------
 * PART B - OPEN ENDED EXPERIMENT 5: Rat in a Maze (Robot Pathfinding - Backtracking)
 * 
 * Problem Statement:
 *   A robot needs to find a path from start to end in a maze with obstacles.
 *   The robot can move right, down, left, or up. Design and implement a
 *   solution to find any one valid path using backtracking. Display the path. (CO2)
 * 
 * Chosen Algorithm & Architectural Justification:
 *   1. Algorithm: Recursive Backtracking (DFS with state restoration)
 *      - Justification: Systematically explores the 4-directional search space.
 *        If the robot encounters a wall or a dead-end, it undoes the current
 *        step (backtracks) and tries alternative directional branches.
 *      - Avoids infinite loops by marking visited cells during search.
 *      - Memory Efficient: Auxiliary space is bounded by the recursive call stack
 *        O(N * M), requiring no large external frontier queues.
 * 
 * Movement Directions:
 *   - Right: (row, col + 1)
 *   - Down:  (row + 1, col)
 *   - Left:  (row, col - 1)
 *   - Up:    (row - 1, col)
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 20
#define MAX_COLS 20

/* Direction vectors: Right, Down, Left, Up */
static const int dRow[] = {0, 1, 0, -1};
static const int dCol[] = {1, 0, -1, 0};

/* Path Coordinate for recording step order */
typedef struct {
    int r;
    int c;
} Point;

/* Check if a cell is valid for the robot to move into */
int isSafe(int maze[MAX_ROWS][MAX_COLS], int visited[MAX_ROWS][MAX_COLS],
           int r, int c, int rows, int cols) {
    return (r >= 0 && r < rows && c >= 0 && c < cols &&
            maze[r][c] == 0 && visited[r][c] == 0);
}

/* 
 * Recursive Backtracking Function
 * Explores 4 directions: Right, Down, Left, Up
 */
int solveMazeBacktracking(int maze[MAX_ROWS][MAX_COLS], int visited[MAX_ROWS][MAX_COLS],
                          int r, int c, int destR, int destC, int rows, int cols,
                          Point path[], int* pathLen) {
    // Record current step
    visited[r][c] = 1;
    path[*pathLen].r = r;
    path[*pathLen].c = c;
    (*pathLen)++;

    // Destination reached!
    if (r == destR && c == destC) {
        return 1;
    }

    // Try moving in all 4 directions
    for (int d = 0; d < 4; d++) {
        int nextR = r + dRow[d];
        int nextC = c + dCol[d];

        if (isSafe(maze, visited, nextR, nextC, rows, cols)) {
            if (solveMazeBacktracking(maze, visited, nextR, nextC, destR, destC, rows, cols, path, pathLen)) {
                return 1; // Path found
            }
        }
    }

    // BACKTRACK: Unmark cell as part of the active solution path
    (*pathLen)--;
    visited[r][c] = 0;
    return 0;
}

/* Render Maze Grid with Robot Solution Path */
void printVisualMaze(int maze[MAX_ROWS][MAX_COLS], Point path[], int pathLen, int rows, int cols) {
    char display[MAX_ROWS][MAX_COLS];

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (maze[r][c] == 1) {
                display[r][c] = '#'; // Wall / Obstacle
            } else {
                display[r][c] = '.'; // Open passage
            }
        }
    }

    // Mark solution path with '*'
    for (int i = 0; i < pathLen; i++) {
        display[path[i].r][path[i].c] = '*';
    }

    // Mark Start and End explicitly
    if (pathLen > 0) {
        display[path[0].r][path[0].c] = 'S';
        display[path[pathLen - 1].r][path[pathLen - 1].c] = 'E';
    }

    printf("\n--- VISUAL MAZE GRID WITH SOLVED PATH ---\n");
    printf("Legend: [S] = Start (0,0), [E] = Goal (%d,%d), [*] = Robot Path, [#] = Obstacle, [.] = Open\n\n   ", rows - 1, cols - 1);
    for (int c = 0; c < cols; c++) printf("%2d ", c);
    printf("\n  +");
    for (int c = 0; c < cols; c++) printf("---");
    printf("+\n");

    for (int r = 0; r < rows; r++) {
        printf("%2d|", r);
        for (int c = 0; c < cols; c++) {
            printf(" %c ", display[r][c]);
        }
        printf("|\n");
    }
    printf("  +");
    for (int c = 0; c < cols; c++) printf("---");
    printf("+\n\n");
}

/* Run maze solver and display comprehensive results */
void runMazeSolver(int maze[MAX_ROWS][MAX_COLS], int rows, int cols, const char* label) {
    printf("\n========================================================================\n");
    printf("                %s\n", label);
    printf("========================================================================\n");
    printf("Maze Dimensions: %d Rows x %d Columns\n", rows, cols);
    printf("Start Position : (0, 0)\n");
    printf("Goal Position  : (%d, %d)\n\n", rows - 1, cols - 1);

    // Initial check
    if (maze[0][0] == 1 || maze[rows - 1][cols - 1] == 1) {
        printf("[Error] Start or Goal position is blocked by an obstacle!\n\n");
        return;
    }

    int visited[MAX_ROWS][MAX_COLS] = {0};
    Point path[MAX_ROWS * MAX_COLS];
    int pathLen = 0;

    int success = solveMazeBacktracking(maze, visited, 0, 0, rows - 1, cols - 1, rows, cols, path, &pathLen);

    if (success) {
        printf("[Success] A valid path was successfully discovered using backtracking!\n");
        printf("Total Steps Required: %d\n", pathLen);

        printVisualMaze(maze, path, pathLen, rows, cols);

        printf("STEP-BY-STEP ROBOT TRAJECTORY:\n  ");
        for (int i = 0; i < pathLen; i++) {
            printf("(%d,%d)", path[i].r, path[i].c);
            if (i < pathLen - 1) {
                // Determine direction
                int dr = path[i + 1].r - path[i].r;
                int dc = path[i + 1].c - path[i].c;
                const char* dName = "";
                if (dr == 0 && dc == 1) dName = "R";
                else if (dr == 1 && dc == 0) dName = "D";
                else if (dr == 0 && dc == -1) dName = "L";
                else if (dr == -1 && dc == 0) dName = "U";
                printf(" -[%s]-> ", dName);
            }
            if ((i + 1) % 6 == 0 && i < pathLen - 1) printf("\n  ");
        }
        printf("\n");
    } else {
        printf("[Result] No valid path exists from start to destination in this maze!\n");
        printf("The robot exhausted all directional permutations and hit obstacles.\n");
    }
    printf("========================================================================\n\n");
}

/* Display Design & Algorithmic Justification */
void displayBacktrackingJustification(void) {
    printf("\n========================================================================================\n");
    printf("       ARCHITECTURAL JUSTIFICATION: RAT IN A MAZE (BACKTRACKING) (CO2)                  \n");
    printf("========================================================================================\n");
    printf("1. Why Backtracking is Optimal for Maze Navigation:\n");
    printf("   - Backtracking is a systematic state-space search paradigm.\n");
    printf("   - When a robot enters a cul-de-sac or hit an obstacle wall, backtracking immediately\n");
    printf("     prunes the search path and restores the previous valid coordinate (state rollback).\n");
    printf("   - Prevents exhaustive brute force: Only valid exploratory sub-branches are expanded.\n\n");
    printf("2. Time & Space Complexity:\n");
    printf("   - Worst-case Time Complexity: O(4^(N * M)) in theoretical full tree without pruning,\n");
    printf("     but visited marking bounds practical time to O(N * M) state visits.\n");
    printf("   - Auxiliary Space Complexity: O(N * M) for the recursion call stack and visited matrix.\n\n");
    printf("3. Comparison with BFS:\n");
    printf("   - BFS finds the shortest path but requires an explicit queue storing thousands of states.\n");
    printf("   - Backtracking (DFS) requires minimal memory overhead and is naturally recursive.\n");
    printf("========================================================================================\n\n");
}

int main(void) {
    int choice;

    // Default 7x7 Solvable Complex Maze
    int solvableMaze[MAX_ROWS][MAX_COLS] = {
        {0, 1, 0, 0, 0, 0, 0},
        {0, 1, 0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0, 0, 0},
        {1, 1, 0, 1, 0, 1, 1},
        {0, 0, 0, 0, 0, 1, 0},
        {0, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 0}
    };

    // Unsolvable Maze (Surrounded Goal)
    int unsolvableMaze[MAX_ROWS][MAX_COLS] = {
        {0, 0, 1, 0, 0},
        {1, 0, 1, 0, 0},
        {0, 0, 1, 1, 1},
        {1, 1, 1, 0, 1},
        {0, 0, 0, 1, 0}
    };

    printf("========================================================================\n");
    printf("   BMSIT&M - DSA LAB - PART B: RAT IN A MAZE (BACKTRACKING) (CO2)       \n");
    printf("========================================================================\n");

    while (1) {
        printf("\n--- MENU ---\n");
        printf("1. Solve Standard 7x7 Solvable Maze (Displays Visual Path)\n");
        printf("2. Test Unsolvable Maze (Demonstrates Obstacle Dead-End Detection)\n");
        printf("3. Solve Custom User-Defined Maze\n");
        printf("4. View Architectural Justification (Viva Voce Prep)\n");
        printf("5. Exit\n");
        printf("Enter choice (1-5): ");

        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
            case 1:
                runMazeSolver(solvableMaze, 7, 7, "DEMONSTRATION: 7x7 COMPLEX SOLVABLE MAZE");
                break;

            case 2:
                runMazeSolver(unsolvableMaze, 5, 5, "DEMONSTRATION: UNSOLVABLE MAZE WITH BLOCKED PATH");
                break;

            case 3: {
                int r, c;
                printf("\nEnter number of rows (e.g. 4 to 10): ");
                scanf("%d", &r);
                printf("Enter number of columns (e.g. 4 to 10): ");
                scanf("%d", &c);
                if (r <= 0 || r > MAX_ROWS || c <= 0 || c > MAX_COLS) {
                    printf("[Error] Dimensions out of range.\n");
                    break;
                }
                int customMaze[MAX_ROWS][MAX_COLS];
                printf("Enter maze grid (0 for passage, 1 for wall) row by row:\n");
                for (int i = 0; i < r; i++) {
                    printf("  Row %d (%d numbers): ", i, c);
                    for (int j = 0; j < c; j++) {
                        scanf("%d", &customMaze[i][j]);
                    }
                }
                runMazeSolver(customMaze, r, c, "CUSTOM USER MAZE");
                break;
            }

            case 4:
                displayBacktrackingJustification();
                break;

            case 5:
                printf("Exiting maze solver. Goodbye!\n");
                return 0;

            default:
                printf("[Error] Invalid choice! Select 1-5.\n");
        }
    }

    return 0;
}
