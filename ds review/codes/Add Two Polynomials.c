#include <stdio.h>
#include <stdlib.h>
typedef struct Node *PtrToNode;
struct Node  {
    int Coefficient;
    int Exponent;
    PtrToNode Next;
};
typedef PtrToNode Polynomial;

Polynomial Read(); /* details omitted */
void Print( Polynomial p ); /* details omitted */
Polynomial Add( Polynomial a, Polynomial b );

int main()
{
    Polynomial a, b, s;
    a = Read();
    b = Read();
    s = Add(a, b);
    Print(s);
    return 0;
}

/* Your function will be put here */
Polynomial Add(Polynomial a, Polynomial b) {

	//分配了一个不存放有效数据的头结点
	Polynomial pHead = (Polynomial)malloc(sizeof(struct Node));
	if (NULL == pHead)
	{
		exit(-1);
	}
	Polynomial pTail = pHead;
	pTail->Next = NULL;
	a = a->Next;
	b = b->Next;

	while(a!=NULL&&b!=NULL)
	{
		Polynomial pNew = (Polynomial)malloc(sizeof(struct Node));
		if (NULL == pNew)
		{
			exit(-1);
		}
		
		if (a->Exponent > b->Exponent) {
			pNew->Exponent = a->Exponent;
			pNew->Coefficient = a->Coefficient;
			a = a->Next;
		}
		else if(a->Exponent < b->Exponent) {
			pNew->Exponent = b->Exponent;
			pNew->Coefficient = b->Coefficient;
			b = b->Next;
		}
		else if (a->Exponent == b->Exponent) {
			pNew->Exponent = a->Exponent;
			pNew->Coefficient = a->Coefficient + b->Coefficient;
			a = a->Next;
			b = b->Next;
		}
		if (pNew->Coefficient != 0) {
		pTail->Next = pNew;
		pNew->Next = NULL;
		pTail = pNew;
		}

	}
	while(a != NULL) {
		Polynomial pNew = (Polynomial)malloc(sizeof(struct Node));
		pNew->Exponent = a->Exponent;
		pNew->Coefficient = a->Coefficient;
		a = a->Next;
		pTail->Next = pNew;
		pNew->Next = NULL;
		pTail = pNew;
	}
	while (b != NULL) {
		Polynomial pNew = (Polynomial)malloc(sizeof(struct Node));
		pNew->Exponent = b->Exponent;
		pNew->Coefficient = b->Coefficient;
		b = b->Next;
		pTail->Next = pNew;
		pNew->Next = NULL;
		pTail = pNew;
	}
	return pHead;
}
