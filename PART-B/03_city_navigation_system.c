/**
 * ============================================================================
 * BMS INSTITUTE OF TECHNOLOGY & MANAGEMENT
 * DEPARTMENT OF ARTIFICIAL INTELLIGENCE AND MACHINE LEARNING
 * Course: Data Structures & Algorithms Laboratory (Course Code: 1BAIL306)
 * Semester: III | Scheme: 2025
 * ----------------------------------------------------------------------------
 * PART B - OPEN ENDED EXPERIMENT 3: Navigation System for City Map
 * 
 * Problem Statement:
 *   A GPS navigation company wants to find the shortest path between two
 *   locations in a city. Given a map with roads and distances, design and
 *   implement a solution to display the shortest route. Choose any algorithm
 *   and graph representation, and justify your choices. (CO2)
 * 
 * Chosen Graph Representation & Algorithm:
 *   1. Graph Representation: Adjacency List with Road Names and Distances
 *      - Justification: City road networks are planar and sparse (each junction
 *        typically connects to 3-5 roads). Adjacency List consumes O(V + E)
 *        memory, whereas an Adjacency Matrix wastes O(V^2) space for millions
 *        of zeros.
 *   2. Algorithm: Dijkstra's Shortest Path with Min-Priority Queue (Binary Heap)
 *      - Justification: Road distances are strictly non-negative. Dijkstra's
 *        algorithm guarantees the global optimal shortest route with time
 *        complexity O((V + E) log V). Much faster than Bellman-Ford O(V * E)
 *        or all-pairs Floyd-Warshall O(V^3).
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_LOCATIONS 30
#define NAME_LEN 40
#define ROAD_LEN 40
#define INF 1000000000

/* Edge in Adjacency List representing a road connection */
typedef struct Edge {
    int dest;
    int distanceKm;
    char roadName[ROAD_LEN];
    struct Edge* next;
} Edge;

/* City Graph Structure */
typedef struct {
    char locationNames[MAX_LOCATIONS][NAME_LEN];
    Edge* adjList[MAX_LOCATIONS];
    int numLocations;
} CityGraph;

/* Min-Heap node for Dijkstra Priority Queue */
typedef struct {
    int vertex;
    int dist;
} HeapNode;

typedef struct {
    HeapNode* heap;
    int* pos; // Maps vertex to current position in heap
    int size;
    int capacity;
} MinPriorityQueue;

/* Initialize City Graph */
void initCityGraph(CityGraph* g) {
    g->numLocations = 0;
    for (int i = 0; i < MAX_LOCATIONS; i++) {
        g->adjList[i] = NULL;
    }
}

/* Add location / landmark */
int addLocation(CityGraph* g, const char* name) {
    if (g->numLocations >= MAX_LOCATIONS) {
        printf("[Error] Max location limit reached.\n");
        return -1;
    }
    int id = g->numLocations++;
    strncpy(g->locationNames[id], name, NAME_LEN - 1);
    g->locationNames[id][NAME_LEN - 1] = '\0';
    return id;
}

/* Add bidirectional road between two locations */
void addRoad(CityGraph* g, int u, int v, int distKm, const char* road) {
    if (u < 0 || u >= g->numLocations || v < 0 || v >= g->numLocations) {
        printf("[Error] Invalid location ID.\n");
        return;
    }

    // Edge u -> v
    Edge* e1 = (Edge*)malloc(sizeof(Edge));
    e1->dest = v;
    e1->distanceKm = distKm;
    strncpy(e1->roadName, road, ROAD_LEN - 1);
    e1->roadName[ROAD_LEN - 1] = '\0';
    e1->next = g->adjList[u];
    g->adjList[u] = e1;

    // Edge v -> u (bidirectional road)
    Edge* e2 = (Edge*)malloc(sizeof(Edge));
    e2->dest = u;
    e2->distanceKm = distKm;
    strncpy(e2->roadName, road, ROAD_LEN - 1);
    e2->roadName[ROAD_LEN - 1] = '\0';
    e2->next = g->adjList[v];
    g->adjList[v] = e2;
}

/* Min Priority Queue Functions for Dijkstra */
MinPriorityQueue* createPQ(int capacity) {
    MinPriorityQueue* pq = (MinPriorityQueue*)malloc(sizeof(MinPriorityQueue));
    pq->heap = (HeapNode*)malloc(capacity * sizeof(HeapNode));
    pq->pos = (int*)malloc(capacity * sizeof(int));
    pq->size = 0;
    pq->capacity = capacity;
    return pq;
}

void swapHeapNodes(MinPriorityQueue* pq, int i, int j) {
    HeapNode temp = pq->heap[i];
    pq->heap[i] = pq->heap[j];
    pq->heap[j] = temp;

    pq->pos[pq->heap[i].vertex] = i;
    pq->pos[pq->heap[j].vertex] = j;
}

void minHeapify(MinPriorityQueue* pq, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < pq->size && pq->heap[left].dist < pq->heap[smallest].dist)
        smallest = left;
    if (right < pq->size && pq->heap[right].dist < pq->heap[smallest].dist)
        smallest = right;

    if (smallest != idx) {
        swapHeapNodes(pq, idx, smallest);
        minHeapify(pq, smallest);
    }
}

HeapNode extractMin(MinPriorityQueue* pq) {
    HeapNode root = pq->heap[0];
    HeapNode last = pq->heap[pq->size - 1];
    pq->heap[0] = last;
    pq->pos[last.vertex] = 0;
    pq->pos[root.vertex] = -1;
    pq->size--;
    minHeapify(pq, 0);
    return root;
}

void decreaseKey(MinPriorityQueue* pq, int v, int dist) {
    int i = pq->pos[v];
    pq->heap[i].dist = dist;
    while (i > 0 && pq->heap[i].dist < pq->heap[(i - 1) / 2].dist) {
        swapHeapNodes(pq, i, (i - 1) / 2);
        i = (i - 1) / 2;
    }
}

int isInMinHeap(MinPriorityQueue* pq, int v) {
    return (pq->pos[v] < pq->size && pq->pos[v] >= 0);
}

/* 
 * Dijkstra's Shortest Path Algorithm
 * Finds shortest route from src to dest with turn-by-turn guidance
 */
void findShortestRoute(CityGraph* g, int src, int dest) {
    int V = g->numLocations;
    int dist[MAX_LOCATIONS];
    int parent[MAX_LOCATIONS];
    char roadUsed[MAX_LOCATIONS][ROAD_LEN];

    MinPriorityQueue* pq = createPQ(V);

    for (int v = 0; v < V; v++) {
        dist[v] = INF;
        parent[v] = -1;
        roadUsed[v][0] = '\0';
        pq->heap[v].vertex = v;
        pq->heap[v].dist = INF;
        pq->pos[v] = v;
    }

    dist[src] = 0;
    pq->heap[src].dist = 0;
    pq->size = V;
    decreaseKey(pq, src, 0);

    while (pq->size > 0) {
        HeapNode minNode = extractMin(pq);
        int u = minNode.vertex;

        if (dist[u] == INF) break;
        if (u == dest) break; // Reached target destination

        Edge* edge = g->adjList[u];
        while (edge != NULL) {
            int v = edge->dest;
            if (isInMinHeap(pq, v) && dist[u] + edge->distanceKm < dist[v]) {
                dist[v] = dist[u] + edge->distanceKm;
                parent[v] = u;
                strncpy(roadUsed[v], edge->roadName, ROAD_LEN - 1);
                roadUsed[v][ROAD_LEN - 1] = '\0';
                decreaseKey(pq, v, dist[v]);
            }
            edge = edge->next;
        }
    }

    if (dist[dest] == INF) {
        printf("\n[Navigation Result] No route exists between %s and %s!\n\n",
               g->locationNames[src], g->locationNames[dest]);
        free(pq->heap);
        free(pq->pos);
        free(pq);
        return;
    }

    // Reconstruct path
    int path[MAX_LOCATIONS];
    int pathLen = 0;
    int curr = dest;
    while (curr != -1) {
        path[pathLen++] = curr;
        curr = parent[curr];
    }

    printf("\n========================================================================================\n");
    printf("         GPS NAVIGATION DIRECTIONS: %s -> %s\n", g->locationNames[src], g->locationNames[dest]);
    printf("========================================================================================\n");
    printf("Total Route Distance: %d km\n", dist[dest]);
    double estTimeMin = (dist[dest] / 35.0) * 60.0; // Assume 35 km/h urban speed
    printf("Estimated Travel Time: ~%.0f minutes (assuming normal city traffic)\n\n", estTimeMin);
    printf("TURN-BY-TURN ROUTE GUIDE:\n");

    for (int i = pathLen - 1; i > 0; i--) {
        int from = path[i];
        int to = path[i - 1];
        int stepDist = dist[to] - dist[from];
        printf("  Step %d: From [%s]\n", pathLen - i, g->locationNames[from]);
        printf("          Take '%s' -> Proceed %d km -> Arrive at [%s]\n",
               roadUsed[to], stepDist, g->locationNames[to]);
    }
    printf("  Destination Reached: Welcome to %s!\n", g->locationNames[dest]);
    printf("========================================================================================\n\n");

    free(pq->heap);
    free(pq->pos);
    free(pq);
}

/* Display full city map */
void displayCityMap(CityGraph* g) {
    printf("\n========================================================================\n");
    printf("                   CITY MAP ROAD NETWORK (GRAPH)                        \n");
    printf("========================================================================\n");
    for (int i = 0; i < g->numLocations; i++) {
        printf("[%2d] %-22s -> ", i, g->locationNames[i]);
        Edge* e = g->adjList[i];
        if (!e) {
            printf("No connected roads\n");
            continue;
        }
        while (e != NULL) {
            printf("[%s, %dkm via %s] ", g->locationNames[e->dest], e->distanceKm, e->roadName);
            e = e->next;
        }
        printf("\n");
    }
    printf("========================================================================\n\n");
}

/* Setup Realistic Map (Bengaluru Metropolitan Region with BMSIT&M) */
void setupDefaultBengaluruMap(CityGraph* g) {
    initCityGraph(g);
    addLocation(g, "Majestic (City Hub)");     // 0
    addLocation(g, "MG Road");                 // 1
    addLocation(g, "Indiranagar");             // 2
    addLocation(g, "Whitefield (Tech Hub)");   // 3
    addLocation(g, "Silk Board Junction");     // 4
    addLocation(g, "Electronic City");         // 5
    addLocation(g, "Hebbal Flyover");          // 6
    addLocation(g, "Yelahanka (BMSIT&M)");     // 7
    addLocation(g, "Kempegowda Airport (BLR)");// 8

    addRoad(g, 0, 1, 6, "Subhash Nagar Road");
    addRoad(g, 1, 2, 5, "Old Madras Road");
    addRoad(g, 2, 3, 14, "Whitefield Main Road");
    addRoad(g, 1, 4, 8, "Hosur Road");
    addRoad(g, 4, 5, 12, "Electronic City Flyover");
    addRoad(g, 0, 6, 9, "Bellary Road Corridor");
    addRoad(g, 6, 7, 10, "NH 44 Highway");
    addRoad(g, 7, 8, 16, "Airport Expressway");
    addRoad(g, 6, 2, 11, "Outer Ring Road (East)");
    addRoad(g, 4, 2, 9, "Sarjapur-Ring Road");
}

/* Architectural Justification Display */
void displayNavigationJustification(void) {
    printf("\n========================================================================================\n");
    printf("       ARCHITECTURAL JUSTIFICATION: CITY NAVIGATION SYSTEM (CO2)                        \n");
    printf("========================================================================================\n");
    printf("1. Graph Representation: Adjacency List over Adjacency Matrix:\n");
    printf("   - Physical road maps are sparse planar graphs where degree(V) <= 5 to 6 in practice.\n");
    printf("   - Adjacency List space is O(V + E), whereas Adjacency Matrix is O(V^2).\n");
    printf("   - In a large city with 100,000 intersections, Adjacency Matrix would require 10 billion\n");
    printf("     cells (~40 GB of RAM!), whereas an Adjacency List requires only a few megabytes!\n\n");
    printf("2. Algorithm: Dijkstra with Min-Heap vs Alternatives:\n");
    printf("   - Dijkstra with Min-Priority Queue achieves optimal O((V + E) log V) time.\n");
    printf("   - Why not Bellman-Ford: Bellman-Ford handles negative weights in O(V * E). Physical road\n");
    printf("     distances are NEVER negative, so Bellman-Ford's slower runtime is unnecessary.\n");
    printf("   - Why not Floyd-Warshall: Floyd-Warshall is O(V^3) all-pairs. Point-to-point queries\n");
    printf("     only require single-source shortest path from the driver's current GPS location.\n");
    printf("========================================================================================\n\n");
}

int main(void) {
    CityGraph g;
    setupDefaultBengaluruMap(&g);
    int choice;

    printf("========================================================================\n");
    printf("   BMSIT&M - DSA LAB - PART B: GPS NAVIGATION SYSTEM (CO2)              \n");
    printf("========================================================================\n");

    while (1) {
        printf("\n--- MENU ---\n");
        printf("1. Display City Map & Intersections\n");
        printf("2. Find Shortest Route & Get Turn-by-Turn Directions\n");
        printf("3. Add Custom Location / Road\n");
        printf("4. View Architectural Justification (Viva Voce Prep)\n");
        printf("5. Exit\n");
        printf("Enter choice (1-5): ");

        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
            case 1:
                displayCityMap(&g);
                break;

            case 2: {
                printf("\nAvailable Locations:\n");
                for (int i = 0; i < g.numLocations; i++) {
                    printf("  [%d] %s\n", i, g.locationNames[i]);
                }
                int src, dest;
                printf("\nEnter Source Location ID (0 to %d): ", g.numLocations - 1);
                scanf("%d", &src);
                printf("Enter Destination Location ID (0 to %d): ", g.numLocations - 1);
                scanf("%d", &dest);

                if (src >= 0 && src < g.numLocations && dest >= 0 && dest < g.numLocations) {
                    findShortestRoute(&g, src, dest);
                } else {
                    printf("[Error] Invalid location IDs entered.\n");
                }
                break;
            }

            case 3: {
                char locName[NAME_LEN];
                printf("Enter new location name: ");
                while (getchar() != '\n');
                fgets(locName, sizeof(locName), stdin);
                locName[strcspn(locName, "\r\n")] = '\0';
                int newId = addLocation(&g, locName);
                if (newId != -1) {
                    printf("Location added as ID [%d]. Now connect to an existing location:\n", newId);
                    int connectTo, km;
                    char rName[ROAD_LEN];
                    printf("Connect to Location ID: ");
                    scanf("%d", &connectTo);
                    printf("Distance in km: ");
                    scanf("%d", &km);
                    printf("Road name: ");
                    while (getchar() != '\n');
                    fgets(rName, sizeof(rName), stdin);
                    rName[strcspn(rName, "\r\n")] = '\0';
                    addRoad(&g, newId, connectTo, km, rName);
                    printf("[Success] Road added and connected.\n");
                }
                break;
            }

            case 4:
                displayNavigationJustification();
                break;

            case 5:
                printf("Exiting navigation system. Goodbye!\n");
                return 0;

            default:
                printf("[Error] Invalid choice! Select 1-5.\n");
        }
    }

    return 0;
}
