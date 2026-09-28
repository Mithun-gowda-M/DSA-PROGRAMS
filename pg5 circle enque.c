#include <stdio.h>
#include <stdlib.h>

void enqueue(void);
void dequeue(void);
void display(void);

int queue[20], front = -1, rear = -1, n, option = 0;

int main(void)
{
    printf("Enter the size of queue: ");
    scanf("%d", &n);

    while (option != 4)
    {
        printf("\n1. Enqueue");
        printf("\n2. Dequeue");
        printf("\n3. Display");
        printf("\n4. Exit");

        printf("\nEnter the option: ");
        scanf("%d", &option);

        switch (option)
        {
            case 1:
                enqueue();
                break;

            case 2:
                dequeue();
                break;

            case 3:
                display();
                break;

            case 4:
                exit(0);

            default:
                printf("Invalid option");
        }
    }

    return 0;
}

void enqueue(void)
{
    int element;

    if ((rear + 1) % n == front)
    {
        printf("Overflow");
    }
    else
    {
        printf("Enter the element: ");
        scanf("%d", &element);

        if (front == -1)
        {
            front = 0;
            rear = 0;
        }
        else
        {
            rear = (rear + 1) % n;
        }

        queue[rear] = element;
    }
}

void dequeue(void)
{
    if (front == -1)
    {
        printf("Underflow");
    }
    else
    {
        printf("Deleted element = %d", queue[front]);

        if (front == rear)
        {
            front = -1;
            rear = -1;
        }
        else
        {
            front = (front + 1) % n;
        }
    }
}

void display(void)
{
    int i;

    if (front == -1)
    {
        printf("Queue is empty");
    }
    else
    {
        printf("Queue elements are: ");

        i = front;

        while (1)
        {
            printf("%d ", queue[i]);

            if (i == rear)
                break;

            i = (i + 1) % n;
        }
    }
}