//
// Created by ibraheem on 5/13/20.
//

#ifndef LINUX_DRIVER_SIMPLE_READ_CDRIVER_H
#define LINUX_DRIVER_SIMPLE_READ_CDRIVER_H
#include <linux/ioctl.h>

typedef struct
{
    int status, dignity, ego;
} query_arg_t;

#define QUERY_GET_VARIABLES _IOR('q', 1, query_arg_t *)
#define QUERY_CLR_VARIABLES _IO('q', 2)
#define QUERY_SET_VARIABLES _IOW('q', 3, query_arg_t *)

#endif //LINUX_DRIVER_SIMPLE_READ_CDRIVER_H
