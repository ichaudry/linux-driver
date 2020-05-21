//
// Created by ibraheem on 5/13/20.
//

#ifndef LINUX_DRIVER_SIMPLE_CHAR_DRIVER_H
#define LINUX_DRIVER_SIMPLE_CHAR_DRIVER_H
#include <linux/ioctl.h>

#define IOCTL_SIMPLE_READ _IOR('q', 1, ioctl_arg_t *)

#endif //LINUX_DRIVER_SIMPLE_CHAR_DRIVER_H