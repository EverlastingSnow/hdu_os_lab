#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

//模块参数
static void print_family(struct task_struct *task){
    struct task_struct *parent, *sibling, *child;

    pr_info("List family information for PID %d (%s):\n", task->pid, task->comm);

    parent = task->parent;
    pr_info("Parent: %s (PID: %d, State: %ld)\n",
           parent->comm, parent->pid, parent->state);

    pr_info("Siblings:\n");
    list_for_each_entry(sibling, &parent->children, sibling) {
        if (sibling != task) {
            pr_info("  %s (PID: %d, State: %ld)\n",
                   sibling->comm, sibling->pid, sibling->state);
        }
    }

    pr_info("Children:\n");
    list_for_each_entry(child, &task->children, sibling) {
        pr_info("  %s (PID: %d, State: %ld)\n",
               child->comm, child->pid, child->state);
    }
}

static int target_pid = -1;
module_param(target_pid, int, 0);
MODULE_PARM_DESC(target_pid, "Target process id");
static int __init list_thread_family_init(void){
    struct task_struct *task;

    if (target_pid < 0){
        pr_err("Invalid target pid\n");
        return -EINVAL;
        //invalid value
    }

    task = pid_task(find_vpid(target_pid), PIDTYPE_PID);

    if (!task) {
        pr_err("Invalid target pid %d\n", target_pid);
        return -EINVAL;
    }

    print_family(task);
    return 0;
}
static void __exit list_thread_family_exit(void){
    pr_info("Unload module list_thread_family");
}

module_init(list_thread_family_init);
module_exit(list_thread_family_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("snow");
MODULE_DESCRIPTION("List thread family");
