#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include "simple_char_driver.h"

char uReadBuffer[1024];

void ioctl_read(int fd)
{

    if (ioctl(fd, IOCTL_READ, uReadBuffer) == -1)
    {
        perror("query_apps ioctl get");
    }
    else
    {
        printf("The following message is read from the device file using IOCTL:\n%s\n",uReadBuffer);
    }
}


int main(int argc, char *argv[])
{
    char *file_name = "/dev/myDevice";
    int fd;

    //Open file for read and write
    fd = open(file_name, O_RDWR);
    if (fd == -1)
    {
        perror("myDevice open");
        return 2;
    }

printf("Command Options:\nread: reads the message from the device\nwrite: writes message to the device\nioctl: using ioctl backdoor to commnicate with device and read message\n\n");

    //Start command line utility to interact with the device file
    while(1){
        
        printf("\n>>>");

        //Get stdin
        char * inputLine = NULL; 
        size_t length = 0;

        if(getline(&inputLine, &length,stdin)==-1){
            perror("Error getting line from stdin\n");
            exit(EXIT_FAILURE);
        }

        if(!strcasecmp(inputLine,"exit\n")){
            printf("Exiting the program\n");
            free(inputLine);
            break;
        }

        if(!strcasecmp(inputLine,"read\n")){
            if(read(fd,uReadBuffer,50)<0){
                perror("read: ");
            }
            printf("The following message is read from the device file using read:\n%s\n",uReadBuffer);
            free(inputLine);
            continue;
        }

        if(!strcasecmp(inputLine, "write\n")){
            printf("Writing to the device file\n");
            char * uWriteBuffer;
            size_t uWriteBufferLength;

            //Get data to write to device file
            getline(&uWriteBuffer, &uWriteBufferLength,stdin);

            if (write(fd, uWriteBuffer, uWriteBufferLength) < 0) {
                perror("write: ");
            }
            
            free(uWriteBuffer);
            free(inputLine);
            continue;
        }

        if(!strcasecmp(inputLine,"ioctl\n")){
            printf("Using iotcl to do the operation for read\n");
            ioctl_read(fd);
            free(inputLine);
            continue;
        }


        printf("Command not understood\n");
    
        free(inputLine);
    }

    close (fd);

    return 0;
}