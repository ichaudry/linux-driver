//
// Created by ibraheem on 5/13/20.
//
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/uaccess.h>

#include "simple_read_cDriver.h"

#define FIRST_MINOR 0
#define MINOR_CNT 1


static dev_t dev;
static struct cdev c_dev;
static struct class *cl;
static int position = 1, count = 3;
char buffer[64] = "Hello from the driver";

static int my_open(struct inode *i, struct file *f);
static int my_close(struct inode *i, struct file *f);
ssize_t my_read(struct file *filep, char * buff, size_t count, loff_t *offp);
ssize_t my_write(struct file *filep, const char *buff, size_t count, loff_t *offp);
static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg);

static struct file_operations simple_read_fops =
        {
                .owner = THIS_MODULE,
                .open = my_open,
                .release = my_close,
                .unlocked_ioctl = my_ioctl,
                .read = my_read,
                .write = my_write,
        };

static int __init simple_read_cDriver_init(void)
{
    int ret;
    struct device *dev_ret;


    if ((ret = alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_CNT, "simple_read_cDriver")) < 0)
    {
        return ret;
    }

    cdev_init(&c_dev, &simple_read_fops);

    if ((ret = cdev_add(&c_dev, dev, MINOR_CNT)) < 0)
    {
        return ret;
    }

    if (IS_ERR(cl = class_create(THIS_MODULE, "char")))
    {
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(cl);
    }
    if (IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, "simpleRead")))
    {
        class_destroy(cl);
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(dev_ret);
    }

    return 0;
}

static void __exit simple_read_cDriver_exit(void)
{
    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);
}

static int my_open(struct inode *i, struct file *f)
{
    printk("The %s function was invoked",__FUNCTION__);
    return 0;
}
static int my_close(struct inode *i, struct file *f)
{
    printk("The %s function was invoked",__FUNCTION__);
    return 0;
}

ssize_t my_read(struct file *filep, char *buff, size_t count, loff_t *offp)
{
    printk("The %s function was invoked",__FUNCTION__);
    return 0;
}


ssize_t my_write(struct file *filep, const char *buff, size_t count, loff_t *offp)
{
    printk("The %s function was invoked",__FUNCTION__);
    copy_from_user(filep,buff,count);
    return 0;
}


static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
 
    ioctl_arg_t q;

    printk("The %s function was invoked",__FUNCTION__);

    switch (cmd)
    {
        case IOCTL_SIMPLE_READ:
            
            if (copy_to_user((char *)arg, buffer, 10))
            {
                return -EACCES;
            }
            break;

        default:
            return -EINVAL;
    }

    return 0;
}

module_init(simple_read_cDriver_init);
module_exit(simple_read_cDriver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ibraheem Chaudry");
MODULE_DESCRIPTION("Simple File Read Character Driver");
