# Character Device Driver

## Introduction
In this assignment, we will explore device drivers in the Linux kernel, and how to implement a basic character device driver using a loadable kernel module. This assignment will involve:
- [Creating a device file](#creating-a-device-file) 
- [Implementation](#implementation) 
- [File operations](#file-operations) 
- [Installing and testing](#installing-and-testing) 

## Linux devices overview
In Linux, device I/O is accomplished by using files. For example, reading from and writing to a file will invoke the associated device driver to do that actual reading and writing. All device drivers have a major and minor number, where the major number is unique to every device driver. The minor number differentiates all devices belonging to a specific device driver.

For example, a typical system will have multiple partitions on a single hard disk. A single major number is used to specify to hard disk device driver, and each partition will have a different minor number. You can see this for yourself on your VM:
```
ls -l /dev/sda*
brw-rw---- 1 root disk 259, 0 Apr 27 12:01 /dev/sda
brw-rw---- 1 root disk 259, 1 Apr 27 12:01 /dev/sda1
brw-rw---- 1 root disk 259, 2 Apr 27 12:01 /dev/sda2
brw-rw---- 1 root disk 259, 3 Apr 27 12:01 /dev/sda3
```
Where this device driver has a major number of 259, and four devices (partitions) associated with it (0-3).

Note, that there are two types of device drivers:
1. A **character device driver** reads and writes from/to the device character by character. This is the most basic type of driver, and usually the simplest to implement. These types of drivers are most common for non-hardware applications (eg. a software interface like `stdout`).
2. A **block device driver** reads or writes large blocks of data with a single read/write operation. These drivers are more complex, but usually more efficient in their use of system resources. These drivers are most common for working with physical hardware devices like a network interface.

## Creating a device file
The first step in creating a device driver, is creating the corresponding device file that will serve as the interface to your device. This file will live in the `/dev` directory, and can be created with the following:
```
sudo mknod -m 777 /dev/simple_char_driver c 511 0
```
This will:
- Set the permission bits to `777`, meaning all users can read, write, and execute the file
- `simple_char_driver` is the name of the device file
- `c` specifies the type of driver, in this a character driver
- `511` is the major number of the driver that will be associated with this file
- `0` is the minor number of the device

If you're curious, refer to [`devices.txt` in the kernel documentation](https://www.kernel.org/doc/Documentation/admin-guide/devices.txt) to see a list of all the current device drivers and their associated major/minor numbers. **You must choose 511 to be the major number of your device driver for this assignment**.

## Device driver overview
A user space program will issue calls to `open()`, `read()`, `write()`, `seek()`, and `close()`. These calls, when interfacing with the device driver file you created, will call the corresponding functions provided by the device driver implementation.

For example running `echo "hello world" > /dev/simple_char_driver` will call the `open()` function you implement, the `write()` function, and the `close()` function. Running `cat /dev/simple_char_driver` will open, read, and close the file. You can also use the previous assignment's file tester program as a means of interacting with your device driver.

## Implementation
Along with the header files necessary for module programming, you will also need to include:
- `linux/fs.h` - contains functions used to work with files
- `linux/uaccess.h` - allows you to access data from the user space in kernel space, and vice versa

We have provided a skeleton of the functions you will be implementing [here](char_driver.c). Do note that many of these functions are empty and currently return 0, which may not be the appropriate return value for your implementation.

### Initialisation & Exit functions
Declare your module initialisation and exit functions that will be called by `module_init()` and `module_exit()`, as done in previous assignments.
- In your initialisation function, register your character device using the `register_chrdev()` function
- In your exit function, unregister the device using `unregister_chrdev()` function

**Hint**: A quick and direct way to find out how to use a function (or command line utility) in Linux, is searching the internet for "man <function name>", which will provide you with the manual page for that function, including the parameters the function takes, its return value, etc.

We will use a dynamically allocated kernel buffer (referred to by the macro `BUFFER_SIZE`) with a fixed size of **900 bytes** to store data written to the device. You should allocate this memory in the initialisation function, and free this memory prior to exiting. For this, we will need two memory management functions provided by the Linux kernel that are in the `linux/slab.h` header file:
- `void *kmalloc(size_t size, gfp_t flags)` allocates memory for use in the kernel. Use the macro `GFP_KERNEL` as the flags argument in this case
- `void kfree(const void *kptr)` frees memory previously allocated with `kmalloc()`

### File operations
To bind your device driver's file operation functions (open, close, read, etc.) to functions used internally by the kernel, you need to populate the [`file_operations`](https://elixir.bootlin.com/linux/v6.12.25/source/include/linux/fs.h#L2062) structure. Your device driver will only need `open()`, `close()`, `seek()`, `read()`, and `write()` operations, so only populate those fields. These are also the five functions that you will need to implement in your device driver. Note the "close" function is bound by the `release` function in the `file_operations` struct. Here is an example:
```
struct file_operations simple_char_driver_file_operations = {
    .owner = THIS_MODULE,
    .open = simple_char_driver_open,
    .release = simple_char_driver_close,
    .read = simple_char_driver_read,
    .write = simple_char_driver_write,
    .llseek = simple_char_driver_seek,
};
```
- **Open** - The open function takes two parameters; an [index node](https://www.kernel.org/doc/html/latest/filesystems/ext4/inodes.html) that represents the physical file on the disk, and a pointer to a `file` structure belonging to the kernel that represents the state of a file. This function returns an integer indicating success or failure. In this function, you don't need to do anything other than log the number of times the device has been opened.

- **Close/Release** - The close function takes the same two parameters as the open function, and again returns an integer indicating success or failure. This function only needs to log the number of times the device has been closed.

- **Read** - The read function takes four parameters; a file pointer, a pointer to a user space buffer, the size of that buffer, and a pointer to the current position in the file. Use appropriate methods to copy data from the device buffer, starting at the current position, to the user space buffer. If successful, update the current position in the file, and return the number of bytes read. Additionally, log the number of bytes read to the kernel log.

- **Write** - The write function takes the same four parameters as the read function. Copy data stored in the user space buffer into the device driver buffer. The write starts from the current position, and if successful, the position should be updated accordingly. Once again, log the number of bytes written to the kernel log, and return the number of bytes written. If the user sets the current position back to an earlier position in the file (ie. by seeking), previously written data should be overwritten.

- **Seek** - The seek function takes three parameters; a file pointer, an offset, and a whence value. The valid values of whence behave the same as the seek implementation in the file tester assignment: 0 (`SEEK_SET`) sets the position directly to the value of offset, 1 (`SEEK_CUR`) increments or decrements the position with the value of offset, and 2 (`SEEK_END`) sets the position offset bytes from the end of the file.

### Additional specifications
- If a user attempts to seek to a position that is before the beginning or beyond the end of device buffer, the seek function should set the current position to the beginning of the file or to the end of the file, respectively.
- If a user attempts to read or write some amount of data where the number of bytes would go beyond the end of the device buffer, your driver should only read or write until the end of the buffer.

## Installing and testing
Refer to previous assignment instructions for compiling and installing kernel modules. Verify it was loaded correctly by inspecting the kernel log for the message(s) printed by your initialisation function, check the output of `lsmod` for your module, etc.

To test the functionality of your implementation, use command line utilities such as `echo`, `cat`, `head`, `tail`, or the file tester program you made. Ensure you thoroughly test edge cases like seeking, reading, or writing before the beginning or beyond the end of the device buffer.

We have provided a pre-compiled LKM for you to use as a reference. It can be downloaded [here](sample_char_driver_6_12_65.ko) and loaded into your **6.12.65 kernel** (If you are using a different kernel version, we provide samples for a number of differnt versions in the repo). Note that this example driver **uses a major number of 510**, meaning an additional device driver file will need to be created. You are able to have this one loaded alongside the one you are implementing that has a major number of 511. Consider this the expected behaviour of your implementation.

## Submission requirements
- Your `char_driver.c` source code
- A 1 page report of things you learned, difficulties you encountered and how you overcame them, comments about the assignment, etc.
