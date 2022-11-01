#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAXLENGTH 100
#define MAXSIZE 10

/********结构定义部分********/
/*结点结构体*/
typedef struct PNode
{
    double coef;        //系数
    int exp;            //指数
    struct PNode *next; //指向下一结点
} PNode, *Polynomial;

/********函数声明部分********/
/*从source中提取数据到destination*/
void extract_data(char *dest, char *src);

/*创建新的结点，并为其中的coef和exp进行初始化*/
void creat_new_node(Polynomial pre_node, double coef, int exp);

/*用于递归打印链表多项式的结果，传入起始地址即可*/
void print(Polynomial head);

/*两个多项式相加，参数为两个多项式链表*/
void Add_two_Poly(Polynomial Pa, Polynomial Pb);

/*两个多项式相减，参数为两个多项式链表*/
void Sub_two_Poly(Polynomial Pa, Polynomial Pb);

// note：上面两个函数兼具输出和释放无用结点的功能

/*交换x,y，用于快排中整型数据的数据交换*/
void swap_int(int *x, int *y);

/*交换x,y，用于快排中浮点型数据的数据交换*/
void swap_float(double *x, double *y);

/*快排的分割函数，用于选取基准pivot*/
int Partition(int exp[], double coef[], int low, int high);

/*快速排序，传入数组和排序的区间，进行排序，因同时需要交换coef数组故也传入*/
void QuickSort(int exp[], double coef[], int low, int high);

/*合并一个链表中exp相同的点*/
void merge_same_exp_node(Polynomial head);

/*判断一个系数是否为零*/
bool judge_zero(double num);

/********主函数部分********/
int main()
{
    char str[MAXLENGTH];
    char *str1;
    char *str2;
    gets(str);
    str1 = strtok(str, ")");
    str2 = strtok(NULL, ")");
    char op = str2[0];

    if (*(str1 + 1) == 'x' || (*(str1 + 1) >= '0' && *(str1 + 1) <= '9'))
        *str = '+';
    else
        str1 += 1;
    if (*(str2 + 2) == 'x' || (*(str2 + 2) >= '0' && *(str2 + 2) <= '9'))
    {
        *(str2 + 1) = '+';
        str2++;
    }
    else
        str2 += 2;
    //上述一段代码的用处是使所有的项格式统一，也能起到去除废字符的作用

    char str_poly1[MAXLENGTH] = {0};
    char str_poly2[MAXLENGTH] = {0};

    extract_data(str_poly1, str1);
    extract_data(str_poly2, str2);


    //每个多项式利用两个一维维数组分别暂存系数和指数
    double coef1[MAXSIZE] = {0};
    double coef2[MAXSIZE] = {0};
    int exp1[MAXSIZE] = {0};
    int exp2[MAXSIZE] = {0};
    int i1 = 0, j1 = 0;

    char *p = strtok(str_poly1," ");
    while(p){
        if (i1 % 2 == 0)
            coef1[(i1++) / 2] = atof(p);
        else
        {
            exp1[(i1++) / 2] = atoi(p);
        }
        p = strtok(NULL," ");
    }
    i1 = i1 / 2;
    p = strtok(str_poly2," ");
    while(p){
        if (j1 % 2 == 0)
            coef2[(j1++) / 2] = atof(p);
        else
        {
            exp2[(j1++) / 2] = atoi(p);
        }
        p = strtok(NULL," ");
    }
    j1 = j1 / 2;

    //进行快速排序使之有序
    if (i1 > 1)
        QuickSort(exp1, coef1, 0, i1 - 1);
    if (j1 > 1)
        QuickSort(exp2, coef2, 0, j1 - 1);

    //根据已排序好的数组进行多项式的构建，所使用的结构是链表
    //首先建立两个虚拟头节点，以防空链表出现，虽然本题不会出现
    PNode poly1, poly2;
    poly1.next = NULL;
    poly2.next = NULL;
    //建立两个指针用于以上方两个结点为头往后创建新的结点
    Polynomial ptr1 = &poly1;
    Polynomial ptr2 = &poly2;
    for (int i = 0; i < i1; i++)
    {
        creat_new_node(ptr1, coef1[i], exp1[i]);
        ptr1 = ptr1->next;
    }
    for (int i = 0; i < j1; i++)
    {
        creat_new_node(ptr2, coef2[i], exp2[i]);
        ptr2 = ptr2->next;
    }

    //主要思路采用两个指针，进行协同后移，通过对于条件的判断实现加减
    ptr1 = &poly1;
    ptr2 = &poly2;
    merge_same_exp_node(ptr1->next);
    merge_same_exp_node(ptr2->next);
    if (op == '+')
        Add_two_Poly(ptr1, ptr2);
    else if (op == '-')
        Sub_two_Poly(ptr1, ptr2);
    return 0;
}

/********函数实现部分********/
void extract_data(char dest[], char *src)
{
    int i = 0;
    while (*src != '\0')
    {
        int flag = 0;
        if (*src == '+' || *src == '-')
        {
            if(*(src + 1) == '0' && *(src + 2) == '\0') //对于零情况的判断
            {
                dest[i++] = '0';
                dest[i++] = ' ';
                dest[i++] = '0';
                break;
            }
            if (*(src + 1) == 'x')
            {
                dest[i++] = *src;
                dest[i++] = '1';
                src++;
            }
            else
            {
                while (*src != 'x')
                {
                    if(*(src+1) == '+' || *(src+1) == '-')
                    {
                        flag = 1;
                        dest[i++] = *(src++);
                        break;
                    }
                    dest[i++] = *(src++);
                }
            }
            dest[i++] = ' ';
        }
        if(flag)
        {
            dest[i++] = '0';
            dest[i++] = ' ';
            continue;
        }
        if(*(++src)=='^')
        {
            src++;
            do{
                dest[i++] = *(src++);
            }while(*src != '+' && *src !='-' && *src != '\0');
        }
        else dest[i++] = '1';
        dest[i++] = ' ';
    }
}

void creat_new_node(Polynomial pre_node, double coef, int exp)
{
    pre_node->next = (Polynomial)malloc(sizeof(PNode));
    pre_node->next->coef = coef;
    pre_node->next->exp = exp;
}

void print(Polynomial head)
{
    if (head == NULL)
        return;
    print(head->next);
    if (judge_zero(head->coef))
        return;
    if(head->next == NULL)
    {
        if(head->exp == 1)
        {
            if(judge_zero(head->coef - 1))
                printf("x");
            else if(judge_zero(head->coef + 1))
                printf("-x");
            else
                printf("%gx",head->coef);
        }
        else if(head->exp == 0)
            printf("%g",head->coef);
        else
        {
            if(judge_zero(head->coef - 1))
                printf("x^%d",head->exp);
            else if(judge_zero(head->coef + 1))
                printf("-x^%d",head->exp);
            else
                printf("%gx^%d",head->coef,head->exp);
        }
    }
    else
    {
        if(head->coef > 0)
        {
            if(head->exp == 1)
            {
                if(judge_zero(head->coef - 1))
                    printf("+x");
                else
                    printf("+%gx",head->coef);
            }
            else if(head->exp == 0)
                printf("+%g",head->coef);
            else
            {
                if(judge_zero(head->coef - 1))
                    printf("+x^%d",head->exp);
                else
                    printf("+%gx^%d",head->coef,head->exp);
            }
        }
        else
        {
            if(head->exp == 1)
            {
                if(judge_zero(head->coef + 1))
                    printf("-x");
                else
                    printf("%gx",head->coef);
            }
            else if(head->exp == 0)
                printf("%g",head->coef);
            else
            {
                if(judge_zero(head->coef + 1))
                    printf("-x^%d",head->exp);
                else
                    printf("%gx^%d",head->coef,head->exp);
            }
        }
    }
}

void Add_two_Poly(Polynomial Pa, Polynomial Pb)
{
    PNode *p1, *p2, *p3, *r;
    p1 = Pa->next;
    p2 = Pb->next;
    p3 = Pa;
    double sum;
    while (p1 && p2)
    {
        if (p1->exp == p2->exp)
        {
            sum = p1->coef + p2->coef;
            if (!judge_zero(sum))
            {
                p1->coef = sum;
                p3->next = p1;
                p3 = p1;
                p1 = p1->next;
                r = p2;
                p2 = p2->next;
                free(r);
            }
            else
            {
                r = p1;
                p1 = p1->next;
                free(r);
                r = p2;
                p2 = p2->next;
                free(r);
            }
        }
        else if (p1->exp < p2->exp)
        {
            p3->next = p1;
            p3 = p1;
            p1 = p1->next;
        }
        else
        {
            p3->next = p2;
            p3 = p2;
            p2 = p2->next;
        }
    }
    p3->next = p1 ? p1 : p2;
    if (Pa->next)
        print(Pa->next);
    else
        printf("0");
}

void Sub_two_Poly(Polynomial Pa, Polynomial Pb)
{
    PNode *p1, *p2, *p3, *r;
    p1 = Pa->next;
    p2 = Pb->next;
    p3 = Pa;
    double sum;
    while (p1 && p2)
    {
        if (p1->exp == p2->exp)
        {
            sum = p1->coef - p2->coef;
            if (!judge_zero(sum))
            {
                p1->coef = sum;
                p3->next = p1;
                p3 = p1;
                p1 = p1->next;
                r = p2;
                p2 = p2->next;
                free(r);
            }
            else
            {
                r = p1;
                p1 = p1->next;
                free(r);
                r = p2;
                p2 = p2->next;
                free(r);
            }
        }
        else if (p1->exp < p2->exp)
        {
            p3->next = p1;
            p3 = p1;
            p1 = p1->next;
        }
        else
        {
            p2->coef = -1 * p2->coef;
            p3->next = p2;
            p3 = p2;
            p2 = p2->next;
        }
    }
    if (p1 == NULL)
    {
        p3->next = p2;
        p3 = p3->next;
        while (p3 != NULL)
        {
            p3->coef = -1 * p3->coef;
            p3 = p3->next;
        }
    }
    else
    {
        p3->next = p1;
        p3 = p3->next;
        while (p3 != NULL)
        {
            p3->coef = -1 * p3->coef;
            p3 = p3->next;
        }
    }
    if (Pa->next)
        print(Pa->next);
    else
        printf("0");
}

void swap_int(int *x, int *y)
{
    int t = *x;
    *x = *y;
    *y = t;
}

void swap_float(double *x, double *y)
{
    double t = *x;
    *x = *y;
    *y = t;
}

int Partition(int exp[], double coef[], int low, int high)
{
    int pivot = exp[low];
    while (low < high)
    {
        while (low < high && exp[high] >= pivot)
        {
            --high;
        }
        swap_int(&exp[low], &exp[high]);
        swap_float(&coef[low], &coef[high]); //同时交换coef即系数
        while (low < high && exp[low] <= pivot)
        {
            ++low;
        }
        swap_int(&exp[low], &exp[high]);
        swap_float(&coef[low], &coef[high]);
    }
    return low;
}

void QuickSort(int exp[], double coef[], int low, int high)
{ //快排母函数
    if (low < high)
    {
        int pivot = Partition(exp, coef, low, high);
        QuickSort(exp, coef, low, pivot - 1);
        QuickSort(exp, coef, pivot + 1, high);
    }
}

void merge_same_exp_node(Polynomial head)
{
    Polynomial cur = head;
    Polynomial after = head->next;
    while(after != NULL)
    {
        while(cur->exp == after->exp)
        {
            cur->coef = cur->coef + after->coef;
            cur->next = after->next;
            free(after);
            after = cur->next;
            if(after == NULL) return;
        }
        cur = cur->next;
        after = after->next;
    }
}

bool judge_zero(double num)
{
    if (num < 1e-4 && num > -1e-4)
        return true;
    return false;
}
