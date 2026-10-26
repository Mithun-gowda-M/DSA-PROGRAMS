/**
 * ============================================================================
 * BMS INSTITUTE OF TECHNOLOGY & MANAGEMENT
 * DEPARTMENT OF ARTIFICIAL INTELLIGENCE AND MACHINE LEARNING
 * Course: Data Structures & Algorithms Laboratory (Course Code: 1BAIL306)
 * Semester: III | Scheme: 2025
 * ----------------------------------------------------------------------------
 * PART A - EXPERIMENT 2: Stack Application - Infix to Postfix Conversion
 * 
 * Objective:
 *   Implement a stack using an array in C.
 *   Use it to convert a given infix expression (containing operators +, -, *, /
 *   and parentheses) to its equivalent postfix expression.
 *   Assume single-digit operands (or single-character variables).
 *   Display the postfix expression.
 *   Test with at least 4 different expressions. (CO1)
 * 
 * Time & Space Complexity:
 *   - Infix to Postfix Conversion: Time: O(n), Space: O(n)
 *     where n is the length of the infix expression.
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STACK_SIZE 100

/* Array-based Stack structure */
typedef struct {
    char items[MAX_STACK_SIZE];
    int top;
} Stack;

/* Initialize stack */
void initStack(Stack* s) {
    s->top = -1;
}

/* Check if stack is empty */
int isEmpty(Stack* s) {
    return s->top == -1;
}

/* Check if stack is full */
int isFull(Stack* s) {
    return s->top == MAX_STACK_SIZE - 1;
}

/* Push item onto stack */
void push(Stack* s, char c) {
    if (isFull(s)) {
        printf("[Error] Stack Overflow!\n");
        return;
    }
    s->items[++(s->top)] = c;
}

/* Pop item from stack */
char pop(Stack* s) {
    if (isEmpty(s)) {
        printf("[Error] Stack Underflow!\n");
        return '\0';
    }
    return s->items[(s->top)--];
}

/* Peek top element of stack */
char peek(Stack* s) {
    if (isEmpty(s)) {
        return '\0';
    }
    return s->items[s->top];
}

/* Check operator precedence */
int precedence(char op) {
    switch (op) {
        case '^':
            return 3;
        case '*':
        case '/':
        case '%':
            return 2;
        case '+':
        case '-':
            return 1;
        default:
            return 0; // For '('
    }
}

/* Check if character is an operator */
int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^');
}

/* Check associativity: 1 for right-to-left ('^'), 0 for left-to-right */
int isRightAssociative(char op) {
    return (op == '^');
}

/* 
 * Infix to Postfix conversion function
 * Displays step-by-step trace of conversion
 */
int infixToPostfix(const char* infix, char* postfix, int showTrace) {
    Stack s;
    initStack(&s);
    int k = 0;

    if (showTrace) {
        printf("\n+-------+--------------------+------------------------------+\n");
        printf("| Char  | Stack Contents     | Postfix Expression So Far    |\n");
        printf("+-------+--------------------+------------------------------+\n");
    }

    for (int i = 0; infix[i] != '\0'; i++) {
        char ch = infix[i];

        // Skip whitespace
        if (isspace((unsigned char)ch)) {
            continue;
        }

        // 1. If character is an operand, add to postfix
        if (isalnum((unsigned char)ch)) {
            postfix[k++] = ch;
            postfix[k] = '\0';
        }
        // 2. If left parenthesis, push to stack
        else if (ch == '(') {
            push(&s, ch);
        }
        // 3. If right parenthesis, pop until left parenthesis
        else if (ch == ')') {
            while (!isEmpty(&s) && peek(&s) != '(') {
                postfix[k++] = pop(&s);
                postfix[k] = '\0';
            }
            if (isEmpty(&s)) {
                printf("[Error] Mismatched parentheses in expression!\n");
                return 0; // Syntax error
            }
            pop(&s); // Remove '('
        }
        // 4. If operator encountered
        else if (isOperator(ch)) {
            while (!isEmpty(&s) && peek(&s) != '(') {
                int precCurrent = precedence(ch);
                int precTop = precedence(peek(&s));

                if (precTop > precCurrent || (precTop == precCurrent && !isRightAssociative(ch))) {
                    postfix[k++] = pop(&s);
                    postfix[k] = '\0';
                } else {
                    break;
                }
            }
            push(&s, ch);
        } else {
            printf("[Warning] Ignoring invalid character: '%c'\n", ch);
        }

        if (showTrace) {
            char stackStr[MAX_STACK_SIZE + 1];
            int idx = 0;
            for (int j = 0; j <= s.top; j++) {
                stackStr[idx++] = s.items[j];
            }
            stackStr[idx] = '\0';
            printf("| %-5c | %-18s | %-28s |\n", ch, stackStr, postfix);
        }
    }

    // Pop remaining operators from stack
    while (!isEmpty(&s)) {
        char topOp = pop(&s);
        if (topOp == '(' || topOp == ')') {
            printf("[Error] Mismatched parentheses in expression!\n");
            return 0;
        }
        postfix[k++] = topOp;
        postfix[k] = '\0';

        if (showTrace) {
            char stackStr[MAX_STACK_SIZE + 1];
            int idx = 0;
            for (int j = 0; j <= s.top; j++) {
                stackStr[idx++] = s.items[j];
            }
            stackStr[idx] = '\0';
            printf("| (pop) | %-18s | %-28s |\n", stackStr, postfix);
        }
    }

    postfix[k] = '\0';
    if (showTrace) {
        printf("+-------+--------------------+------------------------------+\n");
    }
    return 1;
}

/* Run the mandatory 4 test cases prescribed by syllabus */
void runAutomatedTests(void) {
    const char* testCases[] = {
        "(A+B)*(C-D)",
        "A+B*C-D/E",
        "(A+B*C)/(D-E)",
        "2+3*(4-1)",
        "A*(B+C)/D+E"
    };
    int numTests = sizeof(testCases) / sizeof(testCases[0]);

    printf("\n===============================================================\n");
    printf("         AUTOMATED TEST SUITE (MINIMUM 4 EXPRESSIONS)          \n");
    printf("===============================================================\n");

    for (int i = 0; i < numTests; i++) {
        char postfix[MAX_STACK_SIZE];
        printf("\n--- Test Case %d ---\n", i + 1);
        printf("Infix Expression  : %s\n", testCases[i]);
        if (infixToPostfix(testCases[i], postfix, 1)) {
            printf("Postfix Result    : %s\n", postfix);
        } else {
            printf("Conversion Failed!\n");
        }
    }
    printf("\n===============================================================\n");
    printf("All automated tests completed successfully.\n");
    printf("===============================================================\n");
}

int main(void) {
    int choice;
    char infix[MAX_STACK_SIZE];
    char postfix[MAX_STACK_SIZE];

    printf("===============================================================\n");
    printf("  BMSIT&M - DSA LAB - EXPERIMENT 2: INFIX TO POSTFIX CONVERSION\n");
    printf("===============================================================\n");

    while (1) {
        printf("\n--- MENU ---\n");
        printf("1. Run Automated Tests (5 Prescribed Expressions with Trace)\n");
        printf("2. Convert Custom Infix Expression\n");
        printf("3. View Algorithm & Time Complexity\n");
        printf("4. Exit\n");
        printf("Enter your choice (1-4): ");

        if (scanf("%d", &choice) != 1) {
            printf("[Error] Invalid input. Exiting.\n");
            break;
        }

        // Clear newline from buffer
        while (getchar() != '\n');

        switch (choice) {
            case 1:
                runAutomatedTests();
                break;

            case 2:
                printf("\nEnter infix expression (e.g. (A+B)*C or 2+3*4): ");
                if (fgets(infix, sizeof(infix), stdin) != NULL) {
                    // Remove newline
                    infix[strcspn(infix, "\r\n")] = '\0';
                    if (strlen(infix) == 0) {
                        printf("[Warning] Empty expression provided.\n");
                        break;
                    }
                    if (infixToPostfix(infix, postfix, 1)) {
                        printf("\n[Result]\n");
                        printf("Infix   : %s\n", infix);
                        printf("Postfix : %s\n", postfix);
                    }
                }
                break;

            case 3:
                printf("\n===============================================================\n");
                printf("             INFIX TO POSTFIX: COMPLEXITY ANALYSIS             \n");
                printf("===============================================================\n");
                printf("Time Complexity:\n");
                printf("  - Parsing expression of length N: O(N) operations.\n");
                printf("  - Each symbol is pushed and popped at most once: O(1) each.\n");
                printf("  - Overall Time Complexity: O(N)\n\n");
                printf("Space Complexity:\n");
                printf("  - Stack storage for operators and parentheses: O(N) auxiliary.\n");
                printf("  - Postfix output string buffer: O(N).\n");
                printf("  - Overall Space Complexity: O(N)\n");
                printf("===============================================================\n");
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
