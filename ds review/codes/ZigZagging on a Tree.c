#include<stdio.h>
#include<queue>
#include<algorithm> 
#define MaxN 32

using namespace std;

int in[MaxN], post[MaxN], level[MaxN], num[MaxN];
int n;
int maxLevel = 0;

void changePos(int l, int r) {
	int m = (l + r) / 2;
	for (int i = l; i <= m; i++) {
		int temp = num[i];
		num[i] = num[r - (i - l)];
		num[r - (i - l)] = temp;
	}
}

typedef struct Node {
	int data, level;
	Node *left;
	Node *right;
}*BTtree;


void build(BTtree &root, int l1, int r1, int l2, int r2) {
	if (l1 > r1) {
		return;
	}
	int m;
	for (int i = l1; i <= r1; i++) {
		if (in[i] == post[r2]) {
			m = i;
			break;
		}
	}
	if (root == NULL) {
		root = new Node;
		root->data = in[m];
		root->left = root->right = NULL;
		//return ;
	}
	build(root->left, l1, m - 1, l2, l2 + m - l1 - 1);
	build(root->right, m + 1, r1, r2 - r1 + m, r2 - 1);
}


void bfs(BTtree root) {
	queue<BTtree> q;
	root->level = 1;
	q.push(root);
	int now = 0;
	
	while (!q.empty()) {
		BTtree p = q.front();
		q.pop();
		level[p->level]++;
		if (p->level > maxLevel) {
			maxLevel = p->level;
		}

		num[now] = p->data;
		now++;
		if (p->left != NULL) {
			p->left->level = p->level + 1;
			q.push(p->left);
		}
		if (p->right != NULL) {
			p->right->level = p->level + 1;
			q.push(p->right);
		}
	}
}


int main() {
	BTtree root = NULL;
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		scanf("%d", &in[i]);
	}
	for (int i = 0; i < n; i++) {
		scanf("%d", &post[i]);
	}
	build(root, 0, n - 1, 0, n - 1);
	bfs(root);


	int pos = 0;
	for (int i = 1; i <= maxLevel; i++) {
		if (i % 2 == 1)
			changePos(pos, pos + level[i] - 1);
		for (int j = 1; j <= level[i]; j++) {
			printf("%d", num[pos++]);
			if (pos != n) {
				printf(" ");
			}
		}
	}
}
