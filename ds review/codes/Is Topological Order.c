#include <stdio.h>
#include <stdlib.h>

typedef enum {false, true} bool;
#define MaxVertexNum 10  /* maximum number of vertices */
typedef int Vertex;      /* vertices are numbered from 1 to MaxVertexNum */

typedef struct AdjVNode *PtrToAdjVNode; 
struct AdjVNode{
    Vertex AdjV;
    PtrToAdjVNode Next;
};

typedef struct Vnode{
    PtrToAdjVNode FirstEdge;
} AdjList[MaxVertexNum];

typedef struct GNode *PtrToGNode;
struct GNode{  
    int Nv;
    int Ne;
    AdjList G;
};
typedef PtrToGNode LGraph;

LGraph ReadG(); /* details omitted */

bool IsTopSeq( LGraph Graph, Vertex Seq[] );

int main()
{
    int i, j, N;
    Vertex Seq[MaxVertexNum];
    LGraph G = ReadG();
    scanf("%d", &N);
    for (i=0; i<N; i++) {
        for (j=0; j<G->Nv; j++)
            scanf("%d", &Seq[j]);
        if ( IsTopSeq(G, Seq)==true ) printf("yes\n");
        else printf("no\n");
    }

    return 0;
}

/* Your function will be put here */
bool IsTopSeq(LGraph Graph, Vertex Seq[]) {
	int Degree[1000];
	for (int i = 0; i <= Graph->Nv; i++) {
		Degree[i] = 0;
	}
	PtrToAdjVNode tnode;
	for (int i = 0; i < Graph->Nv; i++) {
		tnode = Graph->G[i].FirstEdge;
		while (tnode) {
			Degree[tnode->AdjV]++;
			tnode = tnode->Next;
		}
	}
	for (int i = 0; i < Graph->Nv; i++) {
		if (Degree[Seq[i] - 1] == 0) {
			tnode = Graph->G[Seq[i] - 1].FirstEdge;
			while (tnode) {
				Degree[tnode->AdjV]--;
				tnode = tnode->Next;
			}
		}
		else
			return false;
	}
	return true;
}