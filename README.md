# Data Structures & Algorithms Laboratory (1BAIL306)

**BMS Institute of Technology & Management**  
*(An Autonomous Institution affiliated to VTU, Belagavi | Yelahanka, Bengaluru - 560 119)*  
**Department of Artificial Intelligence and Machine Learning**  
**Course Code:** `1BAIL306` | **Semester:** III | **Scheme:** 2025  
**Teaching Hours / Week (L:T:P):** `0:0:2` | **Credits:** 1  
**CIE Marks:** 50 | **SEE Marks:** 50 | **Total Marks:** 100  

---

## 📋 Course Outcomes (COs)

- **CO1:** Implement linear and non-linear data structures in C with appropriate memory management (static or dynamic).
- **CO2:** Apply divide-and-conquer, greedy, dynamic programming, and backtracking paradigms to solve computational problems.
- **CO3:** Develop integrated C programs combining multiple data structures and algorithmic techniques for real-world problems.

---

## 📁 Repository Structure

```tree
DSA-PROGRAMS/
├── PART-A/                                 # Conventional Core Experiments (CO1 & CO2)
│   ├── 01_singly_linked_list.c            # Singly Linked List Operations
│   ├── 02_infix_to_postfix.c              # Stack Application: Infix to Postfix Conversion
│   ├── 03_circular_queue.c                # Circular Queue Implementation & Overcoming Linear Limits
│   ├── 04_merge_sort.c                    # Merge Sort & Empirical Complexity Analysis
│   ├── 05_quick_sort.c                    # Quick Sort (Lomuto) & Comparison vs Merge Sort
│   ├── 06_knapsack_01.c                   # 0/1 Knapsack DP (2D Table, 1D Optimized, Greedy Failure)
│   ├── 07_binary_search_tree.c            # Binary Search Tree Operations & Height Observation
│   └── 08_longest_common_subsequence.c    # Longest Common Subsequence (2D Table & O(min(m,n)) Space)
│
├── PART-B/                                 # Concept-Based Open-Ended Case Studies (CO2 & CO3)
│   ├── 01_search_engine_text_analysis.c   # Search Engine Keyword Frequency (Hash Table + Min-Heap)
│   ├── 02_file_compression_tool.c        # Archival Compression (Huffman Coding vs RLE)
│   ├── 03_city_navigation_system.c        # GPS City Navigation (Dijkstra Shortest Path on Adjacency List)
│   ├── 04_mobile_keyboard_autocorrect.c   # Smartphone Autocorrect (Trie + Levenshtein DP Pruning)
│   ├── 05_rat_in_a_maze.c                 # Robot Maze Navigation (4-Directional Backtracking)
│   └── 06_campus_network_design.c         # Campus Fiber Optic Layout (Kruskal's MST + DSU)
│
├── practice_programs/                      # Legacy & Preliminary Practice Exercises
│   ├── enque deque.c
│   ├── pg1 push,pop.c
│   ├── pg3 postfix to result.c
│   ├── pg4 line enque deque.c
│   ├── pg5 circle enque.c
│   ├── pg6 malloc.c
│   ├── pg7 realloc.c
│   └── program2infixpostfix.c
│
├── results/                                # Empirical Benchmark Output Logs & ASCII Plots
│   ├── merge_sort_benchmark.txt
│   ├── quick_sort_benchmark.txt
│   └── lcs_benchmark.txt
│
├── scripts/                                # Test Automation & Benchmarking Utilities
│   ├── run_all.py                         # One-click compile & test runner for all 14 experiments
│   └── benchmark_and_plot.py              # Automated empirical benchmark execution
│
├── .gitignore
└── README.md                               # Complete Documentation, Lab Manual & Viva Prep
```

---

## 🚀 Quick Start: Compilation & Execution

### Prerequisites
- GCC / Clang (or any C99/C11 compliant compiler)
- Python 3.8+ (for automated test scripts)

### 1. Compile & Verify All Programs in One Command
```bash
python scripts/run_all.py
```
*Expected Output: `Results: 14 / 14 Programs Compiled Successfully (100% Pass Rate)`*

### 2. Run Benchmarking & Empirical Analysis
```bash
python scripts/benchmark_and_plot.py
```

### 3. Compile Any Program Individually (GCC)
```bash
# Example: Part A - Singly Linked List
gcc -Wall -Wextra -O2 PART-A/01_singly_linked_list.c -o sll.exe
./sll.exe

# Example: Part B - City Navigation System
gcc -Wall -Wextra -O2 PART-B/03_city_navigation_system.c -o nav.exe
./nav.exe
```

---

## 📖 PART A: Conventional Experiments

### 1. Singly Linked List Operations (`PART-A/01_singly_linked_list.c`)
- **Course Outcome:** CO1
- **Features:** Menu-driven program supporting Create, Insert at End, Delete by Value, Search Element, In-Place Reversal, and Display.
- **Complexity Breakdown:**
  | Operation | Time (Best) | Time (Average) | Time (Worst) | Auxiliary Space |
  | :--- | :---: | :---: | :---: | :---: |
  | Create ($n$ elements) | $\mathcal{O}(n)$ | $\mathcal{O}(n)$ | $\mathcal{O}(n)$ | $\mathcal{O}(n)$ |
  | Insert at End | $\mathcal{O}(n)^*$ | $\mathcal{O}(n)^*$ | $\mathcal{O}(n)^*$ | $\mathcal{O}(1)$ |
  | Delete by Value | $\mathcal{O}(1)$ | $\mathcal{O}(n)$ | $\mathcal{O}(n)$ | $\mathcal{O}(1)$ |
  | Search for Element | $\mathcal{O}(1)$ | $\mathcal{O}(n)$ | $\mathcal{O}(n)$ | $\mathcal{O}(1)$ |
  | Reverse List (In-Place) | $\mathcal{O}(n)$ | $\mathcal{O}(n)$ | $\mathcal{O}(n)$ | $\mathcal{O}(1)$ |
  *\*Note: Insert at end is $\mathcal{O}(1)$ when maintaining a tail pointer.*

---

### 2. Stack Application: Infix to Postfix Conversion (`PART-A/02_infix_to_postfix.c`)
- **Course Outcome:** CO1
- **Features:** Array-based stack supporting operators `+`, `-`, `*`, `/`, `^`, parentheses `(`, `)`, and single-digit/variable operands. Includes step-by-step conversion trace and an automated 5-expression test suite:
  1. `(A+B)*(C-D)` $\rightarrow$ `AB+CD-*`
  2. `A+B*C-D/E` $\rightarrow$ `ABC*+DE/-`
  3. `(A+B*C)/(D-E)` $\rightarrow$ `ABC*+DE-/`
  4. `2+3*(4-1)` $\rightarrow$ `2341-*+`
  5. `A*(B+C)/D+E` $\rightarrow$ `ABC+*D/E+`
- **Time Complexity:** $\mathcal{O}(N)$ | **Space Complexity:** $\mathcal{O}(N)$

---

### 3. Circular Queue Implementation (`PART-A/03_circular_queue.c`)
- **Course Outcome:** CO1
- **Features:** Array-based circular queue with Enqueue, Dequeue, Display, and an interactive demonstration showing how circular modulo arithmetic `(rear + 1) % MAX` reuses vacant slots and eliminates the "False Overflow" flaw of linear queues.
- **Time Complexity:** Enqueue $\mathcal{O}(1)$, Dequeue $\mathcal{O}(1)$, Display $\mathcal{O}(n)$ | **Space Complexity:** $\mathcal{O}(MAX)$

---

### 4. Merge Sort & Empirical Complexity Analysis (`PART-A/04_merge_sort.c`)
- **Course Outcome:** CO2
- **Features:** Sorts $n$ random integers for $n = 5000, 10000, 20000, 50000$ using Divide and Conquer. Displays runtime in milliseconds and renders an ASCII plot of execution time vs $n$.
- **Empirical Results:**
  | $n$ | Time (seconds) | Time (ms) | Status |
  | :---: | :---: | :---: | :---: |
  | 5,000 | 0.001217 s | 1.217 ms | `VERIFIED` |
  | 10,000 | 0.003107 s | 3.107 ms | `VERIFIED` |
  | 20,000 | 0.003788 s | 3.788 ms | `VERIFIED` |
  | 50,000 | 0.008487 s | 8.487 ms | `VERIFIED` |
- **Recurrence Relation:** $T(n) = 2T(n/2) + \Theta(n) \implies \Theta(n \log_2 n)$ across Best, Average, and Worst cases.

---

### 5. Quick Sort (Lomuto) & Comparison vs Merge Sort (`PART-A/05_quick_sort.c`)
- **Course Outcome:** CO2
- **Features:** In-place partitioning with last element as pivot (Lomuto scheme). Benchmark mode evaluates Quick Sort vs Merge Sort across **Random**, **Already Sorted**, and **Reverse Sorted** arrays.
- **Empirical Comparison Table:**
  | $n$ | Input Distribution | Quick Sort (ms) | Merge Sort (ms) | Winner |
  | :---: | :--- | :---: | :---: | :---: |
  | 5,000 | Random | 0.241 ms | 0.674 ms | **Quick Sort** |
  | 10,000 | Random | 0.597 ms | 1.394 ms | **Quick Sort** |
  | 20,000 | Random | 1.139 ms | 2.724 ms | **Quick Sort** |
  | 5,000 | Already Sorted | 6.781 ms | 0.534 ms | **Merge Sort** |
  | 10,000 | Already Sorted | 26.552 ms | 1.028 ms | **Merge Sort** |
  | 20,000 | Already Sorted | 105.603 ms | 2.136 ms | **Merge Sort** |
  | 5,000 | Reverse Sorted | 4.726 ms | 0.606 ms | **Merge Sort** |
  | 10,000 | Reverse Sorted | 18.481 ms | 1.400 ms | **Merge Sort** |
  | 20,000 | Reverse Sorted | 78.797 ms | 2.094 ms | **Merge Sort** |
- **Key Insight:** On random data, Quick Sort is $2\times$ faster than Merge Sort due to smaller constant factors and cache locality. However, on sorted/reverse-sorted data, Lomuto partitioning degenerates to $\mathcal{O}(n^2)$ ($105.6\text{ ms}$ vs $2.1\text{ ms}$).

---

### 6. 0/1 Knapsack using Dynamic Programming (`PART-A/06_knapsack_01.c`)
- **Course Outcome:** CO2
- **Features:**
  1. Standard 2D DP Table $\mathcal{O}(nW)$ with full item backtracking reconstruction.
  2. Space-Optimized 1D Array $\mathcal{O}(W)$ traversed backwards.
  3. Benchmark for $n = 10, 20, 50$ across capacities $W = 50, 100, 200$.
  4. Mathematical proof and counterexample showing why greedy value/weight ratio fails for 0/1 knapsack.
- **Greedy Failure Proof:**
  - Capacity $W = 50$. Items: (Wt: 10, Val: 60, Ratio: 6.0), (Wt: 20, Val: 100, Ratio: 5.0), (Wt: 30, Val: 120, Ratio: 4.0).
  - Greedy picks Item 1 + Item 2 $\rightarrow$ Total Value = **160** (Wasted capacity: 20).
  - Optimal DP picks Item 2 + Item 3 $\rightarrow$ Total Value = **220** (Optimal capacity usage: 50/50).

---

### 7. Binary Search Tree Operations & Height Observation (`PART-A/07_binary_search_tree.c`)
- **Course Outcome:** CO1
- **Features:** Insert, Search, Inorder, Preorder, Postorder traversals. Height observation benchmark for $n = 100, 500, 1000$ across Random, Ascending, and Descending sequences.
- **Height Observation Results:**
  | $n$ | Random Height | Ascending Height | Descending Height | Ideal $\approx \log_2(n)$ |
  | :---: | :---: | :---: | :---: | :---: |
  | 100 | 14 | 100 | 100 | $\sim 7$ |
  | 500 | 20 | 500 | 500 | $\sim 9$ |
  | 1,000 | 22 | 1,000 | 1,000 | $\sim 10$ |
- **Insight:** Random inputs yield balanced trees of height $\mathcal{O}(\log n)$. Ordered inputs produce degenerate linear chains of height $n$, motivating self-balancing trees (AVL / Red-Black).

---

### 8. Longest Common Subsequence (LCS) using DP (`PART-A/08_longest_common_subsequence.c`)
- **Course Outcome:** CO2
- **Features:**
  1. 2D DP table $\mathcal{O}(mn)$ with backtracking to extract and print the LCS string.
  2. Space-Optimized rolling array $\mathcal{O}(\min(m, n))$.
  3. Benchmark for string lengths up to 1000 ($mn$ up to $1,000,000$) with ASCII time plot.
- **Space Reduction:** For $m=n=1000$, standard 2D table requires $10^6$ ints ($\approx 4\text{ MB}$), while space-optimized DP requires only $2000$ ints ($\approx 8\text{ KB}$).

---

## 💡 PART B: Open-Ended Case Studies

### 1. Text Analysis for Search Engine (`PART-B/01_search_engine_text_analysis.c`)
- **Course Outcome:** CO3
- **Problem Statement:** Identify most relevant keywords from large documents for search ranking, ignoring common noise words.
- **Architectural Design & Justification:**
  - **Hash Table with Chaining (djb2 hash):** Provides $\mathcal{O}(1)$ average lookup and frequency increments over arbitrary document lengths.
  - **Stop-Words Hash Set:** Filters out common English noise words (*the, is, and, of, to, in, with...*) in $\mathcal{O}(1)$ time before counting.
  - **Min-Heap of Size $K$:** Extracts the Top-$K$ most frequent keywords in $\mathcal{O}(V \log K)$ time instead of $\mathcal{O}(V \log V)$ sorting, dramatically reducing CPU cycles on large vocabularies.

---

### 2. File Compression Tool (`PART-B/02_file_compression_tool.c`)
- **Course Outcome:** CO3
- **Problem Statement:** Design a lossless data compression tool and compare its compression ratio with Run-Length Encoding (RLE).
- **Architectural Design & Justification:**
  - **Huffman Coding:** Constructs an optimal prefix binary tree using a Min-Priority Queue. Assigns variable-length bit codes matching symbol entropy.
  - **Run-Length Encoding (RLE):** Linear scan $\mathcal{O}(N)$ encoding consecutive identical runs as count + character.
  - **Empirical Findings:**
    - On natural text, Huffman achieves **$40-50\%$ space savings**, whereas RLE **expands the file ($100\%$ overhead)** due to lack of repeated adjacent characters.
    - On repetitive bitmap runs (`AAAAABBBB...`), RLE achieves **$70-80\%$ space savings**.

---

### 3. City Navigation System (`PART-B/03_city_navigation_system.c`)
- **Course Outcome:** CO2
- **Problem Statement:** Find shortest path between locations in a city and display turn-by-turn routing.
- **Architectural Design & Justification:**
  - **Adjacency List:** Road networks are sparse planar graphs ($\text{deg}(V) \le 5$). Consumes $\mathcal{O}(V + E)$ space, avoiding the $\mathcal{O}(V^2)$ RAM explosion of adjacency matrices on city-scale maps.
  - **Dijkstra's Algorithm with Min-Heap:** Guarantees globally optimal shortest routes in $\mathcal{O}((V + E) \log V)$ time for non-negative road distances.
  - **Turn-by-Turn Guidance:** Preloaded with Bengaluru landmarks (Majestic, MG Road, Whitefield, Electronic City, Hebbal, Yelahanka BMSIT&M, BLR Airport), computing total distance, segment roads, and travel time.

---

### 4. Mobile Keyboard Auto-Correct (`PART-B/04_mobile_keyboard_autocorrect.c`)
- **Course Outcome:** CO3
- **Problem Statement:** Suggest corrections for misspelled words in a smartphone keyboard.
- **Architectural Design & Justification:**
  - **Trie (Prefix Tree):** Stores dictionary vocabulary with shared prefix paths. Lookups take $\mathcal{O}(L)$ time independent of dictionary size.
  - **Levenshtein Distance DP with Branch Pruning:** Models insertion, deletion, and substitution typos. Traverses the Trie via DFS while propagating DP rows; prunes subtrees whenever $\min(\text{row}) > \text{max\_distance}$. Prunes over $95\%$ of dictionary branches, achieving $<2\text{ ms}$ mobile response times.

---

### 5. Rat in a Maze Pathfinding (`PART-B/05_rat_in_a_maze.c`)
- **Course Outcome:** CO2
- **Problem Statement:** Find a path from start to goal for a robot navigating a grid with obstacles, moving Right, Down, Left, or Up.
- **Architectural Design & Justification:**
  - **Recursive Backtracking (DFS):** Explores the 4-directional search space. Upon encountering obstacles or dead-ends, it rolls back state and tries alternative branches.
  - **Visited Marking:** Prevents infinite looping in cyclic open corridors.
  - **Visual Grid Output:** Renders the maze with obstacles `#`, passages `.`, start `S`, destination `E`, and solution trajectory `*`.

---

### 6. Campus Fiber Optic Network Design (`PART-B/06_campus_network_design.c`)
- **Course Outcome:** CO2
- **Problem Statement:** Connect all campus buildings with fiber optic cables at minimum total cost.
- **Architectural Design & Justification:**
  - **Minimum Spanning Tree (MST):** Connects $V$ buildings using exactly $(V - 1)$ edges with minimum aggregate cable length and zero redundant loops.
  - **Kruskal's Algorithm with Disjoint Set Union (DSU):** Uses Path Compression and Union by Rank ($\mathcal{O}(\alpha(V)) \approx \mathcal{O}(1)$). Total time complexity is $\mathcal{O}(E \log E)$, optimal for sparse campus trench pathways.
  - **Cost Estimation:** Calculates total meters of fiber cable and estimated project cost in INR.

---

## 🎯 High-Yield Lab Exam Viva Voce Questions

1. **What is the difference between static and dynamic memory allocation?**  
   *Static memory is allocated on the stack at compile-time with fixed size. Dynamic memory is allocated on the heap at runtime using `malloc`/`calloc` and must be released with `free`.*
2. **How does a circular queue overcome the limitation of a linear queue?**  
   *In a linear queue, when elements are dequeued from the front, `rear` reaches `MAX-1` and falsely reports overflow even when slots are vacant. Circular queues use modulo arithmetic `(rear + 1) % MAX` to wrap around and reuse vacant slots.*
3. **Why does Quick Sort have an $\mathcal{O}(n^2)$ worst-case time complexity?**  
   *When using Lomuto partitioning (last element as pivot) on already sorted or reverse-sorted data, the pivot produces unbalanced partitions of size $0$ and $n-1$, yielding recurrence $T(n) = T(n-1) + \mathcal{O}(n) = \mathcal{O}(n^2)$.*
4. **Why does the greedy approach fail for the 0/1 Knapsack problem?**  
   *Greedy by value/weight ratio cannot divide items. Selecting high-ratio items may leave unfilled capacity that cannot accommodate remaining valuable items. DP evaluates all subsets to find the global optimum.*
5. **Why is an Adjacency List preferred over an Adjacency Matrix for road networks?**  
   *Road networks are planar and sparse ($E \ll V^2$). An Adjacency List requires $\mathcal{O}(V + E)$ space, while an Adjacency Matrix wastes $\mathcal{O}(V^2)$ memory on empty connections.*
6. **How does Huffman Coding achieve lossless compression?**  
   *It uses variable-length prefix codes derived from symbol frequency. Frequent symbols receive shorter bit sequences. Because no code is a prefix of another, decoding is completely deterministic and lossless.*
7. **What is the purpose of Disjoint Set Union (DSU) in Kruskal's algorithm?**  
   *DSU efficiently checks if adding an edge connects two already-connected components. If `findRoot(u) == findRoot(v)`, adding the edge forms a cycle and is discarded in near $\mathcal{O}(1)$ time.*

---

## 👨‍💻 Author & Acknowledgments

- **Course:** Data Structures & Algorithms Laboratory (`1BAIL306`)
- **Institution:** BMS Institute of Technology & Management, Bengaluru
- **Repository Maintainer:** Mithun Gowda M ([@Mithun-gowda-M](https://github.com/Mithun-gowda-M))