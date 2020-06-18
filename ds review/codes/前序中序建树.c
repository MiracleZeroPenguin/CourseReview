#include <stdio.h>
#include <malloc.h>
#define MaxSize 100
typedef char ElemType;
typedef struct node 
{   ElemType data;              //����Ԫ��
    struct node *lchild;        //ָ������
    struct node *rchild;        //ָ���Һ���
} BTNode;

void DestroyBTree(BTNode *(&b))
{   if(b!=NULL)  //���ٶ�����
    {  DestroyBTree(b->lchild);
       DestroyBTree(b->rchild);
       free(b);
    }
}
void DispBTree(BTNode *b)  //�����ű�ʾ�����������
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
{       //����������pre����������in���������
    BTNode *s;
    char *p;
    int k;
    if (n<=0) return NULL;
    s=(BTNode *)malloc(sizeof(BTNode));     //�����������ڵ�*s
    s->data=*pre;
    for (p=in;p<in+n;p++)          //�������������ҵ���*pre��λ��k
        if (*p==*pre)
            break;
    k=p-in;                        //kΪ�������in�е��±� 
    s->lchild=CreateBT1(pre+1,in,k);       //�ݹ鹹�������� 
    s->rchild=CreateBT1(pre+k+1,p+1,n-k-1);//�ݹ鹹��������
    return s;
}
BTNode *CreateBT2(char *post,char *in,int n)
{      //�ɺ�������post����������in���������
    BTNode *b; char r, *p; int k;
    if (n<=0) return NULL;
    r = *(post+n-1);                      //ȡ�����ֵ
    b=(BTNode *)malloc(sizeof(BTNode));   //�����������ڵ�*b
    b->data=r;
    for(p=in;p<in+n;p++)                  //��in�в��Ҹ���� 
        if(*p == r)  break;
    k = p-in;                             //kΪ�������in�е��±� 
    b->lchild=CreateBT2(post,in,k);       //�ݹ鹹�������� 
    b->rchild=CreateBT2(post+k,p+1,n-k-1);//�ݹ鹹��������
    return b;
}

int main()
{
    BTNode *b;
    ElemType pre[]="ABDEHJKLMNCFGI";
    ElemType in[]="DBJHLKMNEAFCGI";
    ElemType post[]="DJLNMKHEBFIGCA";

    /*printf("��������:%s\n",pre);
    printf("��������:%s\n",in);
    printf("����һ�ö�����b:\n");   b=CreateBT1(pre,in,14);
    printf("  ���ű�ʾ��:");DispBTree(b);printf("\n");*/

    printf("��������:%s\n",in);
    printf("��������:%s\n",post);
    printf("����һ�ö�����b:\n");   b=CreateBT2(post,in,14);
    printf(" ���ű�ʾ��:"); DispBTree(b); printf("\n");
    DestroyBTree(b);
}

