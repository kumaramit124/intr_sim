/*
 * sim_interrupt.c -- the module increment the counter every defined time
 *
 * Copyright (C) 2012 -2019 Amit Kumar
 *
 * The source code in this file can be freely used, adapted,
 * and redistributed in source or binary form, so long as an
 * acknowledgment appears in derived source files.  The citation
 * should list that the code comes from the book "Linux Device
 * Drivers" by Alessandro Rubini and Jonathan Corbet, published
 * by O'Reilly & Associates.   No warranty is attached;
 * we cannot take responsibility for errors or fitness for use.
 *
 * $Id: sim_interrupt.c,v 1.0 2015/05/25 14:25:43 Amit K Exp $
 */

#include <linux/module.h>
#include <linux/init.h>

#include <linux/sched.h>  /* current and everything */
#include <linux/kernel.h> /* printk() */
#include <linux/fs.h>     /* everything... */
#include <linux/types.h>  /* size_t */
#include <linux/wait.h>
#include <linux/timer.h>
#include <asm/uaccess.h>

#include "sim_intr.h"

MODULE_LICENSE("Dual BSD/GPL");
#define MAXCOUNT 100000

#define GET_SIM_INTR_INTERVAL_JIFFIES    (HZ / 5)

static int sim_intr_major = 0;
static unsigned int frm_run_count;
static void     sim_intr_avg_timer_hdlr(unsigned long);

ssize_t sim_intr_read(struct file *, char *, size_t, loff_t *);

DEFINE_TIMER(sim_intr_avg_timer, sim_intr_avg_timer_hdlr, 0, 0);

static DECLARE_WAIT_QUEUE_HEAD(wq);
static int flag = 0;

static void sim_intr_avg_timer_hdlr(unsigned long arg)
{
        printk(KERN_DEBUG "process %i (%s) awakening the readers...\n",
                        current->pid, current->comm);
        flag = 1;
        wake_up_interruptible(&wq);

	frm_run_count +=25;
	if(frm_run_count > MAXCOUNT)
	{
		frm_run_count =0;
	}
	
}

ssize_t
sim_intr_read(
    struct file     *filp,
    char            *buffer,
    size_t          length,
    loff_t          *offset)
{
	printk ("In sim_intr_read!!!!!!\n");
	intfrm_info interrupt_info;

        sim_intr_avg_timer.expires = jiffies + GET_SIM_INTR_INTERVAL_JIFFIES;
        add_timer(&sim_intr_avg_timer);
        interrupt_info.intfrm_cpld_info = 0;

	interrupt_info.intfrm_run_count = frm_run_count;

	if (copy_to_user(buffer, &interrupt_info, sizeof(intfrm_info)) != 0)
	{
		return -EFAULT;
	}
        printk(KERN_DEBUG "process %i (%s) going to sleep\n",
                        current->pid, current->comm);
        wait_event_interruptible(wq, flag != 0);
        flag = 0;
        printk(KERN_DEBUG "awoken %i (%s)\n", current->pid, current->comm);
	del_timer_sync(&sim_intr_avg_timer);
	return 0;
}

struct file_operations sim_intr_fops = {
	.owner = THIS_MODULE,
	.read  = sim_intr_read,
};


int sim_intr_init(void)
{
	int result;

	/*
	 * Register your major, and accept a dynamic number
	 */
	result = register_chrdev(sim_intr_major, "sim_intr", &sim_intr_fops);
	if (result < 0)
		return result;
	if (sim_intr_major == 0)
		sim_intr_major = result; /* dynamic */

       // sim_intr_avg_timer.expires = jiffies + GET_SIM_INTR_INTERVAL_JIFFIES;
        //add_timer(&sim_intr_avg_timer);

	//frm_run_count =0;
	frm_run_count =112;

	return 0;
}

void sim_intr_cleanup(void)
{
        /* del_timer_sync is safe for SMP */
        del_timer_sync(&sim_intr_avg_timer);
	unregister_chrdev(sim_intr_major, "sim_intr");
}

module_init(sim_intr_init);
module_exit(sim_intr_cleanup);

