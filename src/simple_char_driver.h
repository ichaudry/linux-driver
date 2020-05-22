//
// Created by ibraheem on 5/13/20.
//

#ifndef LINUX_DRIVER_SIMPLE_CHAR_DRIVER_H
#define LINUX_DRIVER_SIMPLE_CHAR_DRIVER_H
#include <linux/ioctl.h>

#define IOCTL_READ _IOR('q', 1, void *)
#define IOCTL_FILESIZE _IOR('q',2,void *)

/*
*User Methods
*/
//Read device file message using ioctl
void ioctl_read(int fd);

//Get size of device file message using ioctl
int ioctl_messageSize(int fd);

#endif //LINUX_DRIVER_SIMPLE_CHAR_DRIVER_H