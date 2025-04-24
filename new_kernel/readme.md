# Compiling & installing a new Kernel

## Introduction
Welcome to the first programming assignment for CSCI 3753 - Operating Systems. In this assignment we will go over how to compile and install the Linux kernel. This assignment will be completed on your Cloud VM, and assumes you are able to access it already. Note that compiling the kernel can take quite some time (1+ hours). The steps needed to complete this assignment include:
- [Configuring GRUB](#configuring-grub)
- [Obtaining Linux source code](#obtaining-linux-source-code)
- [Compiling the kernel](#compiling-the-kernel)
- [Booting into the new kernel](#booting-the-new-kernel)

## Configuring GRUB
GRUB is the bootloader installed on your VM, and widely used across other Linux systems. The bootloader is a program that runs prior to the operating system itself, and is responsible for booting the actual OS. GRUB allows for booting from a list of different kernels available on the machine. By default, Ubuntu obfuscates this functionality, thus, we will need to update our GRUB configuration to allow for booting from alternate kernel versions. From the command line, begin by editing the GRUB configuration file:
```
sudo vi /etc/default/grub
```
You may use an editor other than Vim, such as Nano. Ensure the following lines are uncommented and present (a `#` at the start of a line indicates a comment):
```
GRUB_TIMEOUT_STYLE=menu
GRUB_SAVEDEFAULT=true
GRUB_DEFAULT=saved
GRUB_TIMEOUT=10
```
Save and close the editor. If using Vim, `:wq` to save, or `:q!` if you wish to discard your changes. Hint: searching the internet for "how to do X in Vim" will prove to be helpful.

These configurations, respectively, configure the menu to be displayed, set GRUB to remember your kernel choice when booting, and set the timeout (if you were to do nothing at this stage) to 10 seconds before booting.

Regenerate the GRUB configuration file that's used when booting (`/boot/grub/grub.cfg`), and reboot your machine:
```
sudo update-grub
sudo reboot
```

After rebooting your VM, verify that you see a boot menu similar to the below image (You might need to select the **Advanced options for Ubuntu** option to get this screen):
![grub menu](images/grub.png)

GRUB can be navigated with the arrow and enter keys. Once you've verified you can select a desired kernel to run, you can proceed to the next step.

## Obtaining Linux source code
Begin by creating a directory where the kernel sources will live:
```
sudo mkdir /home/kernel
sudo chmod 777 /home/kernel
```
Hint: consult the internet for how to perform certain tasks from the command line. This will be very helpful for the duration of this course.

Now, download the kernel sources (if Git is not installed, install it with `apt-get install git`):
```
cd /home/kernel
git clone --depth 1 https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git --branch linux-6.12.y linux-6.12.y
```
This will download a specified branch of the kernel sources from [kernel.org](https://kernel.org), and only include the most recent commit, which will be faster and use less disk space. If successful, you should see a new directory `linux-6.12.y` containing the kernel source code, and you can proceed to the next step.

## Compiling the kernel
The first step is installing required dependencies for kernel compilation:
```
sudo apt-get install make gcc libncurses-def flex bison build-essential bc libssl-dev libelf-dev dwarves zstd
```

Next, generate a configuration file that tells the kernel what options should be compiled. These configuration options include things like specific hardware support, networking configuration, and more.
```
cd /home/kernel/linux-6.12.y
make defconfig
make menuconfig
```
The `menuconfig` option will open an [ncurses](https://en.wikipedia.org/wiki/Ncurses) program that will allow you to set desired kernel parameters. These are written to the aforementioned configuration file.

![menuconfig 1](images/menu1.png)

Navigate this program with the arrow and enter keys:
1. Select **General setup**
2. Select **Local version - append to kernel release**
3. Enter `-rev1` in the box that appears. This will be added to the kernel name
4. Exit the program by selecting **\<Exit\>**, and answering **\<Yes\>** when prompted to save your changes

![menuconfig 2](images/menu2.png)

Now compile and install the kernel by running the following commands from the directory where your kernel sources live. Generally, compiling the kernel will take quite some time. Increasing the number supplied to the `-j#` parameter will utilise more CPU cores for compilation, however this can be bottlenecked by the amount of available RAM. Try initially with the number of CPUs you have (you can use the `nproc` command to determine this).
```shell
make -j#
sudo make modules_install # sudo is required here, as data is being installed to the system
sudo make install
```
If the compilation completed without errors, proceed to the next step. If you receive a non-explicit error, you may need to compile using fewer CPU cores.

## Booting the new kernel
If the compilation was successful, regenerate the GRUB configuration once again and reboot your VM.

When the GRUB menu appears, select the kernel that you compiled (with `-rev1` appended to it). After booting, you can confirm the new kernel is in use with:
```
uname -a
```
The expected output should show the new kernel version:
```
Linux csci3753-vm1-IdentiKey 6.12.y-rev1 #1 SMP PREEMPT_DYNAMIC Mon Dec 16 19:21:45 MST 2024 x86_64 GNU/Linux
```

## Submission requirements
- A screenshot of your GRUB menu showing the new kernel as an option
- A screenshot of the output of the `uname -a` command showing the new kernel version
- We will confirm your VM boots, **by default**, into the `-rev1` kernel. Thus, you must boot into the new kernel successfully, and ensure it automatically boots by default

### Hints
- If you need to start over, reboot your VM into the original `-generic` kernel, delete the `/home/kernel/x.y.z` directory and start from the beginning of this write-up.
- Not everything needs to ran with escalated privileges. Only use `sudo` unless necessary (ie. you receive an error saying something like "operation not permitted").
