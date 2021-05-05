#include<stdio.h>
#include<string.h>
int main(){
    char info[1000]={0};
    int fpopen;
    FILE *fp=fopen("/var/log/kern.log","r");
    if(fp!=NULL){
        fpopen=feof(fp);
        while(!fpopen){
            memset(info,0,sizeof(info));
            fgets(info,sizeof(info)-1,fp);
            printf("%s",info);
        }

        fclose(fp);
        printf("************END of LOG****************\n");
        return 0;
    }else{
        printf("Open log fail\n");
        return -1;
    }
}