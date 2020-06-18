#include <stdio.h>

#define ElementType int
#define MAXN 100

void merge_pass( ElementType list[], ElementType sorted[], int N, int length );

void output( ElementType list[], int N )
{
    int i;
    for (i=0; i<N; i++) printf("%d ", list[i]);
    printf("\n");
}

void  merge_sort( ElementType list[],  int N )
{
    ElementType extra[MAXN];  /* the extra space required */
    int  length = 1;  /* current length of sublist being merged */
    while( length < N ) { 
        merge_pass( list, extra, N, length ); /* merge list into extra */
        output( extra, N );
        length *= 2;
        merge_pass( extra, list, N, length ); /* merge extra back to list */
        output( list, N );
        length *= 2;
    }
} 


int main()
{
    int N, i;
    ElementType A[MAXN];

    scanf("%d", &N);
    for (i=0; i<N; i++) scanf("%d", &A[i]);
    merge_sort(A, N);
    output(A, N);

    return 0;
}

/* Your function will be put here */

void merge_pass( ElementType list[], ElementType sorted[], int N, int length ){
	int temp;
	if(length>N)return ;
	int i,j=0,count1=0,count2=0;
	int M=N;
	count1=0;
	count2=length;
    int start,end;
	int t1,t2;

	for(i=0;i<(N-1)/(2*length)+1;i++){
		t1=0;t2=0;
		if(i!=(N-1)/(2*length)){
			start=i*length*2;
			end=start+length;
			while(t1+t2!=2*length){
				if((list[t1+start]<=list[t2+end]||t2==length)&&t1<length){
					sorted[j++]=list[start+t1++];
				}
				if((list[t1+start]>=list[t2+end]||t1==length)&&t2<length){
					sorted[j++]=list[end+t2++];
				}
			}
		}
		else{
			if((N-1)%(2*length)+1<=length){
				while(j++<N)
				sorted[j-1]=list[j-1];
			}
			else{
				start=N-(N-1)%(2*length)-1;
				end=start+length;
				while(t1+t2!=N-start){
				if((list[t1+start]<=list[t2+end]||t2==N-end)&&t1<length){
					sorted[j++]=list[start+t1++];
				}
				if((list[t1+start]>=list[t2+end]||t1==length)&&t2<N-end){
					sorted[j++]=list[end+t2++];
				}
			}
			}
		}
	}
}
