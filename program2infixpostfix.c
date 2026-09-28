#include <stdio.h>
#include <ctype.h>

#define MAX 100

char stack[MAX];
int top = -1;

void push(char ch)
{
    if (top < MAX - 1)
        stack[++top] = ch;
}

char pop(void)
{
    if (top >= 0)
        return stack[top--];

    return '\0';
}

char peek(void)
{
    if (top >= 0)
        return stack[top];

    return '\0';
}

int precedence(char ch)
{
    switch (ch)
    {
        case '^':
            return 3;
        case '*':
        case '/':
            return 2;
        case '+':
        case '-':
            return 1;
        default:
            return 0;
    }
}

void infixToPostfix(const char infix[], char postfix[])
{
    int i = 0;
    int j = 0;
    char ch;

    while ((ch = infix[i]) != '\0')
    {
        if (isspace((unsigned char)ch))
        {
            i++;
            continue;
        }

        if (isalnum((unsigned char)ch))
        {
            postfix[j++] = ch;
        }
        else if (ch == '(')
        {
            push(ch);
        }
        else if (ch == ')')
        {
            while (top != -1 && peek() != '(')
                postfix[j++] = pop();

            if (top != -1 && peek() == '(')
                pop();  // Discard '('
        }
        else
        {
            while (top != -1 && peek() != '(' &&
                   (precedence(peek()) > precedence(ch) ||
                    (precedence(peek()) == precedence(ch) && ch != '^')))
            {
                postfix[j++] = pop();
            }

            push(ch);
        }

        i++;
    }

    while (top != -1)
        postfix[j++] = pop();

    postfix[j] = '\0';
}

int main(void)
{
    char infix[MAX];
    char postfix[MAX];

    printf("Enter infix expression: ");
    if (scanf("%99s", infix) != 1)
        return 1;

    infixToPostfix(infix, postfix);

    printf("Postfix expression: %s\n", postfix);

    return 0;
}