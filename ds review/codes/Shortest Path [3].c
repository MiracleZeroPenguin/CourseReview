#include <stdio.h>
#include <stdlib.h>

typedef enum { false, true } bool;
#define INFINITY 1000000
#define MaxVertexNum 10  /* maximum number of vertices */
typedef int Vertex;      /* vertices are numbered from 0 to MaxVertexNum-1 */
typedef int WeightType;

typedef struct GNode *PtrToGNode;
struct GNode {
	int Nv;
	int Ne;
	WeightType G[MaxVertexNum][MaxVertexNum];
};
typedef PtrToGNode MGraph;

MGraph ReadG4(); /* details omitted */
MGraph ReadG4(void)
{
	MGraph G = (MGraph)malloc(sizeof(struct GNode));
	scanf("%d%d", &G->Nv, &G->Ne);
	int i, j;
	for (i = 0; i < G->Nv; i++){
		for (j = 0; j < G->Nv; j++){
			G->G[i][j] = INFINITY;
		}
	}
	for (i = 0; i < G->Ne; i++){
		int a, b, x;
		scanf("%d%d%d", &a, &b, &x);
		G->G[a][b] = x;
	}
	return G;
}

void ShortestDist(MGraph Graph, int dist[], int count[], Vertex S);

int main4()
{
	int dist[MaxVertexNum], count[MaxVertexNum];
	Vertex S, V;
	MGraph G = ReadG();

	scanf("%d", &S);
	ShortestDist(G, dist, count, S);

	for (V = 0; V < G->Nv; V++)
		printf("%d ", dist[V]);
	printf("\n");
	for (V = 0; V < G->Nv; V++)
		printf("%d ", count[V]);
	printf("\n");

	return 0;
}

/* Your function will be put here */
void ShortestDist3(MGraph Graph, int dist[], int count[], Vertex S) {
	
	int visit[MaxVertexNum];
	for (int i = 0; i < Graph->Nv; i++) {
		visit[i] = 0;
		dist[i] = Graph->G[S][i];
		count[i] = 0;
	}
	dist[S] = 0;
	count[S] = 1;
	while (1)
	{
		int mindist = INFINITY;
		int minidx = -1;
		int i = 0;
		do{
			if (!visit[i] && dist[i] < mindist)
			{
				mindist = dist[i];
				minidx = i;
			}
			i++;
		} while (i < Graph->Nv);
		if (minidx == -1)break;
		visit[minidx] = 1;
		i = 0;
		do{
			if (Graph->G[minidx][i] != INFINITY && i != minidx)
			{
				if (dist[minidx] + Graph->G[minidx][i] == dist[i])
					count[i] += count[minidx];
				else if (dist[minidx] + Graph->G[minidx][i] < dist[i])
				{
					count[i] = count[minidx];
					dist[i] = dist[minidx] + Graph->G[minidx][i];
				}
			}
			i++;
		} while (i < Graph->Nv);
	}
	int x = 0;
	do{
		if (dist[x] == INFINITY)
		{
			count[x] = 0;
			dist[x] = -1;
		}
		x++;
	} while (x < Graph->Nv);
	
}