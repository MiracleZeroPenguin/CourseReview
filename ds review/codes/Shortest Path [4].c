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

MGraph ReadG(); /* details omitted */

MGraph ReadG(void)
{
	MGraph G = (MGraph)malloc(sizeof(struct GNode));
	scanf("%d%d", &G->Nv, &G->Ne);
	int i, j;
	for (i = 0; i < G->Nv; i++) {
		for (j = 0; j < G->Nv; j++) {
			G->G[i][j] = INFINITY;
		}
	}
	for (i = 0; i < G->Ne; i++) {
		int a, b, x;
		scanf("%d%d%d", &a, &b, &x);
		G->G[a][b] = x;
	}
	return G;
}

void ShortestDist(MGraph Graph, int dist[], int path[], Vertex S);

int main5()
{
	int dist[MaxVertexNum], path[MaxVertexNum];
	Vertex S, V;
	MGraph G = ReadG();

	scanf("%d", &S);
	ShortestDist(G, dist, path, S);

	for (V = 0; V < G->Nv; V++)
		printf("%d ", dist[V]);
	printf("\n");
	for (V = 0; V < G->Nv; V++)
		printf("%d ", path[V]);
	printf("\n");

	return 0;
}
void ShortestDist(MGraph Graph, int dist[], int path[], Vertex S) {
	int visit[MaxVertexNum],num[MaxVertexNum];
	for (int i = 0; i < Graph->Nv; i++) {
		visit[i] = 0;
		num[i] = -1;
		dist[i] = INFINITY;
		path[i] = -1;
	}
	dist[S] = 0;
	do{
		int mindist = INFINITY;
		int minidx = -1;
		int i = 0;
		do {
			if (!visit[i] && mindist > dist[i]) {
				mindist = dist[i];
				minidx = i;
			}
			i++;
		} while (i < Graph->Nv);

		if (minidx == -1)
			break;
		
		visit[minidx] = 1;

		i = 0;
		while (i < Graph->Nv) {
			if (visit[i]) {
				i++; continue;
			}
			if (Graph->G[minidx][i] == INFINITY) {
				i++; continue;
			}
			if (Graph->G[minidx][i] + dist[minidx] == dist[i]) {
				if (num[i] > num[minidx] + 1) {
					num[i] = num[minidx] + 1;
					path[i] = minidx;
				}
			}
			else if (Graph->G[minidx][i] + dist[minidx] < dist[i]) {
				num[i] = num[minidx] + 1;
				path[i] = minidx;
				dist[i] = Graph->G[minidx][i] + dist[minidx];
			}
			i++;
		} 
	} while (1);
	
	int j = 0;
	while (j < Graph->Nv) {
		if (dist[j] == INFINITY) {
			dist[j] = -1;
			num[j] = -1;
		}
		j++;
	}
}
/* Your function will be put here */
