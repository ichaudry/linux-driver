#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include "simple_char_driver.h"

#define DEVICE_NAME "simpleCharDriver"	

//Major name assigned to our device driver
static int Major;		

//Keep track if device file already open
volatile static int is_open= 0;

//Message device reads and writes
static char message[1024];

//Message size tracker
int num_bytes=0;

/*
*Device File Operations
*/
int myDevice_open (struct inode * inodep, struct file * filep);
int myDevice_close (struct inode * inodep, struct file * filep);
ssize_t myDevice_write (struct file * filep, const char __user * uInBuff, size_t nbytes, loff_t * offp);
ssize_t myDevice_read(struct file * filep, char __user * uOutBuff, size_t nbytes, loff_t * offp);
static long myDevice_ioctl(struct file *f, unsigned int cmd, unsigned long arg);


struct file_operations fops = {
	read: myDevice_read,
	write: myDevice_write,
	open: myDevice_open,
    unlocked_ioctl: myDevice_ioctl,
	release: myDevice_close
};


/*
*Called at module initialization
*/
static int __init myModule_start (void)
{
    printk("The %s function was invoked\n",__FUNCTION__);
    printk(KERN_INFO "My module has started. Hello world\n");

    //Test message
    strncpy(message, "Hello world.",1023);
    num_bytes= strlen(message);


    //Registering character driver. Letting kernel pick device number
    Major= register_chrdev(0,DEVICE_NAME ,&fops);

    //Catch errors registering device
	if (Major < 0) {
	  printk(KERN_ALERT "Registering char device failed with %d\n", Major);
	  return Major;
	}

    printk(KERN_INFO "The simple char device major number is :%d\n",Major);

    return 0;
}

/*
*Called at module exit
*/
static void __exit myModule_cleanup (void)
{
    /* 
	 * Unregister the device 
	 */
	unregister_chrdev(Major, DEVICE_NAME);
	
    printk("The %s function was invoked\n",__FUNCTION__);
    printk(KERN_INFO "My module says goodbye world\n");
    
}

/*
*Ioctl backdoor used to read message from device and get message size
*/
static long myDevice_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{

    printk("The %s function was invoked",__FUNCTION__);

    if(cmd == IOCTL_READ){
        //Copy message to user buffer
        if (copy_to_user((char *)arg, &message, num_bytes))
            {
                return -EACCES;
            }
    }
    else if(cmd == IOCTL_FILESIZE){
        //copy file size to user buffer
        if (copy_to_user((int *)arg, &num_bytes, sizeof(int)))
            {
                return -EACCES;
            }
    }
    else{
        return -EINVAL;
    }

    return 0;
}

/*
*Called when file is read
*/
ssize_t myDevice_read(struct file * filep, char __user * uOutBuff, size_t nbytes, loff_t * offp)
{
    //Track number of bytes already read
    int bytes_read=0;

    printk("The %s function was invoked\n",__FUNCTION__);


    //Catch null offp
    if(offp == NULL) return -EINVAL;

    //If already read to the end of file there is nothing to read
    if(*offp >= num_bytes) return 0;

    while((bytes_read < nbytes) && (*offp < num_bytes))
    {
        put_user(message[ *offp], &uOutBuff[bytes_read]);
        *offp= *offp + 1;
        bytes_read++;
    }

    return bytes_read;
}


/*
*Called when file is written to
*/
ssize_t myDevice_write (struct file * filep, const char __user * uInBuff, size_t nbytes, loff_t * offp)
{
    int bytes_write = 0;

    printk(KERN_INFO "This is the string recieved from the user: %s\n",uInBuff);
    
    printk("The %s function was invoked\n",__FUNCTION__);

    if(offp == NULL)  return -EINVAL;
    if(*offp >= 1023) return -EINVAL;

    while((bytes_write < nbytes) && (*offp < 1023)){
        get_user(message[*offp], &uInBuff[bytes_write]);
        *offp = *offp + 1;
        bytes_write++;
    }
    printk(KERN_INFO "(%s)\n",message);

    num_bytes=num_bytes + bytes_write;

    return bytes_write;
}


/*
*Is called when a process opens the device file
*/
int myDevice_open (struct inode * inodep, struct file * filep)
{
    printk("The %s function was invoked\n",__FUNCTION__);

    //Check if device open
    if(is_open){
        printk(KERN_INFO "Error - the device is already open\n");
        return -EBUSY;
    }

    is_open=1;
    try_module_get(THIS_MODULE);

    return 0;
}

/*
*Is called when a process closes the device file
*/
int myDevice_close (struct inode * inodep, struct file * filep)
{
    printk("The %s function was invoked\n",__FUNCTION__);
    //Check if device open
    if(!is_open){
        printk(KERN_INFO "Error - the device wasn't opened\n");
        return -EBUSY;
    }

    is_open=0;
    module_put(THIS_MODULE);
    
    return 0;
}


module_init(myModule_start);
module_exit(myModule_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ibraheem Chaudry");
MODULE_DESCRIPTION("Simple Character Driver");

