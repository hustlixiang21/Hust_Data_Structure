#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAXLENGTH 20 //定义站点名字符串的最大长度
#define MAXSIZE 25   //定义最大的站点个数
#define MAXLINES 10  //定义最多的地铁线路数
#define MAX_ADJ 4    //定义一个站的最大邻接数

static int count = 0;                             //记录换乘站的个数
char start_stations[MAXLINES][MAXLENGTH] = {0};   //记录起始站的名字
char transfer_stations[MAXSIZE][MAXLENGTH] = {0}; //记录换乘站的名字

typedef struct DoubleNode /*双向链表用于存储线路*/
{
    char name[MAXLENGTH];    //站点名称
    float length;            //与下一个站点的距离
    struct DoubleNode *next; //后继
    struct DoubleNode *prev; //前驱
} DoubleNode;
typedef struct EdgeNode /*边表结点*/
{
    int line_number;       //地铁线路号
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
void EdgeNode_insert(MetroGraph *G, DoubleNode *head, int line);

/*添加有效的地铁站点*/
void Add_station(MetroGraph *G, char pre_station[], char cur_station[], float pre_distance, float after_distance, int line_number);

/*删除有效的地铁站点*/
void Del_station(MetroGraph *G, char old_station[]);

/*添加由于删除或增加新产生的边表结点*/
void Add_EdgeNode(MetroGraph *G, int index, char station_name[], int Line_number, float Distance);

/*删除边表结点,并且直接存储结点的离后一个站点的距离，因为删除之后要合并距离*/
void Del_EdgeNode(MetroGraph *G, int index, char station_name[], float *after_distance);

/*判断station是否已经存入了顶点表结点*/
bool existed(MetroGraph *G, char name[]);

/*查找图G中与name相同的顶点表中的元素，并返回stations数组的下标*/
int locate(MetroGraph *G, char name[]);

/*查找顶点的邻接点中线路号相同的元素,配对作用*/
EdgeNode *Find_same_line(EdgeNode *head, int Line_number);

/*查找顶点的邻接点中线路号相同的元素,不配对，找出所有相同的*/
EdgeNode *Search_same_line(EdgeNode *head, int Line_number);

//上面两个函数理论上应该是可以整合到一起的，但是我想了很久还是不太会
//主要是因为程序太长了，要改逻辑的话过于复杂

/*根据邻接表打印地铁线路*/
void print_line(MetroGraph *G, int Line_number);

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
        strcpy(start_stations[i], lines[i]->next->name);
        Station_insert(G, lines[i]);
    }
    //邻接表的顶点表的创建(成功)

    //邻接表边表结点的创建
    for (int i = 0; i < MAXLINES; i++)
    {
        if (lines[i] == NULL)
            continue;
        EdgeNode_insert(G, lines[i], i);
    }
    //邻接表边表结点的创建(成功)

    //添加和删除操作的解析
    char op[7] = {0};
    int line_num = 0;
    float pre_distance = 0;
    float aft_distance = 0;
    char pre_station[MAXLENGTH] = {0};
    char cur_station[MAXLENGTH] = {0};
    char aft_station[MAXLENGTH] = {0};
    scanf("%s", op);
    if (strcmp(op, "add") == 0) // add操作
    {
        scanf("%d", &line_num);
        scanf("%g", &pre_distance);
        scanf("%g", &aft_distance);
        if ((int)pre_distance == 0)
        {
            scanf("%s", cur_station);
            if (locate(G, cur_station) > 0)
            {
                printf("增加失败，已有同名站点");
            }
            else
            {
                strcpy(aft_station, start_stations[line_num]);
                strcpy(start_stations[line_num], cur_station);
                Add_EdgeNode(G, locate(G, aft_station), cur_station, line_num, aft_distance);
                strcpy(G->Stations[G->numNodes++].Station_Name, cur_station);
                Add_EdgeNode(G, locate(G, cur_station), aft_station, line_num, aft_distance);
                print_line(G, line_num);
            }
        }
        else
        {
            scanf("%s", pre_station);
            scanf("%s", cur_station);
            if (locate(G, pre_station) == -1)
            {
                printf("增加失败，没有与输入的增加位置前一站点同名的站点");
            }
            else if (locate(G, cur_station) > 0)
            {
                printf("增加失败，已有同名站点");
            }
            else
            {
                Add_station(G, pre_station, cur_station, pre_distance, aft_distance, line_num);
                print_line(G, line_num);
            }
        }
    }
    else
    {
        scanf("%d", &line_num);
        scanf("%s", cur_station);
        if (locate(G, cur_station) == -1)
        {
            printf("删除失败，没有同名站点");
        }
        else
        {
            Del_station(G, cur_station);
            print_line(G, line_num);
        }
    }

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
        else
        {
            strcpy(transfer_stations[count++], head->name);
        }
        head = head->next;
    }
}

void EdgeNode_insert(MetroGraph *G, DoubleNode *head, int line)
{
    head = head->next;
    int index;   //用于定位Stations中的站点
    EdgeNode *e; //主要用于malloc作为一个临时的指针
    while (head->next != NULL)
    {
        index = locate(G, head->name);
        e = (EdgeNode *)malloc(sizeof(EdgeNode));
        strcpy(e->name, head->next->name);
        e->distance = head->length;
        e->line_number = line;
        e->next = G->Stations[index].firstedge;
        G->Stations[index].firstedge = e;

        index = locate(G, head->next->name);
        e = (EdgeNode *)malloc(sizeof(EdgeNode));
        strcpy(e->name, head->name);
        e->distance = head->length;
        e->line_number = line;
        e->next = G->Stations[index].firstedge;
        G->Stations[index].firstedge = e;
        G->numEdges += 2;
        head = head->next;
    }
}

void Add_station(MetroGraph *G, char pre_station[], char cur_station[], float pre_distance, float after_distance, int line_number)
{
    //首先通过对于线路起点至终点的遍历找到cur与after
    char Start[MAXLENGTH];         //存放起始站名
    char Prev_Station[MAXLENGTH];  //用于普通站点（即非换乘站和非起始站）的已访问记录
    char After_Station[MAXLENGTH]; //用于存放下一个站点
    EdgeNode *ptr = NULL;
    strcpy(Start, start_stations[line_number]);
    int index = locate(G, Start);
    strcpy(Prev_Station, Start);
    strcpy(After_Station, G->Stations[index].firstedge->name);
    while (true)
    {
        if (strcmp(pre_station, Prev_Station) == 0)
            break;
        index = locate(G, After_Station);
        if (G->Stations[index].firstedge->next == NULL)
            break;
        ptr = Search_same_line(G->Stations[index].firstedge, line_number);
        if (strcmp(Prev_Station, ptr->name) == 0) //如果访问过直接从ptr开始寻找
        {
            ptr = ptr->next;
            ptr = Search_same_line(ptr, line_number);
        }
        strcpy(Prev_Station, After_Station);
        strcpy(After_Station, ptr->name);
    }
    //以上代码只为找到pre结点在原线路的下一结点
    strcpy(G->Stations[G->numNodes++].Station_Name, cur_station);
    index = locate(G, Prev_Station);
    ptr = G->Stations[index].firstedge;
    while (ptr != NULL)
    {
        if (strcmp(ptr->name, After_Station) == 0)
        {
            strcpy(ptr->name, cur_station);
            ptr->distance = pre_distance;
            break;
        }
        ptr = ptr->next;
    }
    index = locate(G, After_Station);
    ptr = G->Stations[index].firstedge;
    while (ptr != NULL)
    {
        if (strcmp(ptr->name, Prev_Station) == 0)
        {
            strcpy(ptr->name, cur_station);
            ptr->distance = after_distance;
            break;
        }
        ptr = ptr->next;
    }
    Add_EdgeNode(G, locate(G, cur_station), Prev_Station, line_number, pre_distance);
    Add_EdgeNode(G, locate(G, cur_station), After_Station, line_number, after_distance);
    G->numEdges += 2;
}

void Del_station(MetroGraph *G, char old_station[])
{
    int flag = 0;                      //普通站点flag=0；起始站点flag=1；换乘站点和普通站点的处理方法一致；
    int index;                         //用于索引
    int visited[MAXLINES] = {0};       //用于标记已经访问过的线路
    EdgeNode *ptr = NULL;              //用于遍历邻接点
    for (int i = 0; i < MAXLINES; i++) //寻找起始站点
    {
        if (start_stations[i][0] == 0)
            continue;
        if (strcmp(old_station, start_stations[i]) == 0)
        {
            flag = 1;
            break;
        }
    }
    //以上部分用于设置flag以体现不同类型的站点
    //删除之后会产生新的邻接结点，需要连接起来
    index = locate(G, old_station);
    if (flag == 0)
    {
        char adj_stations[MAX_ADJ][MAXLENGTH] = {0};
        ptr = G->Stations[index].firstedge;
        strcpy(G->Stations[index].Station_Name, "\0"); //在顶点表结点中删除该结点
        for (int i = 0; i < MAX_ADJ && ptr; i++)
        {
            float distance2;
            strcpy(adj_stations[i], ptr->name);
            index = locate(G, adj_stations[i]);
            Del_EdgeNode(G, index, old_station, &distance2);
            G->numEdges -= 2;
            if (!visited[ptr->line_number]) //对于未访问过的进行新边的添加
            {
                EdgeNode *new_adj_station = Find_same_line(ptr, ptr->line_number);
                float Sum_of_Distance = ptr->distance;
                Sum_of_Distance += distance2;
                Add_EdgeNode(G, index, new_adj_station->name, new_adj_station->line_number, Sum_of_Distance);
                Add_EdgeNode(G, locate(G, new_adj_station->name), ptr->name, ptr->line_number, Sum_of_Distance);
                G->numEdges += 2;
                visited[ptr->line_number] = 1;
            }
            ptr = ptr->next;
        }
    }
    else //删除起始结点，并且不需要进行连接（一般来说是这样的）
    {
        for (int i = 0; i < count; i++)
        {
            if (strcmp(old_station, start_stations[i]) == 0)
            {
                strcpy(start_stations[i], G->Stations[index].firstedge->name);
                break;
            }
        }
        index = locate(G, G->Stations[index].firstedge->name);
        Del_EdgeNode(G, index, old_station, NULL);
        G->numEdges -= 2;
    }
    G->numNodes--;
}

void Add_EdgeNode(MetroGraph *G, int index, char station_name[], int Line_number, float Distance)
{
    EdgeNode *e = (EdgeNode *)malloc(sizeof(EdgeNode));
    e->line_number = Line_number;
    e->distance = Distance;
    strcpy(e->name, station_name);
    e->next = G->Stations[index].firstedge;
    G->Stations[index].firstedge = e;
}

void Del_EdgeNode(MetroGraph *G, int index, char station_name[], float *after_distance)
{
    EdgeNode *cur = G->Stations[index].firstedge;
    EdgeNode *pre = NULL;
    while (cur != NULL)
    {
        if (strcmp(cur->name, station_name) == 0)
        {
            if (pre == NULL)
                G->Stations[index].firstedge = cur->next;
            else
                pre->next = cur->next;
            if (after_distance)
                *after_distance = cur->distance;
            free(cur);
            break;
        }
        pre = cur;
        cur = cur->next;
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

EdgeNode *Find_same_line(EdgeNode *head, int Line_number)
{
    head = head->next;
    while (head)
    {
        if (head->line_number == Line_number)
            return head;
        head = head->next;
    }
    return NULL;
}

EdgeNode *Search_same_line(EdgeNode *head, int Line_number)
{
    while (head)
    {
        if (head->line_number == Line_number)
            return head;
        head = head->next;
    }
    return NULL;
}

void print_line(MetroGraph *G, int Line_number)
{
    printf("%d ", Line_number);
    //在起始站数组中寻找该起始点
    char Start[MAXLENGTH];         //存放起始站名
    char Prev_Station[MAXLENGTH];  //用于普通站点（即非换乘站和非起始站）的已访问记录
    char After_Station[MAXLENGTH]; //用于存放下一个站点
    EdgeNode *ptr = NULL;
    strcpy(Start, start_stations[Line_number]);
    int index = locate(G, Start);
    float After_length = G->Stations[index].firstedge->distance;
    printf("%s %.2f ", Start, After_length);
    strcpy(Prev_Station, Start);
    strcpy(After_Station, G->Stations[index].firstedge->name);
    while (true)
    {
        index = locate(G, After_Station);
        if (G->Stations[index].firstedge->next == NULL)
            break;
        ptr = Search_same_line(G->Stations[index].firstedge, Line_number);
        if (strcmp(Prev_Station, ptr->name) == 0) //如果访问过直接从ptr开始寻找
        {
            ptr = ptr->next;
            ptr = Search_same_line(ptr, Line_number);
        }
        strcpy(Prev_Station, After_Station);
        if(ptr == NULL)
            break;
        After_length = ptr->distance;
        printf("%s %.2f ", After_Station, After_length);
        strcpy(After_Station, ptr->name);
    }
    printf("%s", After_Station);
}
