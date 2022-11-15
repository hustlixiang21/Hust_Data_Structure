#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef char ElemType;

/********二叉链表（孩子-兄弟）存储表示********/
typedef struct TreeNode
{
    struct TreeNode *pFirstChild;  //指向该结点的长子
    struct TreeNode *pNextSibling; //指向该结点的右兄弟
    ElemType data;
} TreeNode, *pTreeNode, **ppTreeNode;

/********辅助队列********/
typedef struct QueueNode
{
    struct QueueNode *pNext; //指向下一个结点
    pTreeNode data;          //链式队列的结点的类型是一个树结点类型
} QueueNode, *pQueueNode;

typedef struct Queue
{
    pQueueNode pHead; //指向队头
    pQueueNode pTail; //指向队尾
    int nodeCount;    //存储队列的元素个数
} Queue, *pQueue;

/********队列相关操作的定义********/

void InitQueue(pQueue q);                       //初始化队列
void Enqueue(pQueue q, pTreeNode ptree_node);   //入队
void Dequeue(pQueue q, ppTreeNode pptree_node); //出队
bool IsQueueEmpty(Queue q);                     //判断队列是否为空
void DestroyQueue(pQueue q);                    //销毁队列

/********树相关操作的定义********/
void CreateTree(pTreeNode ptree_node);       //创建一个树
void DestroyTree(pTreeNode ptree_node);      //销毁该树
int GetDepth(pTreeNode ptree_node);          //获取树的深度
int GetLeaf(pTreeNode ptree_node);           //获取该树的叶子结点的个数
void PrintTree(pTreeNode ptree_node, int i); //凹入表（缩进目录结构）方式打印一棵树

//暂且搁置
void PreTravel(pTreeNode ptree_node);
void PostTravel(pTreeNode ptree_node);
void MidTravel(pTreeNode ptree_node);
void LevelTravel(pTreeNode ptree_node);

pTreeNode Point(pTreeNode ptree_node, ElemType e);
ElemType GetParent(pTreeNode ptree_node, ElemType e);

Queue queue; //定义全局变量队列

int main()
{
    TreeNode root;
    CreateTree(&root);
    printf("该树的深度为%d\n", GetDepth(&root));
    printf("该树的叶子结点个数为%d\n", GetLeaf(&root));
    PrintTree(&root,1);
    return 0;
}

void InitQueue(pQueue q)
{
    q->pHead = q->pTail = (pQueueNode)malloc(sizeof(pQueueNode));
    q->nodeCount = 0;
    q->pHead->pNext = NULL;
}

void Enqueue(pQueue q, pTreeNode ptree_node)
{
    pQueueNode pNew = (pQueueNode)malloc(sizeof(QueueNode));
    pNew->pNext = NULL;
    pNew->data = ptree_node;
    q->pTail->pNext = pNew;
    q->pTail = pNew;
    q->nodeCount++;
}

void Dequeue(pQueue q, ppTreeNode pptree_node)
{
    pQueueNode pDel = q->pHead->pNext;
    q->pHead->pNext = pDel->pNext;
    if (pDel == q->pTail)
    {
        q->pTail = q->pHead;
    }
    *pptree_node = pDel->data;
    free(pDel);
    q->nodeCount--;
}

bool IsQueueEmpty(Queue q)
{
    return q.nodeCount == 0;
}

void DestroyQueue(pQueue q)
{
    if (q != NULL)
    {
        pQueueNode pTravel = q->pHead->pNext;
        while (pTravel != NULL)
        {
            q->pHead->pNext = pTravel->pNext;
            free(pTravel);
            pTravel = q->pHead->pNext;
        }
        free(q->pHead);
        q->nodeCount = 0;
    }
}

void CreateTree(pTreeNode ptree_node)
{
    char buffer[20];
    char ch;
    printf("输入根结点:");
    scanf("%c", &ch);
    InitQueue(&queue);
    if (ch != '#') //#代表根结点为空
    {
        ptree_node->data = ch;
        ptree_node->pNextSibling = NULL;
        Enqueue(&queue, ptree_node); //入队根节点
        pTreeNode pTemp, pTemp1;
        while (!IsQueueEmpty(queue))
        {
            Dequeue(&queue, &pTemp);
            printf("输入%c的孩子结点:", pTemp->data);
            scanf("%s", buffer);
            if (buffer[0] != '#')
            {
                pTemp->pFirstChild = (pTreeNode)malloc(sizeof(TreeNode));
                pTemp->pFirstChild->data = buffer[0];
                pTemp1 = pTemp->pFirstChild;
                for (int i = 1; i < strlen(buffer); i++)
                {
                    pTemp1->pNextSibling = (pTreeNode)malloc(sizeof(TreeNode));
                    Enqueue(&queue, pTemp1);
                    pTemp1->pNextSibling->data = buffer[i];
                    pTemp1 = pTemp1->pNextSibling;
                }
                pTemp1->pNextSibling = NULL;
                Enqueue(&queue, pTemp1);
            }
        }
    }
    else
    {
        ptree_node = NULL;
    }
}

void DestroyTree(pTreeNode ptree_node)
{
    if (ptree_node != NULL)
    {
        if (ptree_node->pFirstChild != NULL)
        {
            DestroyTree(ptree_node->pFirstChild);
        }
        if (ptree_node->pNextSibling != NULL)
        {
            DestroyTree(ptree_node->pNextSibling);
        }
        free(ptree_node);
        ptree_node = NULL;
    }
}

int GetDepth(pTreeNode ptree_node)
{
    if (ptree_node == NULL)
    {
        return 0;
    }
    if (ptree_node->pFirstChild == NULL)
    {
        return 1;
    }
    int depth = 0, max = 0;
    pTreeNode temp = ptree_node->pFirstChild;
    while (temp != NULL)
    {
        depth = GetDepth(temp);
        if (depth > max)
        {
            max = depth;
        }
        temp = temp->pNextSibling;
    }
    return max + 1;
}

int GetLeaf(pTreeNode ptree_node)
{
    if (ptree_node != NULL)
    {
        if (ptree_node->pFirstChild == NULL)
            return 1 + GetLeaf(ptree_node->pNextSibling);
        else
            return GetLeaf(ptree_node->pFirstChild) + GetLeaf(ptree_node->pNextSibling);
    }
    else
        return 0;
}

void PrintTree(pTreeNode ptree_node, int i)
{
    pTreeNode p;
    for (int cnt = 1; cnt < i; cnt++)
        printf(" ");
    printf("%c\n", ptree_node->data);
    for (p = ptree_node->pFirstChild; p != NULL; p = p->pNextSibling)
        PrintTree(p, i + 1);
}


