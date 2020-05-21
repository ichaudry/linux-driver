//
// Created by ibraheem on 5/13/20.
//

#ifndef LINUX_DRIVER_SIMPLE_CHAR_DRIVER_H
#define LINUX_DRIVER_SIMPLE_CHAR_DRIVER_H
#include <linux/ioctl.h>

#define IOCTL_READ _IOR('q', 1, void *)
#define IOCTL_FILESIZE _IOR('q',2,void *)


#define DEVICE_NAME "simpleCharDriver"	


//Major name assigned to our device driver
static int Major;		

//Keep track if device file already open
volatile static int is_open= 0;

static char message[1024];
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
*User Methods
*/
//Read device file message using ioctl
void ioctl_read(int fd);

//Get size of device file message using ioctl
int ioctl_messageSize(int fd);


#endif //LINUX_DRIVER_SIMPLE_CHAR_DRIVER_H