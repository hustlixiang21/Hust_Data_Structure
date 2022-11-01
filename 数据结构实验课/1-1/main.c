#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define MAXQSIZE 8    //宏定义队列最大容量
#define OK 1
#define ERROR 0

typedef int ElemType;

int size;           //用于记录元素当前个数

typedef struct Node     //定义队列元素结点
{
    ElemType data;
    struct Node *next;
}QueueNode;

typedef struct
{
    QueueNode *base;    //始终指向队列的下标为0处
    QueueNode *front;   //指向队首元素指针
    QueueNode *rear;    //指向队尾元素指针
}LinkQueue;

/*initialize the queue*/
void InitQueue(LinkQueue *q);

/*Destroy the queue*/
void DestroyQueue(LinkQueue *q);

/*determine if the queue is empty*/
bool IsEmpty(LinkQueue q);

/*determine if the queue is full*/
bool IsFull(LinkQueue q);

/*return the head elem of the queue*/
ElemType Top(LinkQueue q);

/*return the back elem of the queue*/
ElemType Back(LinkQueue q);

/*enqueue, insert the rear*/
bool EnQueue(LinkQueue *q, ElemType e);

/*dequeue, pop the front*/
bool DeQueue(LinkQueue *q);


void InitQueue(LinkQueue *q) {
    q->base = (QueueNode *)malloc(sizeof(QueueNode));
    q->front = q->rear = q->base;
    QueueNode *ptr = q->base;
    for(int i = 0; i < 7; i++)  //进行8个队列元素的初始化
    {
        ptr->next = (QueueNode *)malloc(sizeof(QueueNode));
        ptr = ptr->next;
    }
    ptr->next = NULL;
    size = 0;
}

void DestroyQueue(LinkQueue *q) {
    for(;q->base != NULL;)
    {
        QueueNode *temp = q->base;
        q->base = q->base->next;
        free(temp);
    }
}

bool IsEmpty(LinkQueue q) {
    return size == 0;
}

bool IsFull(LinkQueue q) {
    return size == MAXQSIZE;
}

ElemType Top(LinkQueue q) {
    return q.front->data;
}

ElemType Back(LinkQueue q) {
    return q.rear->data;
}

bool EnQueue(LinkQueue *q, ElemType e) {
    if (IsFull(*q))
        return ERROR;
    q->rear->data = e;
    if(q->rear->next == NULL)
    {
        q->rear = q->base;
    }
    else
    {
        q->rear = q->rear->next;
    }
    size++;
    return OK;
}
/*dequeue, pop the front*/
bool DeQueue(LinkQueue *q) {
    if(IsEmpty(*q))
        return ERROR;
    if(q->front->next == NULL)
    {
        q->front = q->base;
    }
    else
    {
        q->front = q->front->next;
    }
    size--;
    return OK;
}
/*
void input(LinkQueue q,int n,int i) {
    ElemType e;
    if(n > 8) n = 8;   //多于8个元素全部舍去
    for(int j = 0; j < i; j++)  //从i处开始
    {
        q.front = q.front->next;
    }
    q.rear = q.front;
    for(int j = 0; j < n; j++)
    {
        if(IsFull(q)) exit(ERROR);
        scanf("%d",&e);
        EnQueue(&q,e);
    }
}

void output(LinkQueue q,int k) {
    if(k>MAXQSIZE) printf("Error");
    else if(k>size) printf("Fault");
    else if(IsFull(q)) {
        printf("Full ");
        for(int j = 0; j < k; j++)
        {
            if(IsEmpty(q)) exit(ERROR);
            printf("%d ",Top(q));
            DeQueue(&q);
        }
    }
    else{
        for(int j = 0; j < k; j++)
        {
            if(IsEmpty(q)) exit(ERROR);
            printf("%d ",Top(q));
            DeQueue(&q);
        }
    }

}
*/
int main()
{
    LinkQueue q;
    int n,i,k;  //n是要存入的数据数，i是存放的起始处，k是输出的数据数
    scanf("%d %d %d",&n,&i,&k);
    InitQueue(&q);  //初始化8个元素的队列
    ElemType e;
    if(n > 8) n = 8;   //多于8个元素全部舍去
    for(int j = 0; j < i; j++)  //从i处开始
    {
        q.front = q.front->next;
    }
    q.rear = q.front;
    for(int j = 0; j < n; j++)
    {
        if(IsFull(q)) exit(ERROR);
        scanf("%d",&e);
        EnQueue(&q,e);
    }
    if(k>MAXQSIZE) printf("Error");
    else if(k>size) printf("Fault");
    else if(IsFull(q)) {
        printf("Full ");
        for(int j = 0; j < k; j++)
        {
            if(IsEmpty(q)) exit(ERROR);
            printf("%d ",Top(q));
            DeQueue(&q);
        }
    }
    else{
        for(int j = 0; j < k; j++)
        {
            if(IsEmpty(q)) exit(ERROR);
            printf("%d ",Top(q));
            DeQueue(&q);
        }
    }
    DestroyQueue(&q);
    return 0;
}