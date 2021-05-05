#include<stdio.h>
#include<string.h>
int main(){
    char info[1000]={0};        //store infomation read from log
    int file;                   //a variable to store if in the end of log
    FILE *fp=fopen("/var/log/kern.log","r");    //store the log file
    if(fp!=NULL){               //if open succesfully
        file=feof(fp);          //check if it is in the end of log
        while(!file){           //if not in the end
            memset(info,0,sizeof(info));    //initialize the space to store
            fgets(info,sizeof(info)-1,fp);  //read the log to the info
            printf("%s",info);              //print out
        }
        fclose(fp);
        printf("************END of LOG****************\n");
        return 0;               //end the programm
    }else{                      //if open unsuccessfully
        printf("Open log fail\n");
        return -1;              //return error
    }
}