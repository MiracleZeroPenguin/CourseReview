#include <stdio.h>
#include <stdlib.h>

typedef struct TNode *BinTree;
struct TNode{
    int Key;
    BinTree Left;
    BinTree Right;
};

BinTree BuildTree(); /* details omitted */
int CheckBST ( BinTree T, int K );

int main()
{
    BinTree T;
    int K, out;

    T = BuildTree();
    scanf("%d", &K);
    out = CheckBST(T, K);
    if ( out < 0 )
        printf("No.  Height = %d\n", -out);
    else
        printf("Yes.  Key = %d\n", out);

    return 0;
}
/* 你的代码将被嵌在这里 */
int GetLevel(BinTree T) {
	int level, rlevel, llevel;
	if (T->Left == NULL && T->Right == NULL)return 1;
	else if (T->Left != NULL && T->Right == NULL)return 1 + GetLevel(T->Left);
	else if (T->Left == NULL && T->Right != NULL)return 1 + GetLevel(T->Right);
	else {
		rlevel = GetLevel(T->Right);
		llevel = GetLevel(T->Left);
		level = 1 + (rlevel > llevel ? rlevel : llevel);
		return level;
	}
}

int CheckBST(BinTree T, int K) {
	int depth = GetLevel(T);
	BinTree stack[1000]; int s[1000];
	s[0] = -0x3f3f3f;
	int top = 0; int i = 1;
	BinTree cur = T;
	while (top || cur) {
		if (cur) {
			stack[top++] = cur;
			cur = cur->Left;
		}
		else {
			cur = stack[top - 1];
			if (cur->Key < s[i - 1])
				return -depth;
			s[i++] = stack[--top]->Key;
			cur = cur->Right;
		}
	}
	return s[i - K];
}
