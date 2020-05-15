//
// Created by ibraheem on 5/13/20.
//

#ifndef LINUX_DRIVER_SIMPLE_READ_CDRIVER_H
#define LINUX_DRIVER_SIMPLE_READ_CDRIVER_H
#include <linux/ioctl.h>

typedef struct
{
    int position;
    int count;
    char * buffer;
} ioctl_arg_t;

#define IOCTL_SIMPLE_READ _IOR('q', 1, ioctl_arg_t *)
// #define QUERY_CLR_VARIABLES _IO('q', 2)
#define IOCTL_SIMPLE_WRITE _IOW('q', 3, ioctl_arg_t *)

#endif //LINUX_DRIVER_SIMPLE_READ_CDRIVER_H
