#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAXLENGTH 20 //定义站点名字符串的最大长度
#define MAXSIZE 25   //定义最大的站点个数
#define MAXLINES 10  //定义最多的地铁线路数

typedef struct DoubleNode /*双向链表用于存储线路*/
{
    char name[MAXLENGTH];    //站点名称
    float length;            //与下一个站点的距离
    struct DoubleNode *next; //后继
    struct DoubleNode *prev; //前驱
} DoubleNode;
typedef struct EdgeNode /*边表结点*/
{
    char name[MAXLENGTH];  //用于存放站点的名称
    float distance;        //用于存储站点之间的距离
    struct EdgeNode *next; //链域，指向下一个邻接点
} EdgeNode;

typedef struct Station /*顶点表结点*/
{
    char Station_Name[MAXLENGTH]; //顶点域用于存储顶点的信息
    EdgeNode *firstedge;          //边表头指针
} Station, Stations_List[MAXSIZE];

typedef struct /*地铁线路图的定义*/
{
    Stations_List Stations; //图的点集，即站点信息
    int numNodes, numEdges; //结点的个数和边的个数
} MetroGraph;

/**********函数声明部分**********/
//双向链表相关函数

/*初始化一个双向链表*/
DoubleNode *Doubly_list_init();

/*在双向链表的尾部添加新结点*/
void Add_rear(DoubleNode *head, char *name, float length);

/*在双向链表的首部添加新结点*/
void Add_front(DoubleNode *head, char *name, float length);

/*从头至尾打印双向链表*/
void print_Doubly_list(DoubleNode *head);

/*双向链表的摧毁*/
void Destroy_Doubly_list(DoubleNode *head);

//邻接表创建相关函数

/*初始化一个地铁有向图*/
MetroGraph *graph_init();

/*销毁已经创建的图结构*/
void graph_destroy(MetroGraph *G);

/*依据双向链表中的信息创建顶点表结点*/
void Station_insert(MetroGraph *G, DoubleNode *head);

/*依据双向链表中的信息创建边表结点*/
void EdgeNode_insert(MetroGraph *G, DoubleNode *head);

/*判断station是否已经存入了顶点表结点*/
bool existed(MetroGraph *G, char name[]);

/*查找图G中与name相同的顶点表中的元素，并返回stations数组的下标*/
int locate(MetroGraph *G, char name[]);

/**********主函数部分**********/
int main()
{
    //双向链表的创建与输入的读取
    int n;
    DoubleNode *lines[MAXLINES] = {NULL};
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        DoubleNode *temp = Doubly_list_init(); //新建双向链表的虚拟头节点
        int Line;
        char name[MAXLENGTH];
        float length = 1;
        scanf("%d", &Line);
        lines[Line] = temp;
        while ((int)length != 0)
        {
            scanf("%s %g", name, &length);
            Add_rear(lines[Line], name, length);
        }
    }
    //双向链表的创建与输入的读取(成功)

    //打印地铁线路(即一个双向链表)
    for (int i = 0; i < MAXLINES; i++)
    {
        if (lines[i] == NULL)
            continue;
        printf("%d ", i);
        print_Doubly_list(lines[i]);
    }

    //邻接表的创建
    MetroGraph *G = graph_init();

    //邻接表的顶点表的创建
    for (int i = 0; i < MAXLINES; i++)
    {
        if (lines[i] == NULL)
            continue;
        Station_insert(G, lines[i]);
    }
    //邻接表的顶点表的创建(成功)

    //邻接表边表结点的创建
    for (int i = 0; i < MAXLINES; i++)
    {
        if (lines[i] == NULL)
            continue;
        EdgeNode_insert(G, lines[i]);
    }
    //邻接表边表结点的创建(成功)

    //双向链表的销毁
    for (int i = 0; i < MAXLINES; i++)
    {
        if (lines[i] == NULL)
            continue;
        Destroy_Doubly_list(lines[i]);
    }

    //图的销毁
    graph_destroy(G);
    return 0;
}

/**********函数实现部分**********/
//双向链表相关
DoubleNode *Doubly_list_init()
{
    DoubleNode *p = (DoubleNode *)malloc(sizeof(DoubleNode));
    p->length = 0;
    p->next = NULL;
    p->prev = NULL;
    return p;
}

void Add_rear(DoubleNode *head, char name[], float length)
{
    while (head->next != NULL) //定位到双向链表的末尾
    {
        head = head->next;
    }
    head->next = (DoubleNode *)malloc(sizeof(DoubleNode));
    strcpy(head->next->name, name);
    head->next->length = length;
    head->next->next = NULL;
    head->next->prev = head;
}

void Add_front(DoubleNode *head, char *name, float length)
{
    DoubleNode *temp = (DoubleNode *)malloc(sizeof(DoubleNode));
    strcpy(temp->name, name);
    temp->length = length;
    head->next = temp->next;
    temp->next->prev = temp;
    temp->prev = head;
}

void print_Doubly_list(DoubleNode *head)
{
    head = head->next;
    while (head != NULL)
    {
        if ((int)head->length == 0)
            printf("%s", head->name);
        else
            printf("%s %.2f ", head->name, head->length);
        head = head->next;
    }
    printf("\n");
}

void Destroy_Doubly_list(DoubleNode *head)
{
    while (head != NULL)
    {
        DoubleNode *temp = head;
        head = head->next;
        free(temp);
    }
}

//有向图邻接表相关
MetroGraph *graph_init()
{
    MetroGraph *G = (MetroGraph *)malloc(sizeof(MetroGraph));
    G->numEdges = 0;
    G->numNodes = 0;
    for (int i = 0; i < MAXSIZE; i++)
    {
        G->Stations[i].firstedge = NULL;
    }
    return G;
}

void graph_destroy(MetroGraph *G)
{
    for (int i = 0; i < G->numNodes; i++)
    {
        EdgeNode *temp;
        while (G->Stations[i].firstedge != NULL)
        {
            temp = G->Stations[i].firstedge;
            G->Stations[i].firstedge = G->Stations[i].firstedge->next;
            free(temp);
        }
    }
    free(G);
}

void Station_insert(MetroGraph *G, DoubleNode *head)
{
    head = head->next; //跳过双向链表的虚拟头结点
    while (head != NULL)
    {
        if (!existed(G, head->name)) //如果站名在顶点表里不存在就加入
        {
            strcpy(G->Stations[G->numNodes++].Station_Name, head->name);
        }
        head = head->next;
    }
}

void EdgeNode_insert(MetroGraph *G, DoubleNode *head)
{
    head = head->next; //跳过虚拟头节点
    int index;   //用于定位Stations中的站点
    EdgeNode *e; //主要用于malloc作为一个临时的指针
    while (head->next != NULL)
    {
        index = locate(G, head->name);
        e = (EdgeNode *)malloc(sizeof(EdgeNode));
        strcpy(e->name, head->next->name);
        e->distance = head->length;
        e->next = G->Stations[index].firstedge;
        G->Stations[index].firstedge = e;

        index = locate(G, head->next->name);
        e = (EdgeNode *)malloc(sizeof(EdgeNode));
        strcpy(e->name, head->name);
        e->distance = head->length;
        e->next = G->Stations[index].firstedge;
        G->Stations[index].firstedge = e;
        G->numEdges += 2;
        head = head->next;
    }
}

bool existed(MetroGraph *G, char name[])
{
    for (int i = 0; i < MAXSIZE; i++)
    {
        if (strcmp(G->Stations[i].Station_Name, name) == 0)
            return true; //如果存在相同的站名就返回true
    }
    return false;
}

int locate(MetroGraph *G, char name[])
{
    for (int i = 0; i < G->numNodes; i++)
    {
        if (strcmp(G->Stations[i].Station_Name, name) == 0)
            return i;
    }
    return -1;
}
