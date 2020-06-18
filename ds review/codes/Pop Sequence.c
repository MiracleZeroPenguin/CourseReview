
#include<stdlib.h>
#include<stdio.h>
#define MaxSize 1003

typedef int Data[MaxSize];
typedef struct StkNode* PtrtoStkNode;
typedef PtrtoStkNode Stack;

struct StkNode {
	int top;
	int size;
	Data record;

};

int Push(Stack S, int a) {
	if (S->top < S->size) {
		S->record[S->top++] = a;
		return 1;
	}
	else return 0;
}

int pop(Stack S) {
	if (S->top)return S->record[--S->top];
	else return 0;
}

int main() {
	int M, N, K, i, j, temp, rule, flag;
	Stack S = (Stack)malloc(sizeof(struct StkNode));
	scanf("%d %d %d", &M, &N, &K);
	S->size = M;
	for (i = 0; i < K; i++) {
		S->top = 0;
		rule = 1;
		flag = 1;
		for (j = 0; j < N; j++) {
			scanf("%d", &temp);
			while (flag && rule<= temp) { 
				flag = Push(S, rule);
				rule++;
			}
			if (pop(S) != temp) flag = 0;
		}
		if (!flag) printf("NO\n");
		else printf("YES\n");
	}
	return 0;
}
