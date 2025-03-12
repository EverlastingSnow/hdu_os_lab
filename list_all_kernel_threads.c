#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/sched/signal.h> //for each process

static void print_kernel_threads(void){
    struct task_struct *task;

    pr_info("Listing all kernel threads:\n");
    pr_info("Name\t\tPID\tState\tPrio\tPPID\n");

    for_each_process(task) {
        //内核线程使用内核空间地址不需要用户空间地址
        if (task->mm == NULL) {
            pr_info("%s\t%d\t%ld\t%d\t%d\n",
                   task->comm, task->pid, task->state,
                   task->prio, task->parent->pid);
        }
    }
}
static int __init list_all_kernel_threads_init(void){
    print_kernel_threads();
    return 0;
}

static void __exit list_all_kernel_threads_exit(void){
    pr_info("Unload list_all_kernel_threads module\n");
}

module_init(list_all_kernel_threads_init);
module_exit(list_all_kernel_threads_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("snow");
MODULE_DESCRIPTION("List all kernel threads");
