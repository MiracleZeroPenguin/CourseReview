#include <stdio.h>
#include <stdlib.h>

#define MaxVertices 10  /* maximum number of vertices */
typedef int Vertex;     /* vertices are numbered from 0 to MaxVertices-1 */
typedef struct VNode *PtrToVNode;
struct VNode {
	Vertex Vert;
	PtrToVNode Next;
};
typedef struct GNode *Graph;
struct GNode {
	int NumOfVertices;
	int NumOfEdges;
	PtrToVNode *Array;
};

Graph ReadG(); /* details omitted */

void PrintV(Vertex V)
{
	printf("%d ", V);
}

void StronglyConnectedComponents(Graph G, void(*visit)(Vertex V));

int main3()
{
	Graph G = ReadG();
	StronglyConnectedComponents(G, PrintV);
	return 0;
}

Graph ReadG1() {
	int a, b;
	Graph G = (Graph)malloc(sizeof(struct GNode));
	scanf("%d%d", &G->NumOfVertices, &G->NumOfEdges);
	G->Array = (PtrToVNode *)malloc(sizeof(PtrToVNode) * G->NumOfVertices);
	for (int i = 0; i < G->NumOfVertices; i++) {
		G->Array[i] = NULL;
	}
	for (int i = 0; i < G->NumOfEdges; i++) {
		scanf("%d%d", &a, &b);
		PtrToVNode p = (PtrToVNode)malloc(sizeof(struct VNode));
		p->Vert = b;
		p->Next = G->Array[a];
		G->Array[a] = p;
	}
	return G;
}



/* Your function will be put here */

void StronglyConnectedComponents(Graph G, void(*visit)(Vertex V)) {
	int connect[15][15] = { 0 };
	for (int i = 0; i < G->NumOfVertices; i++) {
		connect[i][i] = 1;
		PtrToVNode tmpnode = G->Array[i];
		while (tmpnode != NULL) {
			connect[i][tmpnode->Vert] = 1;
			tmpnode = tmpnode->Next;
		}
	}

	for (int i = 0; i < G->NumOfVertices; i++) {
		int access[15] = { 0 };
		int ss;
		int stack[15];
		ss = 0;
		stack[++ss] = i;
		while (ss != 0) {
			int j = stack[ss];
			ss--;
			PtrToVNode b;
			b = G->Array[j];
			while (b != NULL) {
				connect[i][b->Vert] = 1;
				if (access[b->Vert] == 0) {
					access[b->Vert] = 1;
					stack[++ss] = b->Vert;
				}
				b = b->Next;
			}
		}
	}
	
	int record[15] = { 0 };
	for (int i = 0; i < G->NumOfVertices;) {
		int first = 0;
		for (int j = i; j < G->NumOfVertices; j++) {
			if (connect[i][j] == 1 && connect[j][i] == 1) {
				record[j] = 1;
				if (first != 0)
					printf(" ");
				else
					first = 1;
				(*visit)(j);
			}
		}
		printf("\n");
		while (record[i] != 0) {
			i++;
			if (i > G->NumOfVertices)break;
		}
	}
}