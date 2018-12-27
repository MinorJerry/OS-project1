#include <linux/module.h> //define the module
#include <linux/kernel.h> //print the kernel info
#include <linux/sched/signal.h> // next_task() need this lib  
#include <linux/init.h>   //init_task
#include <linux/list.h>   //struct list_head

int init_module(void)
{
    int num_task = 0;                    //number of all task
    int num_task_running = 0;            //number of running task
    int num_task_interruptible = 0;      //number of interruptible task
    int num_task_uninterruptible = 0;    //number of uninterruptible task
    int num_task_zombie = 0;             //number of zombie task
    int num_task_stopped = 0;            //number of stoppend task
    int num_task_traced = 0;             //number of traced task
    int num_task_dead = 0;               //number of dead task
    int num_task_unknown = 0;            //number of unknown task
    int task_state;                      //the running state of the task
    int task_exit_state;                 //the exit state of the task
    struct task_struct *p = &init_task;  //the pointer to travel all the process
    //print the info that the module begin (kernel state)
    printk("MicroJerry@Linux-kernel module begins\n");
    //use for loop and pointer p to travel all the process 
    for (p = &init_task; (p = next_task(p)) != &init_task;)
    {
        //print the info of current process
        printk("MicroJerry@proInfo:Name:%s pid:%d State:%ld parentName:%s\n",
               p->comm, p->pid, p->state, p->real_parent->comm);
        //the toal num of task +1
        num_task++;
        //get the state of current process
        task_state = p->state;
        //get the state of current process
        task_exit_state = p->exit_state;
        //if the state is the exit state
        if (task_exit_state)
        {
            //use "switch case" to add the zombie and dead state 
            switch (task_exit_state)
            {
            case EXIT_ZOMBIE:     //if the state is EXIT_ZOMBIE
                num_task_zombie++;//the number of process in zombie state plus 1
                break;            //switch out
            case EXIT_DEAD:       //if the state is EXIT_DEAD
                num_task_dead++;  //the number of process in dead state plus 1
                break;            //switch out
            default:              //other case we don't care
                break;            //switch out     
            }
        }
        //if the state is the running state
        else
        {
            //use "switch case"
            switch (task_state)
            {
            case TASK_RUNNING:             //if the state is in TASK_RUNNING
                num_task_running++;        //the number of process in TASK_RUNNING state+1
                break;                     //switch out
            case TASK_INTERRUPTIBLE:       //if the state is in TASK_INTERRUPTIBLE
                num_task_interruptible++;  //the number of process in TASK_INTERRUPTIBLE state+1
                break;                     //switch out
            case TASK_UNINTERRUPTIBLE:     //if the state is in TASK_UNINTERRUPTIBLE
                num_task_uninterruptible++;//the number of process in TASK_UNINTERRUPTIBLE state+1
                break;                     //switch out
            case TASK_STOPPED:             //if the state is in TASK_STOPPED
                num_task_stopped++;        //the number of process in TASK_STOPPED state+1
                break;                     //switch out
            case TASK_TRACED:              //if the state is in TASK_TRACED
                num_task_traced++;         //the number of process in TASK_TRACED state+1
                break;                     //switch out
            default:                       //other states
                num_task_unknown++;        //the number of process in unknown state+1
                break;                     //switch out
            } 
        }
    }
    //print the info of Total number of the task
    printk("MicroJerry@Total number = %d\n", num_task);
    //print the info :the number of TASK_RUNNING task
    printk("MicroJerry@TASK_RUNNING: %d\n", num_task_running);
    //print the info :the number of TASK_INTERRUPTIBLE task
    printk("MicroJerry@TASK_INTERRUPTIBLE: %d\n", num_task_interruptible);
    //print the info :the number of TASK_UNINTERRUPTIBLE task
    printk("MicroJerry@TASK_UNINTERRUPTIBLE: %d\n", num_task_uninterruptible);
    //print the info :the number of TASK_STOPPED task
    printk("MicroJerry@TASK_STOPPED: %d\n", num_task_stopped);
    //print the info :the number of TASK_TRACED task
    printk("MicroJerry@TASK_TRACED: %d\n", num_task_traced);
    //print the info :the number of EXIT_ZOMBIE task
    printk("MicroJerry@EXIT_ZOMBIE: %d\n", num_task_zombie);
    //print the info :the number of EXIT_DEAD task
    printk("MicroJerry@EXIT_DEAD: %d\n", num_task_dead);
    //print the info :the number of TASK_UNKNOWN task
    printk("MicroJerry@TASK_UNKNOWN: %d\n", num_task_unknown);
    //print the info :the end of our print information
    printk("MicroJerry@INFO: ENDOUTPUT\n");
    return 0;
}

void cleanup_module(void)
{
    ////print the info :MODULE CLEANUP
    printk("MicroJerry@MODULE CLEANUP\n");
}
MODULE_LICENSE("GPL");//The code follows the GUN General Public license