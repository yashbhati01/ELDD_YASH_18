/*****************************************************Q(1) Kernel Synchronization Mechanism********************************************************/

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>		// MAJOR() MINOR()
#include <linux/fs.h>			//file operations structure
#include <linux/slab.h>                 //kmalloc()
#include <linux/uaccess.h>              //copy_to/from_user()
#include<linux/wait.h>
#include<linux/semaphore.h>


#define mem_size        1024           //Memory Size

dev_t dev = 0;
static struct class *file_class;
uint8_t *kernel_buffer;
struct semaphore my_sema;
static struct task_struct *wait_thread;                               
int wait_queue_flag = 0;
uint32_t read_count = 0;

DECLARE_WAIT_QUEUE_HEAD(wq_file);

/*Function Prototypes*/

static int      __init file_init(void);
static void     __exit file_exit(void);
static int      file_open(struct inode *inode, struct file *file);
static int      file_release(struct inode *inode, struct file *file);
static ssize_t  file_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  file_write(struct file *filp, const char *buf, size_t len, loff_t * off);


/*File Operations structure*/

struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = file_read,
        .write          = file_write,
        .open           = file_open,
        .release        = file_release,
};

static int wait_function(void *wf)
{
    while(1)
    {
        pr_info("waiting for event\n");
        wait_event_interruptible(wq_file,wait_queue_flag!=0);
        if(wait_queue_flag==2)
        {
           pr_info("Event came from read function %d\n",++read_count);
           wait_queue_flag = 0;
	   return 0;
        }
        if(wait_queue_flag==1)
        {
          pr_info("Event came from write function %d\n",++read_count);
          wait_queue_flag = 0;
          return 0;
        }
        if(wait_queue_flag==3)
	{
	  pr_info("Event came from exit function %d\n",++read_count);
          wait_queue_flag = 0;
          return 0;
	}
    }
    do_exit(0);

    return 0;
}

/*This function will be called when we open the Device file*/

static int file_open(struct inode *inode, struct file *file)
{
        pr_info("Device File Opened...!!!\n");
        return 0;
}

/*This function will be called when we close the Device file*/

static int file_release(struct inode *inode, struct file *file)
{
        pr_info("Device File Closed...!!!\n");
        return 0;
}

/*This function will be called when we read the Device file*/

static ssize_t file_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        
	wait_queue_flag = 1;
        wake_up_interruptible(&wq_file);
	up(&my_sema);
        if( copy_to_user(buf, kernel_buffer, mem_size) )
        {
                pr_err("Data Read : Err!\n");
        }
        pr_info("Data Read : Done!\n");
        return mem_size;
}

/*This function will be called when we write the Device file*/

static ssize_t file_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        
	wait_queue_flag = 3;
	wake_up_interruptible(&wq_file);
	down(&my_sema);
	if( copy_from_user(kernel_buffer, buf, len) )
        {
                pr_err("Data Write : Err!\n");
        }
        pr_info("Data Write : Done!\n");
        return len;
}

/*Module Init function*/

static int __init file_init(void)
{
        /*Allocating Major number*/
        if((alloc_chrdev_region(&dev, 0, 1, "file1_Dev")) <0)
	{
                printk("Cannot allocate major number\n");
                return -1;
        }
	printk("For device1\n");
        printk(KERN_INFO "Major = %d\nMinor = %d \n",MAJOR(dev), MINOR(dev));
	
	if((alloc_chrdev_region(&dev,0,1,"file2_dev"))<0)
	{
		printk("Cannot allocate major number\n");
		return -1;
	}
	printk("For device2\n");
	printk(KERN_INFO "Major = %d\nMinor = %d \n",MAJOR(dev), MINOR(dev));
        
	printk(KERN_INFO "Kernel Module Inserted Successfully....\n");
 	return 0;	


        /*Creating Physical memory*/
        if((kernel_buffer = kmalloc(mem_size , GFP_KERNEL)) == 0)
	{
            pr_info("Cannot allocate memory in kernel\n");
            return -1;
        }

        strcpy(kernel_buffer, "Hello_World");
	sema_init(&my_sema,1);
        pr_info("Device Driver Insert...Done!!!\n");
        return 0;

}

/*Module exit function*/

static void __exit file_exit(void)
{
	wait_queue_flag=2;
     	wake_up_interruptible(&wq_file);
	kfree(kernel_buffer);
        unregister_chrdev_region(dev, 1);
	unregister_chrdev_region(dev,1);
        pr_info("Device Driver Remove...Done!!!\n");
}

module_init(file_init);
module_exit(file_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YASH");
MODULE_DESCRIPTION("Synchronization");
