
// buckets sort in arrays, the same element in each bucket

#include<iostream>

#include<string.h>         // memset函数在此头文件中定义

#define MAX_LEN 100

using namespace std;

 

int main()

{

    int arr[]={3,1,4,8,2,13,3,5,2};                      // 简单情形：没有小数(如果有小数，可以考虑先整体*10^n，n为小数位最大值，后面再复原) 

    int i, bucket[MAX_LEN];

    memset(bucket,0,sizeof(bucket));              // 用多个桶分别来记录相应索引i在原数组arr中出现的次数，全初始化为0 

    int ElemNum=sizeof(arr)/sizeof(arr[0]);     // 计算原序列中数的个数，记为ElemNum

    for(i=0;i<ElemNum;i++)

    {

        int v=arr[i];

        bucket[v]++;     // 记录相应索引i在原数组arr中出现的次数，没有出现的元素，存默认的0到数组bucket中 

    }

    for(i=0;i<MAX_LEN;i++)

    {

        while(bucket[i]>0)

        {

            cout<<i<<" ";          // 按序出桶

            bucket[i]--;

        }

    }

    cout<<endl;

    return 0;

}
