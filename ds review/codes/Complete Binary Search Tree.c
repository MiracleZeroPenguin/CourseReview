#include<stdio.h>
#include<string.h>
#include<math.h>
#define MinData -1
typedef int ElementType;

typedef struct HeapStruct *BinaryQueue;
struct HeapStruct {
	int Capacity;
	int Size;
	ElementType  *Elements;
};

BinaryQueue Initialize(int MaxElements);
void Sort(BinaryQueue H);
BinaryQueue BuildBST(BinaryQueue T);
void RecursiveBST(int i, int Lnum, int Rnum, BinaryQueue T, BinaryQueue BST);

//初始化
BinaryQueue Initialize(int MaxElements) {
	BinaryQueue H;
	if (MaxElements < MinData)printf("Priority queue size is too small");
	H = malloc(sizeof(struct HeapStruct));
	if (H == NULL)printf("Out of Space");

	H->Elements = malloc((MaxElements + 1) * sizeof(ElementType));
	if (H->Elements == NULL)printf("Out of Space");

	H->Capacity = MaxElements;
	H->Size = 0;
	H->Elements[0] = MinData;

	return H;
}

//升序
void Sort(BinaryQueue H) {
	int len = H->Capacity ;
	for (int i = 0; i < len - 1; i++) /* 外循环为排序趟数，len个数进行len-1趟 */
		for (int j = 0; j < len - 1 - i; j++) { /* 内循环为每趟比较的次数，第i趟比较len-i次 */
			if (H->Elements[j] > H->Elements[j + 1]) { /* 相邻元素比较，若逆序则交换（升序为左大于右，降序反之） */
				ElementType temp;
				temp = H->Elements[j];
				H->Elements[j] = H->Elements[j + 1];
				H->Elements[j + 1] = temp;
			}
		}
}

//创建BST序列
BinaryQueue BuildBST(BinaryQueue T) {
	BinaryQueue BST;
	BST = Initialize(T->Size);
	RecursiveBST(0, 0, BST->Capacity - 1, T, BST);
	return BST;
}

int IntPow(int a, int b) {
	int ans, i;
	ans = 1;
	for (i = 1; i <= b; i++) {
		ans = ans * a;
	}
	return ans;
}

//循环
void RecursiveBST(int i, int Lnum, int Rnum, BinaryQueue T, BinaryQueue BST) {
	if (i < BST->Capacity) {
		if (Lnum == Rnum) {
			BST->Elements[i] = T->Elements[Lnum];
			BST->Size++;
			return;
		}
		int c;
		c = floor(FastLog2(Rnum - Lnum + 1)) + 1;
		if (Rnum - Lnum + 2 > 3 * IntPow(2 , (c - 2))) {
			BST->Elements[i] = T->Elements[Lnum + IntPow(2, (c - 1)) - 1];
			BST->Size++;
			if (i * 2 + 1 <= BST->Capacity - 1)
				RecursiveBST(i * 2 + 1, Lnum, Lnum+IntPow(2, (c - 1)) - 2, T, BST);
			if (i * 2 + 2 <= BST->Capacity - 1)
				RecursiveBST(i * 2 + 2, Lnum+IntPow(2, (c - 1)), Rnum, T, BST);
		}
		else {
			//BST->Elements[i] = T->Elements[-IntPow(2, (c - 2)) + Rnum - Lnum + 1];
			BST->Elements[i] = T->Elements[-IntPow(2, (c - 2)) + Rnum + 1];
			BST->Size++;
			if (i * 2 + 1 <= BST->Capacity - 1)
				//RecursiveBST(i * 2 + 1, Lnum, -IntPow(2, (c - 2)) + Rnum - Lnum , T, BST);
				RecursiveBST(i * 2 + 1, Lnum, -IntPow(2, (c - 2)) + Rnum , T, BST);
			if (i * 2 + 2 <= BST->Capacity - 1)
				//RecursiveBST(i * 2 + 2, -IntPow(2, (c - 2)) + Rnum - Lnum + 2, Rnum, T, BST);
				RecursiveBST(i * 2 + 2, -IntPow(2, (c - 2)) + Rnum + 2, Rnum, T, BST);
		}
		return;
		
	}
	else {
		return;
	}
}



int FastLog2(int x)
{
	float fx;
	unsigned long ix, exp;

	fx = (float)x;
	ix = *(unsigned long*)&fx;
	exp = (ix >> 23) & 0xFF;

	return exp - 127;
}


int main(){
	int n;
	BinaryQueue H;
	BinaryQueue BST;
	scanf("%d", &n);
	H = Initialize(n);
	for (int i = 0; i < n; i++) {
		int tmp;
		scanf("%d", &tmp);
		H->Elements[i] = tmp;
	}
	H->Size = n;
	Sort(H);
	BST = BuildBST(H);
	printf("%d", BST->Elements[0]);
	for (int i = 1; i < BST->Size; i++)
		printf(" %d", BST->Elements[i]);
	system("pause");
	return 0;

}