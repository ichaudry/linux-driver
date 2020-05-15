#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#include "simple_read_cDriver.h"

void simple_read(int fd)
{
    ioctl_arg_t q;

    q.buffer= malloc(1024);

    if (ioctl(fd, IOCTL_SIMPLE_READ, &q) == -1)
    {
        perror("query_apps ioctl get");
    }
    else
    {
        printf("The read:\n%s", q.buffer);
    }

    free(q.buffer);
}

void simple_write(int fd)
{
    int v;
    ioctl_arg_t q;

    printf("Enter position: ");
    scanf("%d", &v);
    getchar();
    q.position = v;
    printf("Enter count: ");
    scanf("%d", &v);
    getchar();
    q.count = v;
    printf("Enter Ego: ");
    scanf("%d", &v);
    getchar();
    q.ego = v;

    if (ioctl(fd, IOCTL_SIMPLE_WRITE, &q) == -1)
    {
        perror("query_apps ioctl set");
    }
}

int main(int argc, char *argv[])
{
    char *file_name = "/dev/simpleRead";
    int fd;
    
    enum
    {
        s_read,
        s_write
    } ioctl_option;

   if (argc == 2)
    {
        if (strcmp(argv[1], "-r") == 0)
        {
            ioctl_option = s_read;
        }
        else if (strcmp(argv[1], "-w") == 0)
        {
            ioctl_option= s_write;
        } 
    }
    else
    {
        fprintf(stderr, "Usage: %s [-r | -w ]\n", argv[0]);
        return 1;
    }

    //Open file for read and write
    fd = open(file_name, O_RDWR);
    if (fd == -1)
    {
        perror("simple_read_app open");
        return 2;
    }

    switch (ioctl_option)
    {
        case s_read:
            simple_read(fd);
            break;
        case s_write:
            simple_write(fd);
            break;
        default:
            break;
    }

    close (fd);

    return 0;
}
