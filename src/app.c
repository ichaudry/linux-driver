#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include "simple_char_driver.h"

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

        /*
        * Exit program
        */
        if(!strcasecmp(inputLine,"exit\n")){
            printf("Exiting the program\n");
            free(inputLine);
            break;
        }

        /*
        * Read from message from device file
        */
        if(!strcasecmp(inputLine,"read\n")){
            //Read buffer
            char uReadBuffer[1024];
           
            //Get the number of bytes to read from device file using ioctl
            int nBytes = ioctl_messageSize(fd);

            //Check for errors 
            if(pread(fd,uReadBuffer,nBytes,0)<0){
                perror("read: ");
            }

            //Read to stdout
            printf("The following message is read from the device file using read:\n%s\n",uReadBuffer);
            
            free(inputLine);
            continue;
        }

        /*
        * Write message to device file
        */
        if(!strcasecmp(inputLine, "write\n")){
            printf("Enter message to send to device file:\n");

            //Write Buffer
            char uWriteBuffer [128];

            //Get message from user
            fgets(uWriteBuffer,128,stdin);

            //Set null terminator
            uWriteBuffer[strlen(uWriteBuffer) -1] = '\0';

            size_t uWriteBufferLength= strlen(uWriteBuffer);

            //catch errors        
            if (write(fd, uWriteBuffer, uWriteBufferLength) < 0) {
                perror("write: ");
            }
            
            free(inputLine);
            continue;
        }

        /*
        * Read message using ioctl backdoor
        */
        if(!strcasecmp(inputLine,"ioctl\n")){
            //Calling ioctl function for reading
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

/*
*Using ioctl backdoor to read message from the device file
*/
void ioctl_read(int fd)
{
    char uIoctlBuffer[1024];

    if (ioctl(fd, IOCTL_READ, uIoctlBuffer) == -1)
    {
        perror("query_apps ioctl get");
    }
    else
    {
        printf("The following message is read from the device file using IOCTL:\n%s\n",uIoctlBuffer);
    }
}

/*
*Using ioctl backdoor to get message size from the device
*/
int ioctl_messageSize(int fd)
{
    int messageSze;

    if (ioctl(fd, IOCTL_FILESIZE, &messageSze) == -1)
    {
        perror("query_apps ioctl get");
        return 0;
    }
    else
    {
        return messageSze;
    }
}
