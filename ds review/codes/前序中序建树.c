#include <stdio.h>
#include <malloc.h>
#define MaxSize 100
typedef char ElemType;
typedef struct node 
{   ElemType data;              //数据元素
    struct node *lchild;        //指向左孩子
    struct node *rchild;        //指向右孩子
} BTNode;

void DestroyBTree(BTNode *(&b))
{   if(b!=NULL)  //销毁二叉树
    {  DestroyBTree(b->lchild);
       DestroyBTree(b->rchild);
       free(b);
    }
}
void DispBTree(BTNode *b)  //以括号表示法输出二叉树
{
    if(b!=NULL)
    {
        printf("%c", b->data);
        if(b->lchild!=NULL || b->rchild!=NULL)
        {
            printf("(");
            DispBTree(b->lchild);
            if(b->rchild!=NULL)
                printf(",");
            DispBTree(b->rchild);
            printf(")");
        }
    }
}

BTNode *CreateBT1(char *pre,char *in,int n)
{       //由先序序列pre和中序序列in构造二叉树
    BTNode *s;
    char *p;
    int k;
    if (n<=0) return NULL;
    s=(BTNode *)malloc(sizeof(BTNode));     //创建二叉树节点*s
    s->data=*pre;
    for (p=in;p<in+n;p++)          //在中序序列中找等于*pre的位置k
        if (*p==*pre)
            break;
    k=p-in;                        //k为根结点在in中的下标 
    s->lchild=CreateBT1(pre+1,in,k);       //递归构造左子树 
    s->rchild=CreateBT1(pre+k+1,p+1,n-k-1);//递归构造右子树
    return s;
}
BTNode *CreateBT2(char *post,char *in,int n)
{      //由后序序列post和中序序列in构造二叉树
    BTNode *b; char r, *p; int k;
    if (n<=0) return NULL;
    r = *(post+n-1);                      //取根结点值
    b=(BTNode *)malloc(sizeof(BTNode));   //创建二叉树节点*b
    b->data=r;
    for(p=in;p<in+n;p++)                  //在in中查找根结点 
        if(*p == r)  break;
    k = p-in;                             //k为根结点在in中的下标 
    b->lchild=CreateBT2(post,in,k);       //递归构造左子树 
    b->rchild=CreateBT2(post+k,p+1,n-k-1);//递归构造右子树
    return b;
}

int main()
{
    BTNode *b;
    ElemType pre[]="ABDEHJKLMNCFGI";
    ElemType in[]="DBJHLKMNEAFCGI";
    ElemType post[]="DJLNMKHEBFIGCA";

    /*printf("先序序列:%s\n",pre);
    printf("中序序列:%s\n",in);
    printf("构造一棵二叉树b:\n");   b=CreateBT1(pre,in,14);
    printf("  括号表示法:");DispBTree(b);printf("\n");*/

    printf("中序序列:%s\n",in);
    printf("后序序列:%s\n",post);
    printf("构造一棵二叉树b:\n");   b=CreateBT2(post,in,14);
    printf(" 括号表示法:"); DispBTree(b); printf("\n");
    DestroyBTree(b);
}

