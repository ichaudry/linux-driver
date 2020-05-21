#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include "simple_char_driver.h"

void simple_read(int fd)
{

    char * uBuffer= malloc(1024);

    if (ioctl(fd, IOCTL_SIMPLE_READ, uBuffer) == -1)
    {
        perror("query_apps ioctl get");
    }
    else
    {
        printf("The following message is read from the device file: %s \n",uBuffer);
    }

    free(uBuffer);
}


int main(int argc, char *argv[])
{
    char *file_name = "/dev/myDevice";
    int fd;
    
    //Allow for more ioctl options in future e.g. write etc
    enum
    {
        s_read
    } ioctl_option;

   if (argc == 2)
    {
        if (strcmp(argv[1], "-r") == 0)
        {
            ioctl_option = s_read;
        }
        else 
        {
            fprintf(stderr, "Usage: %s [-r ]\n", argv[0]);
            return 1;
        }
    }
    else
    {
        fprintf(stderr, "Usage: %s [-r ]\n", argv[0]);
        return 1;
    }

    //Open file for read and write
    fd = open(file_name, O_RDWR);
    if (fd == -1)
    {
        perror("simple_read_app open");
        return 2;
    }

    //One case for read but more can be added later as enum ioctl args is expanded
    switch (ioctl_option)
    {
        case s_read:
            simple_read(fd);
            break;
        default:
            break;
    }

    close (fd);

    return 0;
}