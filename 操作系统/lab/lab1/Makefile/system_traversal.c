#include<linux/module.h>            //in order to create a module
#include<linux/kernel.h>            //in order to KERN_INFO
#include <linux/sched.h>            //in order to get task scheduling functions
#include <linux/sched/task.h>       //in order to use next_task
#include <linux/sched/signal.h>     //in order to use next_task

//Definition of global variables
int num_tot_proc=0;                 //total number of processors
int num_running_proc=0;             //number of running processors
int num_interruptible_proc=0;       //number of interruptible processors
int num_uninterruptible_proc=0;     //number of uninterruptible processors
int num_stopped_proc=0;             //number of stopped processors
int num_traced_proc=0;              //number of traced processors
int num_zombie_proc=0;              //number of zombie processors
int num_dead_proc=0;                //number of dead processors
int num_unknown=0;                  //number of unknown processors

//function to init the system_traversal module
int init_module(){
    struct task_struct *tmpTask;        //declare a pointer to tast_struct
    int tmpTask_state;                  //define an integer to store the temporary state
    int tmpTask_exitState;              //define an interger to store the temporary exit state
    printk(KERN_INFO"Info of all processors\n");    //begin to traversal
    for(tmpTask=&init_task;(tmpTask=next_task(tmpTask))!=&init_task;){      //move the pointer to the next task
        printk(KERN_INFO"Name:%s Pid:%d State:%ld ParName:%s\n",tmpTask->comm,tmpTask->pid,tmpTask->state,tmpTask->real_parent->comm);  //print out the information of the task
        num_tot_proc++;                 //number of total processors added
        tmpTask_state=tmpTask->state;   //store the state of the task
        tmpTask_exitState=tmpTask->exit_state;  //store the exit state of the task
        if(tmpTask_exitState!=0){//the process has exited
            switch (tmpTask_state)      //classify the task by its state
            {
            case EXIT_ZOMBIE:num_zombie_proc++;break;   //number of zombie processors added
            case EXIT_DEAD:num_dead_proc++;break;       //number of dead processors added
            default:break;
            }
        }else{//the process has not exited
            switch (tmpTask_state)      //classify the task by its state
            {
            case TASK_RUNNING:num_running_proc++;break;                     //number of running processors added
            case TASK_INTERRUPTIBLE:num_interruptible_proc++;break;         //number of interruptible processors added
            case TASK_UNINTERRUPTIBLE:num_uninterruptible_proc++;break;     //number of uninterruptible processors added
            case TASK_STOPPED:num_stopped_proc++;break;                     //number of stopped processors added
            case TASK_TRACED:num_traced_proc++;break;                       //number of traced processors added
            default:num_unknown++;break;                                    //number of unknown processors added
            }
        }
    }
    printk("*******************Statistic INFO*********************\n");
    printk("Total tasks:%d\n",num_tot_proc);                            //print out the total number of processors
    printk("TASK_RUNNING:%d\n",num_running_proc);                       //print out the number of running processors
    printk("TASK_INTERRUPTIBLE:%d\n",num_interruptible_proc);           //print out the number of interruptible processors
    printk("TASK_UNINTERRUPTIBLE:%d\n",num_uninterruptible_proc);       //print out the number of uninterruptible processors
    printk("TASK_STOP:%d\n",num_stopped_proc);                          //print out the number of stopped processors
    printk("TASK_TRACED:%d\n",num_traced_proc);                         //print out the number of traced processors
    printk("EXIT_ZOMBIE:%d\n",num_zombie_proc);                         //print out the number of zombie processors
    printk("EXIT_DEAD:%d\n",num_dead_proc);                             //print out the number of dead processors
    printk("UNKNOWN:%d\n",num_unknown);                                 //print out the number of unknown processors
    return 0;
}

//function:clean_up module
void cleanup_module(){
    printk(KERN_INFO"end printing!!!!\n");
}