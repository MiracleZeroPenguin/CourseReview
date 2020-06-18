#include <stdio.h>
#include <stdlib.h>

#define ElementType int
#define ERROR 1e5
typedef enum { push, pop, inject, eject, end } Operation;

typedef struct Node *PtrToNode;
struct Node {
    ElementType Element;
    PtrToNode Next, Last;
};
typedef struct DequeRecord *Deque;
struct DequeRecord {
    PtrToNode Front, Rear;
};
Deque CreateDeque();
int Push( ElementType X, Deque D );
ElementType Pop( Deque D );
int Inject( ElementType X, Deque D );
ElementType Eject( Deque D );

Operation GetOp();          /* details omitted */
void PrintDeque( Deque D ); /* details omitted */

int main()
{
    ElementType X;
    Deque D;
    int done = 0;

    D = CreateDeque();
    while (!done) {
        switch(GetOp()) {
        case push: 
            scanf("%d", &X);
            if (!Push(X, D)) printf("Memory is Full!\n");
            break;
        case pop:
            X = Pop(D);
            if ( X==ERROR ) printf("Deque is Empty!\n");
            break;
        case inject: 
            scanf("%d", &X);
            if (!Inject(X, D)) printf("Memory is Full!\n");
            break;
        case eject:
            X = Eject(D);
            if ( X==ERROR ) printf("Deque is Empty!\n");
            break;
        case end:
            PrintDeque(D);
            done = 1;
            break;
        }
    }
    return 0;
}

/* Your function will be put here */
Deque CreateDeque() {
	Deque newdeq = (Deque)malloc(sizeof(struct DequeRecord));
	newdeq->Front = (PtrToNode)malloc(sizeof(struct Node));
	newdeq->Front->Last = NULL;
	newdeq->Rear = newdeq->Front;
	newdeq->Rear->Next = NULL;
	return newdeq;
}

int Push(ElementType X, Deque D) {
	struct Node* t;
	t = (struct Node*)malloc(sizeof(struct Node));
	if (!t)return 0;
	else {
		t->Element = X;
		if (D->Front == D->Rear) {
			D->Front->Next = t;
			t->Last = D->Front;
			D->Rear = t;
			t->Next = NULL;
			return 1;
		}
		else {
			t->Last = D->Front;
			t->Next = D->Front->Next;
			D->Front->Next->Last = t;
			D->Front->Next = t;
			return 1;
		}
	}
}

ElementType Pop(Deque D) {
	if (D->Front == D->Rear)return ERROR;
	else {
		ElementType ans;
		ans = D->Front->Next->Element;
		struct Node* t = D->Front->Next;
		if (D->Front->Next == D->Rear) {
			D->Rear = D->Front;
			D->Rear->Next = NULL;
		}
		else {
			D->Front->Next->Next->Last = D->Front;
			D->Front->Next = D->Front->Next->Next;
		}
		free(t);
		return ans;
	}
}

int Inject(ElementType X, Deque D) {
	struct Node* t;
	t = (struct Node*)malloc(sizeof(struct Node));
	if (!t)return 0;
	else {
		t->Element = X;
		if (D->Front == D->Rear) {
			D->Front->Next = t;
			t->Last = D->Front;
			D->Rear = t;
			return 1;
		}
		else {
			D->Rear->Next = t;
			t->Last = D->Rear;
			t->Next = NULL;
			D->Rear = t;
			return 1;
		}
	}
}

ElementType Eject(Deque D) {
	if (D->Front == D->Rear)return ERROR;
	else {
		ElementType ans;
		ans = D->Rear->Element;
		struct Node* t = D->Rear;
		if (D->Front->Next == D->Rear) {
			D->Rear = D->Front;
			D->Rear->Next = NULL;
		}
		else {
			D->Rear = D->Rear->Last;
			D->Rear->Next = NULL;
		}

		free(t);
		return ans;
	}
}