#include <stdio.h>
#include <ctype.h>

#define MAX 100

int stack[MAX];
int top = -1;

void push(int value)
{
    top++;
    stack[top] = value;
}

int pop()
{
    int value = stack[top];
    top--;
    return value;
}

int evaluatePostfix(char postfix[])
{
    int i;
    int a, b;
    char ch;

    for (i = 0; postfix[i] != '\0'; i++)
    {
        ch = postfix[i];

        if (isdigit(ch))
        {
            push(ch - '0');
        }
        else
        {
            b = pop();
            a = pop();

            switch (ch)
            {
                case '+':
                    push(a + b);
                    break;

                case '-':
                    push(a - b);
                    break;

                case '*':
                    push(a * b);
                    break;

                case '/':
                    push(a / b);
                    break;
            }
        }
    }

    return pop();
}

int main()
{
    char postfix[MAX];
    int result;

    printf("Enter postfix expression: ");
    scanf("%99s", postfix);

    result = evaluatePostfix(postfix);

    printf("Result = %d\n", result);

    return 0;
}