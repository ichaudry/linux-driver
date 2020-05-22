# linux-driver
Loadable Linux Character device driver with support for open, close, read, write and ioctl file operations

#### Steps to load and interact with device driver:
- Clone the repository
```
git clone <repistory-url>
```
- Open a terminal in the cloned directory and run the following commands. (Its strongly recommended you use a virtual image of any linux based operating system as inserting modules into kernel can result in system crashes)
    - Compile the module and generate a .ko file 
    ```sh
    $ cd src
    $ make
    ```
    - Insert the module into the kernel
    ```sh
    $ sudo insmod simple_char_driver.ko
    ```
    - Get <MAJORNUMBER> assigned of loaded module
    ```sh
    $ cat /proc/devices | grep simpleCharDriver
    ```
     - Create device file associated with the module inserted into the kernel
    ```sh
    $ sudo mknod /dev/myDevice c <MAJORNUMBER> 0
    ```
     - Change permissions of device file to allow user application read/write
    ```sh
    $ sudo chmod 666 /dev/myDevice
    ```
   - Compile the app.c file
    ```sh
    $ make app
    ```
   - Run app
    ```sh
    $ ./app
    ```
    
#### App Commands to interact with device driver
- write
    This takes a message from the user and write it to the device file
- read
    This read message from the device file
- ioctl
    This uses the ioctl backdoor to read message from the device file
instead of using the read file operation
