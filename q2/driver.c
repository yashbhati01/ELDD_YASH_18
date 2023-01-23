#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>		//MAJOR() MINOR()
#include <linux/fs.h>
#include <linux/slab.h>                 //kmalloc()
#include <linux/uaccess.h>              //copy_to/from_user()
#include <linux/err.h>
#include<linux/ioctl.h>		 	//ioctl functionality
#include "ioctl.h"


#define mem_size        1024           //Memory Size
#define WR_DATA _IOW('a','a',int32_t *)
#define RD_DATA _IOR('a','b',int32_t *)

dev_t dev = 0;

int32_t kernel_buffer;

/*Function Prototypes*/

static int      __init driver2_init(void);
static void     __exit driver2_exit(void);
static int      file_open(struct inode *inode, struct file *file);
static int      file_release(struct inode *inode, struct file *file);
extern long file_ioctl(struct file *file, unsigned int cmd, unsigned long arg);


/*File Operations structure*/

static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .open           = file_open,
        .release        = file_release,
        .unlocked_ioctl = file_ioctl,
};
 
 
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
/*This function provies IOCTL functionality for the driver*/
extern long file_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	switch(cmd)
	{
		case WR_DATA:	if(copy_from_user(&kernel_buffer,(int32_t *)arg,sizeof(kernel_buffer)))
		               {
                                   pr_err("Data Write : Err!\n");
                               }
				printk("Writing ioctl data\n");
				break;
				
		case RD_DATA:	if(copy_to_user((int32_t *)arg,&kernel_buffer,sizeof(kernel_buffer)))
		                {
                                pr_err("Data Write : Err!\n");
                               }
				printk("Reading data\n");
				break;
		default :
			printk("DEFAULT\n");
			break;			
	}
	return 0;
}

/*Module Init function*/

static int __init driver2_init(void)
{		
        /*Allocating Major number*/
        if((alloc_chrdev_region(&dev, 0, 1, "driver2_Dev")) <0)
        {
                pr_info("Cannot allocate major number\n");
                return -1;
        }
        pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
        
         /*Creating Physical memory*/
        if((kernel_buffer = kmalloc(mem_size , GFP_KERNEL)) == 0)
        {
            pr_info("Cannot allocate memory in kernel\n");
            return -1;
        }
        
        strcpy(kernel_buffer, "Hello_World");
       
        pr_info("Device Driver Insert...Done!!!\n");
        return 0;
}

static void __exit driver2_exit(void)
{
	kfree(kernel_buffer);
        unregister_chrdev_region(dev, 1);
        pr_info("Device Driver Remove...Done!!!\n");
}
 
module_init(driver2_init);
module_exit(driver2_exit);
 
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LED OPERATION");
