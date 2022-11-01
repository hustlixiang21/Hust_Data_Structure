//Your code begins.
//示例仅供参考，你也可以自行修改设计。
#include <stdio.h>
#include <stdlib.h>
#define MaxDepth = 4
typedef struct node
{
	int coef;	    //系数
	int exp;		//指数
	int layer;		//该栈当前所处的地址（层数），从0开始到4。用于判断栈满情况
	struct node* next;
}node, * pstack; 		//node用于声明结点，pstack用于声明栈指针

/*创建新结点并执行初始化*/
pstack create_node(pstack new_node, int coef, int exp);

/*将新节点压入栈中，由top指示入栈的位置，并返回新的栈顶*/
pstack push(pstack top, pstack new_node);

/*实现合并两个有序链表*/
pstack merge_two_polynomials(pstack polynomial1, pstack polynomial2);

/*递归实现链表的逆序输出*/
void output(pstack head);

/*释放head为头的链表空间*/
void StackDestroy(pstack head);

pstack create_node(pstack new_node, int coef, int exp){
    new_node = (pstack) malloc(sizeof(node));
    new_node->coef = coef;
    new_node->exp = exp;
    return new_node;
}

pstack push(pstack top, pstack new_node)
{
    //将新节点压入栈中，由top指示入栈的位置
    if(top->layer == 4)  //层数满的时候直接输出并退出
    {
        printf("Full!");
        exit(-1);
    }
    new_node->layer = top->layer + 1;
    top->next = new_node;
    return new_node;  //返回新的栈顶指针
}

pstack merge_two_polynomials(pstack polynomial1, pstack polynomial2)
{
    //合并两个多项式，参数是两个多项式的头指针，返回值是合并后的头节点
    pstack head = NULL;
    head = create_node(head,0,0);
    pstack tail = head;
    while(polynomial1 != NULL && polynomial2 != NULL){
        if(polynomial1->exp < polynomial2->exp)
        {
            tail->next = polynomial1;
            polynomial1 = polynomial1->next;
        }
        else
        {
            tail->next = polynomial2;
            polynomial2 = polynomial2->next;
        }
        tail = tail->next;
    }
    if(polynomial1 == NULL) tail->next = polynomial2;
    if(polynomial2 == NULL) tail->next = polynomial1;

    return head->next;
}

void output(pstack head){
    if(head == NULL) return;
    output(head->next);
    printf("%d %d ",head->coef,head->exp);
}

void StackDestroy(pstack head){
    pstack temp;
    while(head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main()
{
    int i1=0,j1=0;
    int num;
    int nums1[10] = {0,};
    int nums2[10] = {0,};
    while(scanf("%d",&num))
    {
        nums1[i1++] = num;
    }
    char c = getchar();
    while(scanf("%d",&num)!=EOF)
    {
        nums2[j1++] = num;
    }
    //上方代码用于数据的输入

    int coef,exp;

    node polynomial1; //为多项式1建立虚拟头
    polynomial1.layer = -1;
    pstack new1 = &polynomial1; //用于创建新结点
    pstack top1 = &polynomial1; //用于指向堆栈顶

    int i2 = 0;
    while(i2 < i1)
    {
        coef = nums1[i2++];
        exp = nums1[i2++];
        new1->next = create_node(new1->next,coef,exp);
        new1 = new1->next;
        top1 = push(top1, new1);
    }
    new1->next = NULL; //多项式1创建完毕之后令最后一个节点的next为空
    node polynomial2; //为多项式2建立虚拟头
    polynomial2.layer = -1;
    pstack new2 = &polynomial2; //用于创建新结点
    pstack top2 = &polynomial2; //用于指向堆栈顶

    int j2 = 0;
    while(j2 < j1)
    {
        coef = nums2[j2++];
        exp = nums2[j2++];
        new2->next = create_node(new2->next,coef,exp);
        new2 = new2->next;
        top2 = push(top2, new2);
    }
    new2->next = NULL; //同上
    pstack newhead = merge_two_polynomials(polynomial1.next,polynomial2.next);
    output(newhead);
    StackDestroy(polynomial1.next);
    StackDestroy(polynomial2.next);
    return 0;
}
//Your code ends.