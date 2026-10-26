/**
 * ============================================================================
 * BMS INSTITUTE OF TECHNOLOGY & MANAGEMENT
 * DEPARTMENT OF ARTIFICIAL INTELLIGENCE AND MACHINE LEARNING
 * Course: Data Structures & Algorithms Laboratory (Course Code: 1BAIL306)
 * Semester: III | Scheme: 2025
 * ----------------------------------------------------------------------------
 * PART B - OPEN ENDED EXPERIMENT 2: File Compression Tool
 * 
 * Problem Statement:
 *   A data storage company needs to reduce file sizes for archiving.
 *   Design and implement a compression algorithm without using built-in
 *   compression libraries. Compare your compression ratio with a simple
 *   run-length encoding (RLE) approach. (CO3)
 * 
 * Chosen Algorithms & Data Structures:
 *   1. Huffman Coding:
 *      - Lossless prefix-tree compression based on symbol frequencies.
 *      - Implemented using a Min-Priority Queue (Binary Min-Heap) to construct
 *        the optimal prefix code tree in O(N log K) time.
 *      - Assigns shorter variable-length bit codes to frequent characters and
 *        longer codes to rare characters.
 *   2. Run-Length Encoding (RLE):
 *      - Linear scan compression encoding consecutive identical characters as
 *        a count followed by the character.
 * 
 * Justification of Algorithm Choices:
 *   - Huffman coding is optimal for natural language text and arbitrary data
 *     with non-uniform symbol probability distributions (Shannon Entropy).
 *   - RLE is computationally trivial O(N) and optimal only for long contiguous
 *     runs of repetitive bytes (e.g. bitmaps, sparse files), but often causes
 *     file expansion on natural language text!
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TREE_HT 256
#define NUM_CHARS 256

/* --- HUFFMAN CODING STRUCTURES --- */

typedef struct HuffmanNode {
    char data;
    unsigned freq;
    struct HuffmanNode *left, *right;
} HuffmanNode;

typedef struct {
    unsigned size;
    unsigned capacity;
    HuffmanNode** array;
} MinPriorityQueue;

/* Allocate a new Huffman node */
HuffmanNode* createHuffmanNode(char data, unsigned freq) {
    HuffmanNode* temp = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

/* Allocate Min Priority Queue */
MinPriorityQueue* createMinQueue(unsigned capacity) {
    MinPriorityQueue* minQueue = (MinPriorityQueue*)malloc(sizeof(MinPriorityQueue));
    minQueue->size = 0;
    minQueue->capacity = capacity;
    minQueue->array = (HuffmanNode**)malloc(minQueue->capacity * sizeof(HuffmanNode*));
    return minQueue;
}

void swapNodes(HuffmanNode** a, HuffmanNode** b) {
    HuffmanNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(MinPriorityQueue* minQueue, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < (int)minQueue->size && minQueue->array[left]->freq < minQueue->array[smallest]->freq)
        smallest = left;

    if (right < (int)minQueue->size && minQueue->array[right]->freq < minQueue->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapNodes(&minQueue->array[smallest], &minQueue->array[idx]);
        minHeapify(minQueue, smallest);
    }
}

HuffmanNode* extractMin(MinPriorityQueue* minQueue) {
    HuffmanNode* temp = minQueue->array[0];
    minQueue->array[0] = minQueue->array[minQueue->size - 1];
    --minQueue->size;
    minHeapify(minQueue, 0);
    return temp;
}

void insertMinQueue(MinPriorityQueue* minQueue, HuffmanNode* node) {
    ++minQueue->size;
    int i = minQueue->size - 1;
    while (i && node->freq < minQueue->array[(i - 1) / 2]->freq) {
        minQueue->array[i] = minQueue->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minQueue->array[i] = node;
}

int isLeaf(HuffmanNode* root) {
    return !(root->left) && !(root->right);
}

/* Build Huffman Tree */
HuffmanNode* buildHuffmanTree(const char* data, const unsigned freq[], int size) {
    MinPriorityQueue* minQueue = createMinQueue(size);
    for (int i = 0; i < size; ++i) {
        insertMinQueue(minQueue, createHuffmanNode(data[i], freq[i]));
    }

    while (minQueue->size != 1) {
        HuffmanNode* left = extractMin(minQueue);
        HuffmanNode* right = extractMin(minQueue);

        HuffmanNode* top = createHuffmanNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;

        insertMinQueue(minQueue, top);
    }

    HuffmanNode* root = extractMin(minQueue);
    free(minQueue->array);
    free(minQueue);
    return root;
}

/* Traverse Huffman Tree to store bit codes */
void storeCodes(HuffmanNode* root, int arr[], int top, char codes[NUM_CHARS][MAX_TREE_HT]) {
    if (root->left) {
        arr[top] = 0;
        storeCodes(root->left, arr, top + 1, codes);
    }
    if (root->right) {
        arr[top] = 1;
        storeCodes(root->right, arr, top + 1, codes);
    }
    if (isLeaf(root)) {
        int i;
        for (i = 0; i < top; ++i) {
            codes[(unsigned char)root->data][i] = (char)('0' + arr[i]);
        }
        codes[(unsigned char)root->data][i] = '\0';
    }
}

/* Free Huffman Tree */
void freeHuffmanTree(HuffmanNode* root) {
    if (root == NULL) return;
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right);
    free(root);
}

/* --- RUN LENGTH ENCODING (RLE) IMPLEMENTATION --- */

void compressRLE(const char* input, char* output) {
    int len = (int)strlen(input);
    int outIdx = 0;

    for (int i = 0; i < len; i++) {
        int count = 1;
        while (i + 1 < len && input[i] == input[i + 1] && count < 99) {
            count++;
            i++;
        }
        outIdx += sprintf(&output[outIdx], "%d%c", count, input[i]);
    }
    output[outIdx] = '\0';
}

void decompressRLE(const char* input, char* output) {
    int inLen = (int)strlen(input);
    int outIdx = 0;
    int i = 0;

    while (i < inLen) {
        int count = 0;
        while (i < inLen && isdigit((unsigned char)input[i])) {
            count = count * 10 + (input[i] - '0');
            i++;
        }
        if (i < inLen) {
            char ch = input[i++];
            for (int k = 0; k < count; k++) {
                output[outIdx++] = ch;
            }
        }
    }
    output[outIdx] = '\0';
}

/* 
 * Benchmark & Compare Huffman vs RLE on a given text
 */
void evaluateCompression(const char* testLabel, const char* text) {
    size_t origBytes = strlen(text);
    if (origBytes == 0) return;

    printf("\n========================================================================\n");
    printf("   COMPRESSION TEST CASE: %s\n", testLabel);
    printf("========================================================================\n");
    printf("Original Text Preview : \"%.60s%s\"\n", text, (origBytes > 60) ? "..." : "");
    printf("Original Size         : %zu bytes (%zu bits)\n\n", origBytes, origBytes * 8);

    // 1. Run-Length Encoding (RLE)
    char* rleOutput = (char*)malloc(origBytes * 3 + 10);
    compressRLE(text, rleOutput);
    size_t rleBytes = strlen(rleOutput);
    double rleRatio = (1.0 - ((double)rleBytes / origBytes)) * 100.0;

    printf("--- 1. RUN-LENGTH ENCODING (RLE) ---\n");
    printf("  RLE Encoded Output   : \"%.50s%s\"\n", rleOutput, (rleBytes > 50) ? "..." : "");
    printf("  RLE Compressed Size  : %zu bytes\n", rleBytes);
    printf("  RLE Compression Ratio: %+.2f%% (%s)\n\n",
           rleRatio, (rleBytes < origBytes) ? "Space Saved" : "FILE EXPANDED!");

    // 2. Huffman Coding
    unsigned freq[NUM_CHARS] = {0};
    for (size_t i = 0; i < origBytes; i++) {
        freq[(unsigned char)text[i]]++;
    }

    char distinctChars[NUM_CHARS];
    unsigned distinctFreqs[NUM_CHARS];
    int numDistinct = 0;

    for (int i = 0; i < NUM_CHARS; i++) {
        if (freq[i] > 0) {
            distinctChars[numDistinct] = (char)i;
            distinctFreqs[numDistinct] = freq[i];
            numDistinct++;
        }
    }

    char codes[NUM_CHARS][MAX_TREE_HT];
    for (int i = 0; i < NUM_CHARS; i++) codes[i][0] = '\0';

    HuffmanNode* root = buildHuffmanTree(distinctChars, distinctFreqs, numDistinct);
    int bitArr[MAX_TREE_HT];
    storeCodes(root, bitArr, 0, codes);

    size_t totalHuffmanBits = 0;
    for (size_t i = 0; i < origBytes; i++) {
        totalHuffmanBits += strlen(codes[(unsigned char)text[i]]);
    }
    size_t huffmanBytes = (totalHuffmanBits + 7) / 8;
    double huffmanRatio = (1.0 - ((double)huffmanBytes / origBytes)) * 100.0;

    printf("--- 2. HUFFMAN CODING ---\n");
    printf("  Distinct Characters  : %d\n", numDistinct);
    printf("  Sample Prefix Codes  :\n");
    int shown = 0;
    for (int i = 0; i < numDistinct && shown < 6; i++) {
        char ch = distinctChars[i];
        if (isprint((unsigned char)ch)) {
            printf("    '%c' (Freq: %3u) -> Code: %s\n", ch, freq[(unsigned char)ch], codes[(unsigned char)ch]);
        } else {
            printf("    [0x%02X] (Freq: %3u) -> Code: %s\n", (unsigned char)ch, freq[(unsigned char)ch], codes[(unsigned char)ch]);
        }
        shown++;
    }
    printf("  Huffman Compressed   : %zu bits (%zu bytes)\n", totalHuffmanBits, huffmanBytes);
    printf("  Huffman Ratio        : %+.2f%% Space Saved\n\n", huffmanRatio);

    // Summary Comparison Table
    printf("--- PERFORMANCE COMPARISON SUMMARY ---\n");
    printf("| %-20s | %-16s | %-16s | %-12s |\n", "Method", "Original (bytes)", "Compressed (bytes)", "Space Saved");
    printf("|----------------------|------------------|--------------------|--------------|\n");
    printf("| Original Raw Text    | %-16zu | %-18zu | %-12s |\n", origBytes, origBytes, "0.00%");
    printf("| Run-Length (RLE)     | %-16zu | %-18zu | %+-11.2f%% |\n", origBytes, rleBytes, rleRatio);
    printf("| Huffman Coding       | %-16zu | %-18zu | %+-11.2f%% |\n", origBytes, huffmanBytes, huffmanRatio);
    printf("------------------------------------------------------------------------\n");

    if (huffmanBytes < rleBytes) {
        printf(">>> WINNER: Huffman Coding outperformed RLE by %.2f%%.\n", huffmanRatio - rleRatio);
    } else {
        printf(">>> WINNER: RLE outperformed Huffman Coding due to long sequential runs.\n");
    }
    printf("========================================================================\n\n");

    free(rleOutput);
    freeHuffmanTree(root);
}

/* Display Design Justification */
void displayJustification(void) {
    printf("\n========================================================================================\n");
    printf("      ARCHITECTURAL JUSTIFICATION: HUFFMAN CODING vs RUN-LENGTH ENCODING (CO3)          \n");
    printf("========================================================================================\n");
    printf("1. Theoretical Basis for Huffman Coding:\n");
    printf("   - Natural language text exhibits highly skewed character frequency (e.g. 'e', 't', 'a'\n");
    printf("     appear much more frequently than 'q', 'x', 'z').\n");
    printf("   - Fixed 8-bit ASCII encoding wastes bandwidth on frequent symbols.\n");
    printf("   - Huffman coding satisfies the Shannon Source Coding Theorem by achieving code lengths\n");
    printf("     inversely proportional to symbol probabilities (L ~ H(X)).\n");
    printf("   - Prefix Property: No code is a prefix of another, enabling deterministic decoding\n");
    printf("     without delimiter overhead.\n\n");
    printf("2. Why Run-Length Encoding (RLE) Fails on Typical Text:\n");
    printf("   - RLE relies strictly on adjacent character repetition ('AAAAA' -> '5A').\n");
    printf("   - In normal text, adjacent repeated characters are rare. Replacing 'cat' with '1c1a1t'\n");
    printf("     DOUBLES the file size (100%% expansion)!\n");
    printf("   - RLE is only suitable for raw graphics, binary mask images, or repetitive sensor logs.\n");
    printf("========================================================================================\n\n");
}

int main(void) {
    int choice;

    const char* sampleNaturalText =
        "Data structures and algorithms laboratory practical examination scheme 2025. "
        "Dynamic programming solves problems by combining optimal solutions to subproblems. "
        "Lossless compression algorithms preserve every bit of data upon decompression.";

    const char* sampleRepetitiveText =
        "AAAAAAAAAAAAABBBBBBBBBBBBBBCCCCCCCCCCCCCCDDDDDDDDDDDDDDDDDDDDDDDDDD"
        "EEEEEEEEEEEEEEEEEEEEEEFFFFFFFFFFFFFFFFFFFFGGGGGGGGGGGGGGGGGGGGGGGGGG"
        "111111111111111111111111111111111111111110000000000000000000000000";

    printf("========================================================================\n");
    printf("    BMSIT&M - DSA LAB - PART B: FILE COMPRESSION TOOL (CO3)             \n");
    printf("========================================================================\n");

    while (1) {
        printf("\n--- MENU ---\n");
        printf("1. Test on Natural Language Text (Shows Huffman Superiority)\n");
        printf("2. Test on Highly Repetitive Run Data (Shows RLE Advantage)\n");
        printf("3. Test on Custom User String\n");
        printf("4. View Design & Algorithmic Justification (Viva Voce Prep)\n");
        printf("5. Exit\n");
        printf("Enter choice (1-5): ");

        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
            case 1:
                evaluateCompression("Natural English Prose", sampleNaturalText);
                break;

            case 2:
                evaluateCompression("Highly Repetitive Character Runs", sampleRepetitiveText);
                break;

            case 3: {
                char userText[2048];
                printf("\nEnter custom text to compress (up to 2000 chars):\n");
                while (getchar() != '\n');
                if (fgets(userText, sizeof(userText), stdin) != NULL) {
                    userText[strcspn(userText, "\r\n")] = '\0';
                    evaluateCompression("User Input Text", userText);
                }
                break;
            }

            case 4:
                displayJustification();
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
