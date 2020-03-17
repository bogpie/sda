#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Node Node;
struct Node
{
    int* adrX;
    Node* next;
};

typedef struct Queue Queue;
struct Queue
{
    Node *front, * rear;
};

typedef struct StackNode StackNode;
struct StackNode
{
    char car;
    StackNode* next;
};

typedef struct Stack Stack;
struct Stack
{
    StackNode* top;
};


void fEnqueue(Queue* queue, int * adrX)
{
    Node* node = malloc(sizeof(Node));
    node->adrX = adrX;
    node->next = NULL;
    if (queue->rear == NULL)
    {
        queue->rear = node;
    }
    else
    {
        queue->rear->next = node;
        queue->rear = node;
    }
    if (queue->front == NULL)
    {
        queue->front = queue->rear;
    }
}

void fCreateQueue(Queue** adrQueue)
{
    Queue* queue = malloc(sizeof(Queue));
    queue->front = queue->rear = NULL;
    *adrQueue = queue;
}


int isEmpty(Queue* queue)
{
    if (queue->front == NULL)
    {
        return 1;
    }
    return 0;
}

int fDeque(Queue* queue)
{
    if (isEmpty(queue)) exit(1);
    Node* dequed = queue->front;
    int returned = *(dequed->adrX);
    queue->front = queue->front->next;
    free(dequed);
    return  returned;
}


void fModify(int ** adrAdrX)
{
    **adrAdrX = -1;
}

int main()
{
    int * adrX;
    Queue* queue;
    fCreateQueue(&queue);

     adrX = malloc(sizeof(int));
   
     *adrX = 123;


  
    fEnqueue(queue, adrX);
    fEnqueue(queue, adrX);
    fEnqueue(queue, adrX);

    fModify(&queue->front->adrX);
    int x = fDeque(queue);
    printf("%d ", x);
     x = fDeque(queue);
    printf("%d ", x);

     x = fDeque(queue);
    printf("%d ", x);

   
    return 0;
}
