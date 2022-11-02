#include "string.h"
#include "stdio.h"    
#include "stdlib.h"   

#include "math.h"  
#include "time.h"

#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0

#define MAXSIZE 100 

typedef int Status;		

int treeIndex=1;
typedef char String[24]; 
String str;

Status StrAssign(String T,char *chars)
{ 
	int i;
	if(strlen(chars)>MAXSIZE)
		return ERROR;
	else
	{
		T[0]=strlen(chars);
		for(i=1;i<=T[0];i++)
			T[i]=*(chars+i-1);
		return OK;
	}
}

typedef char TElemType;
TElemType Nil=' '; 

Status visit(TElemType e)
{
	printf("%c ",e);
	return OK;
}

typedef struct BiTNode 
{
   TElemType data;		
   struct BiTNode *lchild,*rchild; 
}BiTNode,*BiTree;

Status InitBiTree(BiTree *T)
{ 
	*T=NULL;
	return OK;
}

void DestroyBiTree(BiTree *T)
{ 
	if(*T) 
	{
		if((*T)->lchild) 
			DestroyBiTree(&(*T)->lchild); 
		if((*T)->rchild) 
			DestroyBiTree(&(*T)->rchild); 
		free(*T);
		*T=NULL; 
	}
}


void CreateBiTree(BiTree *T)
{ 
	TElemType ch;
	
	/* scanf("%c",&ch); */
	ch=str[treeIndex++];

	if(ch=='#') 
		*T=NULL;
	else
	{
		*T=(BiTree)malloc(sizeof(BiTNode));
		if(!*T)
			exit(OVERFLOW);
		(*T)->data=ch; 
		CreateBiTree(&(*T)->lchild);
		CreateBiTree(&(*T)->rchild); 
	}
}


Status BiTreeEmpty(BiTree T)
{ 
	if(T)
		return FALSE;
	else
		return TRUE;
}

#define ClearBiTree DestroyBiTree


int BiTreeDepth(BiTree T)
{
	int i,j;
	if(!T)
		return 0;
	if(T->lchild)
		i=BiTreeDepth(T->lchild);
	else
		i=0;
	if(T->rchild)
		j=BiTreeDepth(T->rchild);
	else
		j=0;
	return i>j?i+1:j+1;
}


TElemType Root(BiTree T)
{ 
	if(BiTreeEmpty(T))
		return Nil;
	else
		return T->data;
}


TElemType Value(BiTree p)
{
	return p->data;
}


void Assign(BiTree p,TElemType value)
{
	p->data=value;
}


void PreOrderTraverse(BiTree T)
{ 
	if(T==NULL)
		return;
	printf("%c",T->data);
	PreOrderTraverse(T->lchild); 
	PreOrderTraverse(T->rchild); 
}


void InOrderTraverse(BiTree T)
{ 
	if(T==NULL)
		return;
	InOrderTraverse(T->lchild); 
	printf("%c",T->data);
	InOrderTraverse(T->rchild); 
}


void PostOrderTraverse(BiTree T)
{
	if(T==NULL)
		return;
	PostOrderTraverse(T->lchild); 
	PostOrderTraverse(T->rchild); 
	printf("%c",T->data);
}


int main()
{
	int i;
	BiTree T;
	TElemType e1;
	InitBiTree(&T);

	
	StrAssign(str,"ABDH#K###E##CFI###G#J##");

	CreateBiTree(&T);
	
	return 0;
}
