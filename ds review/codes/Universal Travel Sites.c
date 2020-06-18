#include<stdio.h>
#include<string.h>
#define Nmax 1000
int graph[Nmax][Nmax] = { 0 };
int visit[Nmax] = { 0 };
int N;

void readData();
void dfs(int a);
void cutRoad();
struct Stack *stackCreate();
struct pathstack * pathCreate();
//int stackInsert(struct Stack *p, char *name);
//void pathInsert(struct pathstack *p, int id);
int capacity = 0;

typedef struct {
	char name[Nmax][5];
	int size;
}Stack;
Stack *vertexList;


typedef struct {
	int id[Nmax];
	int size;
}pathstack;
pathstack *path;

int main() {
	vertexList = stackCreate();
	path = pathCreate();
	readData();
	dfs(0);
	printf("%d", capacity);
	return 0;
}

void dfs(int a) {
	visit[a] = 1;
	pathInsert(path,a);
	if (a == 1)
		cutRoad();
	else {
		for (int i = 0; i < vertexList->size; i++) {
			if (graph[a][i] > 0 && visit[i] == 0)
				dfs(i);
		}
	}
	visit[a] = 0;
	path->size--;
	return;
}

void cutRoad() {
	int total = path->size;
	int minnum = graph[path->id[0]][path->id[1]];
	for (int i = 1; i < total - 1; i++) {
		if (graph[path->id[i]][path->id[i + 1]]<minnum) {
			minnum = graph[path->id[i]][path->id[i + 1]];
		}
	}
	if (minnum == 0)return;
	capacity = capacity + minnum;
	for (int i = 0; i < total-1; i++) {
		graph[path->id[i]][path->id[i + 1]] = graph[path->id[i]][path->id[i + 1]] - minnum;
	}
	return;
}

void readData() {
	char a[5], b[5];
	int aid, bid;
	scanf("%s %s %d", a, b, &N);
	aid=stackInsert(vertexList, a);
	bid=stackInsert(vertexList, b);
	for (int i = 0; i < N; i++) {
		int weight;
		scanf("%s %s %d", a, b, &weight);
		aid = stackInsert(vertexList, a);
		bid = stackInsert(vertexList, b);
		graph[aid][bid] = weight;
	}
	return;
}

struct pathstack * pathCreate() {
	pathstack *p = (pathstack*)malloc(sizeof(pathstack));
	if (p == NULL)return 0;
	p->size = 0;
	return p;
}

struct Stack* stackCreate() {
	Stack *p = (Stack*)malloc(sizeof(Stack));
	if (p == NULL)return 0;
	p->size = 0;
	return p;
}

int stackInsert(Stack *p, char *name) {
	for (int i = 0; i < p->size; i++) {
		if (!strcmp(p->name[i], name)) {
			return i;
		}
	}
	strcpy(p->name[p->size], name);
	p->size++;
	return (p->size) - 1;
}

void pathInsert(pathstack *p, int id) {
	p->id[path->size] = id;
	p->size++;
}