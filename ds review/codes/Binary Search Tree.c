#include<stdio.h>
#include<stdlib.h>

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
	if (T->Key > key) {
		T->Left = BuildBiTree(T->Left, key);
	}
	else if (T->Key < key) {
		T->Right = BuildBiTree(T->Right, key);
	}
	return T;
}
int CheckSame(BiTree T0, BiTree T1) {
	int flag;
	if (T0 == NULL && T1 == NULL)return 1;
	else if (T0 != NULL && T1 == NULL)return 0;
	else if (T0 == NULL && T1 != NULL)return 0;
	else if (T0->Key != T1->Key)return 0;
	else {
		flag = CheckSame(T0->Left, T1->Left)*CheckSame(T0->Right, T1->Right);
		return flag;
	}
}
int main() {
	int num, i,j,key,times;

	//scanf("%d %d", &num,&times);
	while (scanf("%d", &num) && num) {
		scanf("%d", &times);
		BiTree T0 = NULL;

		int flag;
		for (i = 0; i < num; i++) {
			scanf("%d", &key);
			T0 = BuildBiTree(T0, key);
		}
		for (j = 0; j < times; j++) {
			BiTree T1 = NULL;
			for (i = 0; i < num; i++) {
				scanf("%d", &key);
				T1 = BuildBiTree(T1, key);
			}
			flag = CheckSame(T0, T1);
			if(flag==1)printf("Yes\n");
			else printf("No\n");
		}
	}

	system("pause");
}