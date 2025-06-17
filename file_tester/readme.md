# File Tester

## Introduction
In this assignment, you will write a user space program that takes commands from the user to **read, write, & seek** on a file. This program can subsequently be used to test the functionality of a device driver created in a future assignment.

## Implementation
Your program should accept a filename, passed from the command line, to open and work with:
```
$ ./filetest <filename>
```
If the file does not exist, or is unable to open, your program should print an error message and terminate with a **non-zero** exit status:
```
$ ./filetest bad_file.txt
filetest error: invalid filename
$ echo $?
255
```

Once successfully invoked, your interactive program should open the supplied file, and prompt the user with the string `Option (r for read, w for write, s for seek)`, and then accept the following input after the user presses the enter key:
| Option   | Functionality |
|----------|---------------|
| `r`      | Program should ask the number of bytes to read with the prompt: `Enter the number of bytes you want to read: `. Using [`malloc()`](https://man7.org/linux/man-pages/man3/free.3.html), allocate a buffer large enough, read the file starting from its current position, and print the returned data to the console (`stdout`). |
| `w`      | Program should ask the number of bytes to read with the prompt: `Enter the data you want to write: `. The user then enters the desired data, terminated by pressing the enter key. Your program should then write this data to the file. |
| `s`      | Program should ask for the values **offset** and **whence** with the prompts `Enter an offset value: ` and `Enter a value for whence: `. Your program should then set its position in the file according to the offset and whence (0 for `SEEK_SET`, 1 for `SEEK_CUR`, and 2 for `SEEK_END`). See the [`lseek()`](https://man7.org/linux/man-pages/man2/lseek.2.html) manual page for more info. |
| Ctrl + D | At any time, terminate the program, and return with a successful exit status. |
| Other    | If the user enters something other than listed above, ignore it and print the original prompt again. |

### Hints
- Your program should run in an infinite loop until it's explicitly terminated by the user
- While many sources on the internet may guide you to use the `scanf()` function to take in user input, do be aware of its [dangers](https://www.sekrit.de/webdocs/c/beginners-guide-away-from-scanf.html)
- Pay close attention to the write functionality and adhere to standard practices when working with C strings

We have included a pre-compiled binary to use as a reference for how your program should behave. Download it [here](https://github.com/cu-csci-3753-operating-systems/PAs/blob/main/file_tester/pa2test), and use `chmod` to make it executable.

## Testing
Suggested means of ensuring your program works correctly include `echo`, `cat`, `head`, `tail`, `hexdump`, and other command line utilities to ensure your program writes, reads, and seeks appropriately. Additionally, beware of using a text editors like [gedit](https://en.wikipedia.org/wiki/Gedit) or VSCode, as they can obfuscate certain characters.

```
$ echo "the quick brown fox jumps over the lazy dog" > file.txt
$ cat file.txt
the quick brown fox jumps over the lazy dog

$ head -c 10 file.txt
the quick
$ tail -c 10 file.txt
 lazy dog
$ hexdump -C file.txt
00000000  74 68 65 20 71 75 69 63  6b 20 62 72 6f 77 6e 20  |the quick brown |
00000010  66 6f 78 20 6a 75 6d 70  73 20 6f 76 65 72 20 74  |fox jumps over t|
00000020  68 65 20 6c 61 7a 79 20  64 6f 67 0a              |he lazy dog.|
0000002c
```

## Submission requirements
- Your `filetest.c` source code
- A 1 page report of things you learned, difficulties you encountered and how you overcame them, comments about the assignment, etc.
