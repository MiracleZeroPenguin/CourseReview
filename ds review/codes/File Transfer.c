/*
I stands for inputting a connection between c1 and c2; 
C stands for checking if it is possible to transfer files between c1 and c2
S stands for stopping this case. 
5
C 3 2
I 3 2
C 1 5
I 4 5
I 2 4
C 3 5
S
no
no
yes
There are 2 components.
*/

#include<stdio.h>
#include<stdlib.h>

int N;
int Find(int* S,int X)
{
	if(S[X]<0)	return X;
	else	return S[X]=Find(S,S[X]);//路径压缩，通过不断地压缩路径，让树越来越扁 */
}
void Union(int* S,int X1,int X2)
{
	int root1=Find(S,X1-1);
	int root2=Find(S,X2-1);
	if(root1==root2)
	{
		S[root2]=root1;
		root1-=root2;
	}
	else
	{
		//按秩归并 
		if(root1<root2)//root1 is bigger than root2,so we Union root2 to root1
		{
			S[root2]=root1;
			root1-=root2;//update the value of root2
		}
		else
		{
			S[root1]=root2;
			root2-=root1;
		}
	}
}
void Input_connection(int* S)
{
	int x1,x2;
	scanf("%d %d",&x1,&x2);
	Union(S,x1,x2);
}
void Check_connection(int* S)
{
	int x1,x2;
	scanf("%d %d",&x1,&x2);
	int root1,root2;
	root1=Find(S,x1-1);
	root2=Find(S,x2-1);
	if(root1==root2)
		printf("yes\n");
	else
		printf("no\n");
}
void Check_network(int* S,int N)
{
	int flag=0;
	for(int i=0;i<N;i++)
	{
		if(S[i]<0)
			flag++;
	}
	if(flag==1)
		printf("The network is connected.");
	else
		printf("There are %d components.\n",flag);
} 
int main()
{
	scanf("%d",&N);
	int S[N];
	for(int i=0;i<N;i++)
		S[i]=-1;
	char c;
	do
	{
		scanf("%c",&c);
		switch(c)
		{
			case 'C':	Check_connection(S);break;
			case 'S':	Check_network(S,N);break; 
			case 'I':	Input_connection(S);break;
		}
	}while(c !='S');
	return 0;
} 

