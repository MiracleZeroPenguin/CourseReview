#include <stdio.h>

#define MAXN 10

int IsCBST(int A[], int N);

int main()
{
	int A[MAXN], N, i;

	scanf("%d", &N);
	for (i = 0; i < N; i++) scanf("%d", &A[i]);
	if (IsCBST(A, N)) printf("Yes\n");
	else printf("No\n");
	for (i = 0; i < N; i++) printf("%d ", A[i]);

	return 0;
}


typedef struct BiNode * BiTree;
struct BiNode {
	int Key;
	BiTree Left;
	BiTree Right;
};

BiTree BuildBiTree(BiTree T, int key);
BiTree BuildBiTree(BiTree T, int key) {
	if (T == NULL) {
		T = malloc(sizeof(struct BiNode));
		T->Key = key;
		T->Left = NULL;
		T->Right = NULL;
		return T;
	}
	if (T->Key < key) {
		T->Left = BuildBiTree(T->Left, key);
	}
	else if (T->Key >key) {
		T->Right = BuildBiTree(T->Right, key);
	}
	return T;
}

int checkCBST(BiTree T);
int checkCBST(BiTree T) {
	BiTree b;
	b = T;
	struct BiNode* qu[MAXN], *p;
	int front = -1, rear = -1;
	int cm = 1;
	int bj = 1;
	if (b != NULL) {
		rear++;
		qu[rear] = b;
		
		while (front != rear) {
			front = (front + 1) % MAXN;
			p = qu[front];
			if (p->Left == NULL) {
				bj = 0;
				if (p->Right != NULL)
					cm = 0;
			}
			else {
				if (bj) {
					rear = (rear + 1) % MAXN;
					qu[rear] = p->Left;
					if (p->Right == NULL)
						bj = 0;
					else {
						rear = (rear + 1) % MAXN;
						qu[rear] = p->Right;
					}
				}
				else
					cm = 0;
			}
		}
		return cm;
	}
	return 1;
}

int i = 0;
void PreOrderTraversal(BiTree BT,int A[])
{
	if (BT)
	{
		A[i] = BT->Key;
		i++;
		PreOrderTraversal(BT->Left,A);     //访问左儿�?
		PreOrderTraversal(BT->Right,A);    //访问右儿�?
	}
}

/* Your function will be put here */
int IsCBST(int A[], int N) {
	BiTree T0 = NULL;
	for (int i = 0; i < N; i++) {
		T0 = BuildBiTree(T0, A[i]);
	}
	int iscbst = checkCBST(T0);
	PreOrderTraversal(T0, A);
	if (iscbst) {
		return 1;
	}
	else {
		return 0;
	}
	
}