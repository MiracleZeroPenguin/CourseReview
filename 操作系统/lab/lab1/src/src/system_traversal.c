#include<linux/module.h>
#include<linux/kernel.h>
#include <linux/sched.h>
#include <linux/sched/task.h>
#include <linux/sched/signal.h>

int num_tot_proc=0;
int num_running_proc=0;
int num_interruptible_proc=0;
int num_uninterruptible_proc=0;
int num_stopped_proc=0;
int num_traced_proc=0;
int num_zombie_proc=0;
int num_dead_proc=0;
int num_unknown=0;

int init_module(){
    printk(KERN_INFO"Start!!!\n");
    struct task_struct *tmpTask;
    int tmpTask_state;
    int tmpTask_exitState;
    printk(KERN_INFO"Info of all processors\n");
    for(tmpTask=&init_task;(tmpTask=next_task(tmpTask))!=&init_task;){
        printk(KERN_INFO"Name:%s Pid:%d State:%ld ParName:%s\n",tmpTask->comm,tmpTask->pid,tmpTask->state,tmpTask->real_parent->comm);
        num_tot_proc++;
        tmpTask_state=tmpTask->state;
        tmpTask_exitState=tmpTask->exit_state;
        if(tmpTask_exitState!=0){//the process has exited
            switch (tmpTask_state)
            {
            case EXIT_ZOMBIE:num_zombie_proc++;break;
            case EXIT_DEAD:num_dead_proc++;break;          
            default:break;
            }
        }else{//the process has not exited
            switch (tmpTask_state)
            {
            case TASK_RUNNING:num_running_proc++;break;
            case TASK_INTERRUPTIBLE:num_interruptible_proc++;break;
            case TASK_UNINTERRUPTIBLE:num_uninterruptible_proc++;break;
            case TASK_STOPPED:num_stopped_proc++;break;
            case TASK_TRACED:num_traced_proc++;break;
            default:num_unknown++;break;
            }
        }
    }
    printk("*******************Statistic INFO*********************\n");
    printk("Total tasks:%d\n",num_tot_proc);
    printk("TASK_RUNNING:%d\n",num_running_proc);
    printk("TASK_INTERRUPTIBLE:%d\n",num_interruptible_proc);
    printk("TASK_UNINTERRUPTIBLE:%d\n",num_uninterruptible_proc);
    printk("TASK_STOP:%d\n",num_stopped_proc);
    printk("TASK_TRACED:%d\n",num_traced_proc);
    printk("EXIT_ZOMBIE:%d\n",num_zombie_proc);
    printk("EXIT_DEAD:%d\n",num_dead_proc);
    printk("UNKNOWN:%d\n",num_unknown);
    return 0;
}
void cleanup_module(){
    printk(KERN_INFO"end printing!!!!\n");
}