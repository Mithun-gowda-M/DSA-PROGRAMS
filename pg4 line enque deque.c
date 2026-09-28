#include <stdio.h>

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
        printf("\n--- QUEUE MENU ---\n");
        printf("1. Enqueue\n");
        printf("2. Dequeue\n");
        printf("3. Display\n");
        printf("4. Exit\n");

        printf("Enter the option: ");
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
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid option\n");
        }
    }

    return 0;
}

void enqueue(void)
{
    int element;

    if (rear == n - 1)
    {
        printf("Queue is full\n");
    }
    else
    {
        if (front == -1)
            front = 0;

        printf("Enter the element: ");
        scanf("%d", &element);

        rear++;
        queue[rear] = element;

        printf("Element inserted\n");
    }
}

void dequeue(void)
{
    if (front == -1)
    {
        printf("Queue is empty\n");
    }
    else
    {
        printf("Deleted element: %d\n", queue[front]);

        if (front == rear)
        {
            front = -1;
            rear = -1;
        }
        else
        {
            front++;
        }
    }
}

void display(void)
{
    int i;

    if (front == -1)
    {
        printf("Queue is empty\n");
    }
    else
    {
        printf("Queue elements are:\n");

        for (i = front; i <= rear; i++)
        {
            printf("%d ", queue[i]);
        }

        printf("\n");
    }
}