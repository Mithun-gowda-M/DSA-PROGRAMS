/**
 * ============================================================================
 * BMS INSTITUTE OF TECHNOLOGY & MANAGEMENT
 * DEPARTMENT OF ARTIFICIAL INTELLIGENCE AND MACHINE LEARNING
 * Course: Data Structures & Algorithms Laboratory (Course Code: 1BAIL306)
 * Semester: III | Scheme: 2025
 * ----------------------------------------------------------------------------
 * PART B - OPEN ENDED EXPERIMENT 1: Text Analysis for Search Engine
 * 
 * Problem Statement:
 *   A search engine company wants to identify the most relevant words in large
 *   documents to improve search ranking. Design and implement a solution to
 *   find the most frequently occurring words from a text file, ignoring common
 *   words like "the", "is", "and" if needed. Choose appropriate data structures
 *   and justify your choices. (CO3)
 * 
 * Chosen Data Structures & Architectural Justification:
 *   1. Hash Table with Chaining (djb2 hash function):
 *      - Justification: Provides average O(1) time complexity for word lookups,
 *        insertions, and frequency increments across massive document sizes.
 *        Significantly faster than O(N) array search or O(log N) BST.
 *   2. Stop-Words Hash Set:
 *      - Justification: Filters out ubiquitous non-informative words (the, is,
 *        and, of, to, in, at, that, etc.) in O(1) time prior to counting.
 *   3. Min-Heap of Size K:
 *      - Justification: Extracts the Top-K most frequent terms in O(N log K)
 *        time, without needing to sort the entire vocabulary O(N log N).
 * 
 * Time & Space Complexities:
 *   - Word Parsing & Frequency Counting: O(Total Words)
 *   - Stop-Word Filtering: O(1) average per word
 *   - Top-K Extraction using Min-Heap: O(Vocabulary Size * log K)
 *   - Overall Time Complexity: O(M + V log K) where M = total words, V = unique words
 *   - Space Complexity: O(V) for Hash Table and Heap
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HASH_TABLE_SIZE 10007
#define MAX_WORD_LEN 64
#define DEFAULT_TOP_K 10

/* Hash Table Node for Word Frequencies */
typedef struct WordNode {
    char word[MAX_WORD_LEN];
    int count;
    struct WordNode* next;
} WordNode;

/* Hash Table structure */
typedef struct {
    WordNode* buckets[HASH_TABLE_SIZE];
    int uniqueWordsCount;
    int totalWordsCount;
} WordHashTable;

/* Min-Heap Element for Top-K tracking */
typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} HeapElement;

typedef struct {
    HeapElement* data;
    int size;
    int capacity;
} MinHeap;

/* List of common English stop words */
static const char* STOP_WORDS[] = {
    "a", "about", "above", "after", "again", "against", "all", "am", "an", "and",
    "any", "are", "as", "at", "be", "because", "been", "before", "being", "below",
    "between", "both", "but", "by", "could", "did", "do", "does", "doing", "down",
    "during", "each", "few", "for", "from", "further", "had", "has", "have", "having",
    "he", "her", "here", "hers", "herself", "him", "himself", "his", "how", "i",
    "if", "in", "into", "is", "it", "its", "itself", "just", "me", "more", "most",
    "my", "myself", "no", "nor", "not", "now", "of", "off", "on", "once", "only",
    "or", "other", "our", "ours", "ourselves", "out", "over", "own", "same", "she",
    "should", "so", "some", "such", "than", "that", "the", "their", "theirs", "them",
    "themselves", "then", "there", "these", "they", "this", "those", "through", "to",
    "too", "under", "until", "up", "very", "was", "we", "were", "what", "when",
    "where", "which", "while", "who", "whom", "why", "with", "would", "you", "your",
    "yours", "yourself", "yourselves"
};
static const int NUM_STOP_WORDS = sizeof(STOP_WORDS) / sizeof(STOP_WORDS[0]);

/* djb2 string hashing function */
unsigned long hashDJB2(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char)*str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash % HASH_TABLE_SIZE;
}

/* Binary search check for stop words */
int isStopWord(const char* word) {
    int low = 0, high = NUM_STOP_WORDS - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        int cmp = strcmp(word, STOP_WORDS[mid]);
        if (cmp == 0) return 1;
        if (cmp < 0) high = mid - 1;
        else low = mid + 1;
    }
    return 0;
}

/* Initialize Hash Table */
void initHashTable(WordHashTable* ht) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        ht->buckets[i] = NULL;
    }
    ht->uniqueWordsCount = 0;
    ht->totalWordsCount = 0;
}

/* Insert or increment word in Hash Table */
void insertWord(WordHashTable* ht, const char* word) {
    unsigned long index = hashDJB2(word);
    WordNode* curr = ht->buckets[index];

    while (curr != NULL) {
        if (strcmp(curr->word, word) == 0) {
            curr->count++;
            ht->totalWordsCount++;
            return;
        }
        curr = curr->next;
    }

    // Insert new node
    WordNode* newNode = (WordNode*)malloc(sizeof(WordNode));
    strncpy(newNode->word, word, MAX_WORD_LEN - 1);
    newNode->word[MAX_WORD_LEN - 1] = '\0';
    newNode->count = 1;
    newNode->next = ht->buckets[index];
    ht->buckets[index] = newNode;

    ht->uniqueWordsCount++;
    ht->totalWordsCount++;
}

/* Min-Heap operations for Top-K */
MinHeap* createMinHeap(int capacity) {
    MinHeap* h = (MinHeap*)malloc(sizeof(MinHeap));
    h->data = (HeapElement*)malloc(capacity * sizeof(HeapElement));
    h->size = 0;
    h->capacity = capacity;
    return h;
}

void minHeapify(MinHeap* h, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < h->size && h->data[left].count < h->data[smallest].count)
        smallest = left;
    if (right < h->size && h->data[right].count < h->data[smallest].count)
        smallest = right;

    if (smallest != idx) {
        HeapElement temp = h->data[idx];
        h->data[idx] = h->data[smallest];
        h->data[smallest] = temp;
        minHeapify(h, smallest);
    }
}

void addToTopKHeap(MinHeap* h, const char* word, int count) {
    if (h->size < h->capacity) {
        // Insert into heap
        int i = h->size++;
        strncpy(h->data[i].word, word, MAX_WORD_LEN - 1);
        h->data[i].word[MAX_WORD_LEN - 1] = '\0';
        h->data[i].count = count;

        // Bubble up
        while (i != 0 && h->data[(i - 1) / 2].count > h->data[i].count) {
            HeapElement temp = h->data[i];
            h->data[i] = h->data[(i - 1) / 2];
            h->data[(i - 1) / 2] = temp;
            i = (i - 1) / 2;
        }
    } else if (count > h->data[0].count) {
        // Replace root if count is greater than the smallest in top-k
        strncpy(h->data[0].word, word, MAX_WORD_LEN - 1);
        h->data[0].word[MAX_WORD_LEN - 1] = '\0';
        h->data[0].count = count;
        minHeapify(h, 0);
    }
}

/* Sort heap elements descending for display */
int compareHeapDesc(const void* a, const void* b) {
    const HeapElement* elA = (const HeapElement*)a;
    const HeapElement* elB = (const HeapElement*)b;
    return elB->count - elA->count;
}

/* Process raw text string */
void processDocumentText(WordHashTable* ht, const char* text, int filterStopWords) {
    char currentWord[MAX_WORD_LEN];
    int wordIdx = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        char ch = text[i];
        if (isalnum((unsigned char)ch)) {
            if (wordIdx < MAX_WORD_LEN - 1) {
                currentWord[wordIdx++] = (char)tolower((unsigned char)ch);
            }
        } else {
            if (wordIdx > 0) {
                currentWord[wordIdx] = '\0';
                if (!filterStopWords || !isStopWord(currentWord)) {
                    insertWord(ht, currentWord);
                }
                wordIdx = 0;
            }
        }
    }

    if (wordIdx > 0) {
        currentWord[wordIdx] = '\0';
        if (!filterStopWords || !isStopWord(currentWord)) {
            insertWord(ht, currentWord);
        }
    }
}

/* Display Top-K frequent terms */
void displayTopKWords(WordHashTable* ht, int k) {
    if (k > ht->uniqueWordsCount) {
        k = ht->uniqueWordsCount;
    }
    if (k <= 0) {
        printf("[Info] No words to display.\n");
        return;
    }

    MinHeap* heap = createMinHeap(k);

    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        WordNode* curr = ht->buckets[i];
        while (curr != NULL) {
            addToTopKHeap(heap, curr->word, curr->count);
            curr = curr->next;
        }
    }

    // Sort top-k descending
    qsort(heap->data, heap->size, sizeof(HeapElement), compareHeapDesc);

    printf("\n========================================================================\n");
    printf("         TOP %d MOST FREQUENT KEYWORDS FOR SEARCH INDEXING              \n", heap->size);
    printf("========================================================================\n");
    printf("| %-5s | %-24s | %-12s | %-16s |\n", "Rank", "Keyword / Term", "Frequency", "Term Frequency %%");
    printf("|-------|--------------------------|--------------|------------------|\n");

    for (int i = 0; i < heap->size; i++) {
        double tf = ((double)heap->data[i].count / ht->totalWordsCount) * 100.0;
        printf("| %-5d | %-24s | %-12d | %-15.2f%% |\n",
               i + 1, heap->data[i].word, heap->data[i].count, tf);
    }
    printf("========================================================================\n");
    printf("Document Statistics: Total Processed Words = %d, Unique Terms = %d\n\n",
           ht->totalWordsCount, ht->uniqueWordsCount);

    free(heap->data);
    free(heap);
}

/* Free hash table */
void freeHashTable(WordHashTable* ht) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        WordNode* curr = ht->buckets[i];
        while (curr != NULL) {
            WordNode* temp = curr;
            curr = curr->next;
            free(temp);
        }
        ht->buckets[i] = NULL;
    }
    ht->uniqueWordsCount = 0;
    ht->totalWordsCount = 0;
}

/* Architectural Justification Display */
void displayDesignJustification(void) {
    printf("\n========================================================================================\n");
    printf("       DESIGN & ARCHITECTURAL JUSTIFICATION: SEARCH ENGINE TEXT ANALYSIS (CO3)          \n");
    printf("========================================================================================\n");
    printf("1. Hash Table with Separate Chaining:\n");
    printf("   - Why chosen: In search indexing, documents contain millions of words.\n");
    printf("   - Hash Table provides O(1) average lookup and increment time.\n");
    printf("   - Compared to BST: Avoids O(log N) tree overhead and rebalancing operations.\n");
    printf("   - Compared to Unsorted Array: Avoids disastrous O(N^2) quadratic frequency scan.\n\n");
    printf("2. Stop-Words Hash Filtering:\n");
    printf("   - Eliminates grammatical noise ('the', 'is', 'and', 'with') before indexing.\n");
    printf("   - Drastically compresses index size and boosts search relevance (TF-IDF relevance).\n\n");
    printf("3. Min-Heap of Size K for Top-K Keywords:\n");
    printf("   - Traditional Sort: Sorting all V unique vocabulary items requires O(V log V).\n");
    printf("   - Min-Heap Strategy: Maintains only top K candidates -> O(V log K) time and O(K) space.\n");
    printf("   - When K (e.g. 10) is much smaller than V (e.g. 50,000), this saves massive CPU cycles!\n");
    printf("========================================================================================\n\n");
}

int main(void) {
    WordHashTable ht;
    initHashTable(&ht);
    int choice;

    const char* sampleDocument =
        "Data structures and algorithms form the core foundation of computer science. "
        "A search engine relies on inverted index data structures, hash tables, and priority "
        "queues to crawl web pages, rank documents, and return fast search results. Algorithms "
        "such as Dijkstra algorithm, PageRank, and tree traversals enable efficient graph analysis. "
        "Dynamic programming and divide-and-conquer optimize computational resource usage. "
        "Search algorithms process queries rapidly using hash tables and trie prefix trees. "
        "Modern artificial intelligence and machine learning pipelines also require efficient "
        "data structures to store tensors, feature maps, and model parameters.";

    printf("========================================================================\n");
    printf("   BMSIT&M - DSA LAB - PART B: SEARCH ENGINE TEXT ANALYSIS (CO3)        \n");
    printf("========================================================================\n");

    while (1) {
        printf("\n--- MENU ---\n");
        printf("1. Analyze Preloaded Technical Article (With Stop-Word Filtering)\n");
        printf("2. Analyze Preloaded Technical Article (WITHOUT Stop-Word Filtering - Raw)\n");
        printf("3. Analyze Custom Text / Document String\n");
        printf("4. View Architectural & Data Structure Justification (Viva Voce Prep)\n");
        printf("5. Exit\n");
        printf("Enter choice (1-5): ");

        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
            case 1:
                freeHashTable(&ht);
                initHashTable(&ht);
                processDocumentText(&ht, sampleDocument, 1);
                displayTopKWords(&ht, DEFAULT_TOP_K);
                break;

            case 2:
                freeHashTable(&ht);
                initHashTable(&ht);
                processDocumentText(&ht, sampleDocument, 0);
                displayTopKWords(&ht, DEFAULT_TOP_K);
                break;

            case 3: {
                freeHashTable(&ht);
                initHashTable(&ht);
                char buffer[4096];
                printf("\nEnter your text passage (up to 4000 characters):\n");
                while (getchar() != '\n'); // clear buffer
                if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                    processDocumentText(&ht, buffer, 1);
                    int k;
                    printf("Enter K (number of top keywords to display, e.g. 5 or 10): ");
                    if (scanf("%d", &k) == 1 && k > 0) {
                        displayTopKWords(&ht, k);
                    }
                }
                break;
            }

            case 4:
                displayDesignJustification();
                break;

            case 5:
                freeHashTable(&ht);
                printf("Exiting. Goodbye!\n");
                return 0;

            default:
                printf("[Error] Invalid choice! Select 1-5.\n");
        }
    }

    freeHashTable(&ht);
    return 0;
}
