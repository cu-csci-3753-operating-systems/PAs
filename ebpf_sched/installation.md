# I. Installation Guidelines

`sched_ext` is a Linux kernel feature that implements an extensible scheduler class in BPF and dynamically loads custom process schedulers from the user-space. This kernel feature comes loaded within the latest Linux kernel version. In this programming assignment, we shall attempt to get a basic custom scheduler implemented over `sched_ext` and have it execute within the Linux Kernel. This programming assignment is going to be very similar to `PA0`, in which you compiled a kernel from its source-code and installed it. However, due to the fact that the BPF technology is still under development, and the `sched_ext` framework has not yet been introduced into the linux-kernel mainline, we will have to jump over a few hoops to get our operating system configured to support custom user-space process schedulers.

The programming assignment shall consist of the following steps:

1. Configuring the guest-OS and installing dependencies
2. Getting the Linux Kernel source-code, configuring it for `sched_ext` support , compiling it and installing it
3. Installing `sched_ext/scx` into the new linux kernel
4. Executing a custom user-space process scheduler ( `minimal_scheduler` )

At the end of the assignment, we shall ask you to submit your VM configuration files, similar to PA0. You shall be evaluated on the basis of whether you can get the custom user-space scheduler algorithms within the `minimal_scheduler` executing on your VM. We shall be providing some hints on how you may explore the development of your own custom schedulers, but that shall be an un-graded exercise.

**Note that all subsequent steps have been tested and validated only on kernel version 6.8.0-57-generic #59~22.04.1-Ubuntu. Their applicability to other baseline systems is unknown. You are welcome to help test.**

## I.A. Configuring the guest-OS for compilation

Before you start the following configuration steps, ensure that your VM has about 40GB or more of available disk space. This is crucial, as you might run into compilation errors due to your machine running out of disk space.

### I.A.1 Installing dependencies

We start off with completing the installation of the following dependencies :
```
$ sudo apt-get install curl tilix xterm
$ sudo apt-get install linux-tools-generic linux-cloud-tools-generic
$ sudo apt-get install linux-tools-$(uname -r) linux-cloud-tools-$(uname -r)
$ sudo apt-get install -y build-essential libelf-dev libcap2 libcap-dev binutils-dev
$ sudo apt-get install -y make gcc libncurses-dev flex bison build-essential bc libssl-dev libelf-dev dwarves zstd jq

# install python3 tools
$ sudo apt install -y python3 python3-pip python3-setuptools git ninja-build
```


Note that, we have installed `Tilix` and `xTerm` as alternatives to `gnome-terminal`. You may use these applications, in the scenario that your gnome-terminal fails to work, which might occur during the installation process. If you would like to re-install gnome-terminal, you would need to ensure that you have Python3.10 as your default Python3 version and use the following commands:

```
$ sudo apt-get install -y tilix xterm
$ sudo apt update
$ sudo apt install --reinstall gnome-terminal
```

### I.A.2 Install clang-18

```
$ cd $HOME/Downloads
$ wget https://apt.llvm.org/llvm.sh
$ chmod u+x llvm.sh
$ sudo ./llvm.sh 18
```

Configure your system to use the newly installed versions of software. You may find the different versions of clang installed on your system by using
```
$ ls /usr/bin/clang*
```

We shall be configuring our system to utilize `clang-18` and `llvm-18` as follows:
```
$ sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-18 100
$ sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-18 100


$ sudo update-alternatives --install /usr/bin/llvm-config llvm-config /usr/bin/llvm-config-18 100
$ sudo update-alternatives --install /usr/bin/llvm-ar llvm-ar /usr/bin/llvm-ar-18 100

$ sudo update-alternatives --install /usr/bin/strip strip /usr/bin/llvm-strip-18 100
$ sudo update-alternatives --install /usr/bin/llvm-strip llvm-strip /usr/bin/llvm-strip-18 100

$ sudo ln -s /usr/bin/ld.lld-18 /usr/bin/ld.lld
```

### Verify the versions

```
# Ensure the following are all configured to utilize `v18`
$ clang --version
$ clang++ --version
$ llvm-config --version
$ llvm-ar --version
$ llvm-strip --version
$ ld.lld --version
```

### I.A.3 Install rust via rustup

```
$ cd $HOME/Downloads

# install rustup
$ curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
$ . $HOME/.cargo/env

# verify the versions
$ cargo --version # ensure : v1.86.0
$ rustc --version # ensure : v1.86.0
```

### I.A.4 Install Meson

```
$ cd $HOME/Downloads
$ git clone https://github.com/mesonbuild/meson.git
$ cd meson
$ pip3 install .

# Add path of meson!
$ echo 'export PATH=$HOME/.local/bin:$PATH' >> ~/.bashrc
$ source ~/.bashrc
```

### I.A.5 Install Python3.11
```
$ sudo apt install software-properties-common

$ sudo add-apt-repository ppa:deadsnakes/ppa
$ sudo apt update

$ sudo apt install python3.11
$ sudo apt install python3.11-venv python3.11-distutils
$ curl -sS https://bootstrap.pypa.io/get-pip.py | sudo python3.11

# verify the installed version
$ python3.11 --version
$ pip3.11 --version
```

Configure the package manager to select between the different installed versions of Python3.

```
$ sudo update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.11 100 # set 3.11 as default
$ sudo update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.10  1
```

As we proceed with the installations, there will be instances when you might get a python-error saying that `apt_pkg` is missing. In those instances, you would be advised to use `Python3.10` as your current version of Python3. But, in all other instances, ensure that you revert to using `Python3.11`.

```
# Manually configure which Python3 version to use
$ sudo update-alternatives --config python3

# verify the current active version
$ python3 --version
$ pip3 --version
```

### I.A.6. Install `libbpf`

```
# Configure Python3 to use Python3.11
$ sudo update-alternatives --config python3

# libbpf dependencies
$ sudo apt install libelf-dev zlib1g-dev libzstd-dev pkg-config


# Download and build `libbpf` from source
$ cd $HOME/Downloads
$ git clone https://github.com/libbpf/libbpf.git
$ cd libbpf/src
$ make

# Install `libbpf`
$ sudo make install
$ sudo ldconfig
```

Manually verify the version that is installed. We would expect this to be `v1.6`.

```
# find the header files
$ sudo find / -name "libbpf_version.h"

# Locate files::
# /usr/include/bpf/libbpf_version.h ; or
# `/usr/local/include/bpf/libbpf_version.h`
# $HOME/Downloads/libbpf/src/libbpf_version.h

# Read the following file to determine the version
$ cat /usr/include/bpf/libbpf_version.h
```

Ensure that pkg-config is configured to correctly identify the version of libbpf ( `libbpf v1.6` )

```
# Find the location of the `libbpf.pc` file
$ sudo find /usr -name "libbpf.pc"    # Assume : <path_name>/pkgconfig/libbpf.pc


# Set the environment-variable 
$ export PKG_CONFIG_PATH=<path_name>/pkgconfig

# verify the configuration
$ pkg-config --modversion libbpf  # Ensure v1.6+
$ pkg-config --cflags --libs libbpf
```

### I.A.7. Configuring GRUB

GRUB is the boot loader installed with Ubuntu 22.04. It provides configuration options to boot from a list of different kernels available on the machine. By default Ubuntu 22.04 hides much of the boot process from users.  As a result, we will need to update our GRUB configuration to allow booting from multiple kernel versions and to recover from a corrupt installation. Perform the following: 

From the command line, edit the GRUB configuration file.  Feel free to use an editor other than vim.
```
$ sudo vi /etc/default/grub
```
Make sure the following lines are set near the beginning of the configuration file (a # at the start of a line indicates a comment):
```
GRUB_SAVEDEFAULT=true
GRUB_DEFAULT=saved
GRUB_TIMEOUT_STYLE=menu
GRUB_TIMEOUT=5
GRUB_RECORDFAIL_TIMEOUT=10
GRUB_DISTRIBUTOR=`lsb_release -i -s 2> /dev/null || echo Debian`
GRUB_CMDLINE_LINUX_DEFAULT="nosplash ipv6.disable=1"
GRUB_CMDLINE_LINUX=""
```

Exit the editor. If using vim, use `:wq!` to save, or  `:q!` if you want to discard your updates.

From the command line, update GRUB and then reboot:
```
$ sudo update-grub
$ sudo reboot now
```
After rebooting your virtual machine, verify that you see a boot menu similar to Figure 1 in [`PA0`](https://github.com/cu-csci-3753-operating-systems/PAs/tree/main/new_kernel). Once you've verified that you can run alternate versions of the kernel through grub, you're ready to move on to the next step.

## I.B. Building the Linux Kernel from Source

### I.B.1. Obtaining the Linux kernel source-code

We shall be following the instructions provided in PA0. We shall be proceeding with installing the linux kernel from the [Linux Kernel Archives website](https://www.kernel.org/).

First, you have to create a directory where you'll do the actual recompilation.   To do so, execute the following commands in a terminal window:

```
$ sudo mkdir /home/kernel
$ sudo chmod 777 /home/kernel 
```
Before we can download the source files for the kernel, we need to enable the appropriate Ubuntu repositories:
```
$ sudo vi /etc/apt/sources.list
```
Make sure the lines matching the following are NOT commented out:
```
deb http://us.archive.ubuntu.com/ubuntu jammy main restricted
deb http://us.archive.ubuntu.com/ubuntu jammy-updates main restricted
```
Once the source code repositories are enabled, run the following to retrieve the tarball for the [stable kernel version 6.14.2](https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.14.2.tar.xz) into your compilation directory,

```
$ cd /home/kernel
$ sudo apt-get update
```

and extract it using :
```
$ wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.14.2.tar.xz
$ tar -xf ./linux-6.14.2.tar.xz
$ cd linux-6.14.2
```

If you've successfully downloaded and extracted the new sources to `/home/kernel` you should see a new directory, `/home/kernel/linux-6.14.2` populated with kernel source code.

### I.B.2. Installing the `bpftool`

You may verify whether your OS already has an installed version of `bpftool` by attempting to identify the current installed version:

```
$ bpftool --version
```
We would like to proceed with building the tool from source to obtain the latest version ( `v7.6.0+` ) that's going to be compatible with our target OS. We shall proceed to install bpftool as follows:

```
$ sudo apt install -y build-essential libelf-dev
$ sudo apt-get install linux-tools-generic linux-cloud-tools-generic
$ sudo apt-get install linux-tools-$(uname -r) linux-cloud-tools-$(uname -r)
```

We need to ensure that we are using `clang-18` as we proceed with the following steps. First navigate to the `/tools/bpf/bpftool` folder within the kernel source-code.

```
# [<path/to/linux-source-dir>] = /home/kernel/linux-6.14.2
$ cd [<path/to/linux-source-dir>]/tools/bpf/bpftool

$ sudo make LLVM=1 CLANG=clang-18 LLC=llc-18
$ sudo make install
```

We can verify the installation by checking the version of `bpftool` that's installed:
```
$ bpftool --version # v7.6.0
```

### I.B.3 Compiling the Linux Kernel and installing

In this sub-section, we shall follow the instructions provided in the section on [Compiling the Kernel for PA0](https://github.com/cu-csci-3753-operating-systems/PAs/blob/main/new_kernel/readme.md#compiling-the-kernel).

Once you have the Linux source code, you can compile the kernel. The next step is to create the config file for your kernel build. Make sure you run this command in your recently created kernel source directory:
```
$ cd /home/kernel/linux-6.14.2
$ sudo cp /boot/config-$(uname -r) .config
```

The command `uname –r` gives you the current version of the kernel you are using.  This ensures you are compiling the kernel source for the same version of the kernel you're currently running on. The command `uname –a` will give you the system architecture information (for example if your OS is 32 or 64 bit).  Proceed with the instructions provided in configuring the kernel build by using the `menuconfig utility`

```
$ sudo make menuconfig
```

One last thing before we compile the kernel, we need to remove the kernel signature checking in .config. It will cause a compilation error later on, and we don't need it.
```
$ cd /home/kernel/linux-6.14.2
$ sudo vim .config
```

By default, you will see 2 entries as below.
```
CONFIG_SYSTEM_TRUSTED_KEYS="debian/canonical-certs.pem"
CONFIG_SYSTEM_REVOCATION_KEYS="debian/canonical-revoked-certs.pem"
```

You need to change them, and save the file.
```
CONFIG_SYSTEM_TRUSTED_KEYS=""
CONFIG_SYSTEM_REVOCATION_KEYS=""
```

### I.B.4 Configure the Linux Kernel to be `sched-ext` compatible

Additionally, we need to ensure that the following configuration options are enabled to ensure that the Linux Kernel can use [`sched_ext`](https://github.com/sched-ext/scx):
```
CONFIG_BPF=y
CONFIG_SCHED_CLASS_EXT=y
CONFIG_BPF_SYSCALL=y
CONFIG_BPF_JIT=y
CONFIG_DEBUG_INFO_BTF=y
CONFIG_BPF_JIT_ALWAYS_ON=y
CONFIG_BPF_JIT_DEFAULT_ON=y
CONFIG_PAHOLE_HAS_SPLIT_BTF=y
CONFIG_PAHOLE_HAS_BTF_TAG=y
```

With that, we are ready to compile, build and reboot our flashy new Linux Kernel with the power to utilize the [`Extensible Scheduler Class`](https://www.kernel.org/doc/html/next/scheduler/sched-ext.html).

### I.B.5 Compile, Build and Reboot

Now compile and install the kernel by running the following commands. Typically it takes a long time to compile the kernel the first time. Increasing the `-j` parameter can sometimes speedup the compilation, but it's best not to go more than 2 times the number of CPUs allocated to your VM. You can tell how many CPUs you have with either the `nproc` or `lscpu` commands. Noted that during the compilation, there may be some questions about configuration reloading, and compilation log. You can answer yes, you don't need to pay attention to those.

```
$ cd /home/kernel/linux-6.14.2
$ sudo make -j 8
$ sudo make modules_install
$ sudo make install
```

Once you've compiled and installed your new kernel, reboot your VM:
```
$ sudo reboot now 
```

When the GRUB option menu appears, select the version with your new kernel name appended ( `6.14.2-rev1` ). Assuming you correctly modified `/etc/default/grub`, the kernel you selected should now start by default each time you reboot your VM.

After booting into the new kernel, you can confirm again by issuing the following command in the terminal.
```
$ uname -a
```

The expected output should indicate the new kernel version ( `6.14.2-rev1`).

## I.C. Configuring OS with `sched_ext/scx`

### I.C.1. Download source-code for `sched_ext/scx`
```
# Download source-code for sched-ext/scx
$ cd $HOME/Downloads
$ git clone https://github.com/sched-ext/scx.git

# Build scx
$ cd scx
$ meson setup build
$ meson compile -C build

# Install scx 
$ sudo -E $HOME/.local/bin/meson install -C build
```

After the installation process, we shall note the following:

1. All the header files contained in `scx/scheds/include/scx` are copied into `/usr/local/include/scx`, i.e.

```
`$HOME/Downloads/scx/scheds/include/scx/*.h` --> `/usr/local/include/scx`
```
2. All the user-space scheduler algorithm executables are transferred into `/usr/local/bin` , ie.
```
  `scheds/c/scx_simple` --> `/usr/local/bin/scx_simple`
```
3.The relevant services are transferred into `/lib/systemd/system` , ie.

```
# These are just notes to be reviewed as you progress through the steps
# You do not have to execute any instructions within this block. 
`$HOME/Downloads/scx/services/systemd/*.service` --> `/lib/systemd/system`
	a. scx.service
	b. scx_loader.service
	c. org.scx.Loader.service --> `/usr/share/dbus-1/system-services`
	d. `./systemd/../scx` --> `/etc/default`
```

### I.C.2. Configuring the use of `vmlinux.h`

For the purposes of the exercises that we shall be executing, the installation of `/sched-ext/scx` is not complete yet. We need to ensure that the a specific header file ( `vmlinux.h`) that is required for compiling the `sched-ext` schedulers is made available to all new applications. Therefore, we shall manually copy the `vmlinux.h` generated during the successful build and compilation of `sched-ext/scx` into the `/usr/local/include/scx` folder that contains all other header files, ie.

```
`$HOME/Downloads/scx/scheds/include/vmlinux.h` -> `/usr/local/include/scx/vmlinux.h`
```

### I.C.3. Determining correctness of Vmlinux Header File
On executing the following command

```
# Determine the bpf-functions/methods useful for sched-ext/scx
grep scx_bpf ./vmlinux.h
```
Ensure that you find a list that resembles the following:

```
extern bool scx_bpf_consume(u64 dsq_id) __weak __ksym;
extern struct rq *scx_bpf_cpu_rq(s32 cpu) __weak __ksym;
extern u32 scx_bpf_cpuperf_cap(s32 cpu) __weak __ksym;
extern u32 scx_bpf_cpuperf_cur(s32 cpu) __weak __ksym;
extern void scx_bpf_cpuperf_set(s32 cpu, u32 perf) __weak __ksym;
extern s32 scx_bpf_create_dsq(u64 dsq_id, s32 node) __weak __ksym;
extern void scx_bpf_destroy_dsq(u64 dsq_id) __weak __ksym;
extern void scx_bpf_dispatch(struct task_struct *p, u64 dsq_id, u64 slice, u64 enq_flags) __weak __ksym;
extern void scx_bpf_dispatch_cancel(void) __weak __ksym;
extern bool scx_bpf_dispatch_from_dsq(struct bpf_iter_scx_dsq *it__iter, struct task_struct *p, u64 dsq_id, u64 enq_flags) __weak __ksym;
extern void scx_bpf_dispatch_from_dsq_set_slice(struct bpf_iter_scx_dsq *it__iter, u64 slice) __weak __ksym;
extern void scx_bpf_dispatch_from_dsq_set_vtime(struct bpf_iter_scx_dsq *it__iter, u64 vtime) __weak __ksym;
extern u32 scx_bpf_dispatch_nr_slots(void) __weak __ksym;
extern void scx_bpf_dispatch_vtime(struct task_struct *p, u64 dsq_id, u64 slice, u64 vtime, u64 enq_flags) __weak __ksym;
extern bool scx_bpf_dispatch_vtime_from_dsq(struct bpf_iter_scx_dsq *it__iter, struct task_struct *p, u64 dsq_id, u64 enq_flags) __weak __ksym;
extern void scx_bpf_dsq_insert(struct task_struct *p, u64 dsq_id, u64 slice, u64 enq_flags) __weak __ksym;
extern void scx_bpf_dsq_insert_vtime(struct task_struct *p, u64 dsq_id, u64 slice, u64 vtime, u64 enq_flags) __weak __ksym;
extern bool scx_bpf_dsq_move(struct bpf_iter_scx_dsq *it__iter, struct task_struct *p, u64 dsq_id, u64 enq_flags) __weak __ksym;
extern void scx_bpf_dsq_move_set_slice(struct bpf_iter_scx_dsq *it__iter, u64 slice) __weak __ksym;
extern void scx_bpf_dsq_move_set_vtime(struct bpf_iter_scx_dsq *it__iter, u64 vtime) __weak __ksym;
extern bool scx_bpf_dsq_move_to_local(u64 dsq_id) __weak __ksym;
extern bool scx_bpf_dsq_move_vtime(struct bpf_iter_scx_dsq *it__iter, struct task_struct *p, u64 dsq_id, u64 enq_flags) __weak __ksym;
extern s32 scx_bpf_dsq_nr_queued(u64 dsq_id) __weak __ksym;
extern void scx_bpf_dump_bstr(char *fmt, unsigned long long *data, u32 data__sz) __weak __ksym;
extern void scx_bpf_error_bstr(char *fmt, unsigned long long *data, u32 data__sz) __weak __ksym;
extern void scx_bpf_events(struct scx_event_stats *events, size_t events__sz) __weak __ksym;
extern void scx_bpf_exit_bstr(s64 exit_code, char *fmt, unsigned long long *data, u32 data__sz) __weak __ksym;
extern const struct cpumask *scx_bpf_get_idle_cpumask(void) __weak __ksym;
extern const struct cpumask *scx_bpf_get_idle_smtmask(void) __weak __ksym;
extern const struct cpumask *scx_bpf_get_online_cpumask(void) __weak __ksym;
extern const struct cpumask *scx_bpf_get_possible_cpumask(void) __weak __ksym;
extern void scx_bpf_kick_cpu(s32 cpu, u64 flags) __weak __ksym;
extern u64 scx_bpf_now(void) __weak __ksym;
extern u32 scx_bpf_nr_cpu_ids(void) __weak __ksym;
extern s32 scx_bpf_pick_any_cpu(const struct cpumask *cpus_allowed, u64 flags) __weak __ksym;
extern s32 scx_bpf_pick_idle_cpu(const struct cpumask *cpus_allowed, u64 flags) __weak __ksym;
extern void scx_bpf_put_cpumask(const struct cpumask *cpumask) __weak __ksym;
extern void scx_bpf_put_idle_cpumask(const struct cpumask *idle_mask) __weak __ksym;
extern u32 scx_bpf_reenqueue_local(void) __weak __ksym;
extern s32 scx_bpf_select_cpu_dfl(struct task_struct *p, s32 prev_cpu, u64 wake_flags, bool *is_idle) __weak __ksym;
extern struct cgroup *scx_bpf_task_cgroup(struct task_struct *p) __weak __ksym;
extern s32 scx_bpf_task_cpu(const struct task_struct *p) __weak __ksym;
extern bool scx_bpf_task_running(const struct task_struct *p) __weak __ksym;
extern bool scx_bpf_test_and_clear_cpu_idle(s32 cpu) __weak __ksym;
```

We thus ensure that the `vmlinux.h` file can now be safely used for developing custom user-space schedulers. We shall now proceed to the task of building some custom user-space process schedulers that rely on the `sched-ext/scx` kernel for their correct operation.

## I.D. Obtain the Minimal-Scheduler Repository

We shall now follow the instructions provided in the following tutorial on implementing a minimal scheduler in the user-space, that shall run over the eBPF based `sched_ext` library.

### I.D.1. Minimal Scheduler : Round-Robin Scheduler

In the following tutorial, we shall proceed to implement a minimal scheduler written in `sched_ext` that implements a `Round-Robin` scheduling policy (non pre-emptive with each task being allocated a fixed time-slot). This scheduler uses a global scheduling queue from which every CPU gets its tasks to run for a time slice.

We shall now proceed to download the `minimal-scheduler` from the following repository:

```
git clone https://github.com/parttimenerd/minimal-scheduler
cd minimal-scheduler

# remove the vmlinux.h file
rm -rf ./vmlinux.h
```

We need to configure the `vmlinux.h` file that should be used by the user-space schedulers. It is **imperative** that we use the `vmlinux.h` file that was generated during the installation of `sched-ext/scx` in the above sections. Copy the `vmlinux.h`from `/usr/local/include/scx/vmlinux.h` into the `minimal-scheduler` project directory ( `$HOME/Downloads/minimal-scheduler` ):

```
$ cp /usr/local/include/scx/vmlinux.h $HOME/Downloads/minimal-scheduler
```

Before we begin to examine how the scheduler is implemented, we shall try to use the minimal-scheduler using the following commands. We begin by building the scheduler library:

```
# build the scheduler binary
./build.sh
```

The `build.sh` script compiles the scheduler code to BPF bytecode by utilizing the  `vmlinux.h` header file we obtained from `sched-ext/scx`:
```
clang -target bpf -g -O2 -c sched_ext.bpf.c -o sched_ext.bpf.o -I. 
```
We can now start the scheduler by using the `start.sh` script:
```
# start the scheduler 
sudo ./start.sh 

# do something ...
```
Executing the `start.sh` script as a root user attaches the custom-scheduler using the `bpftool`:
```
bpftool struct_ops register sched_ext.bpf.o /sys/fs/bpf/sched_ext
```
The custom scheduler is now the scheduler of the operating system. You can check this by accessing the `/sys/kernel/sched_ext/root/ops` file:
```
$ cat /sys/kernel/sched_ext/root/ops
minimal_scheduler
```
And by checking `dmesg | tail`:
```
$ sudo dmesg | tail
# ...
[32490.366637] sched_ext: BPF scheduler "minimal_scheduler" enabled
```
If you're done, you can detach the scheduler by running the `stop.sh` script using root privileges. This removes the `/sys/fs/bpf/sched_ext/sched_ops` file.
```
# stop the scheduler 
sudo ./stop.sh
```

## I.E.Submission

Confirm the successful execution of the custom user-space process scheduler by executing the following instructions:

```
$ sudo dmesg | tail 
# ... [32490.366637] sched_ext: BPF scheduler "minimal_scheduler" enabled
```
We shall be using the above results to evaluate your PA submission.

### I.E.I Submission
+ A screenshot of your grub boot menu showing the new kernel as an option.
+ A screenshot of the output of `uname -a` command showing the new kernel version (`6.14.2-rev1`).
+ **We'll confirm that your Cloud VM boots, by default, into the -rev1 version of the kernel. Thus, you MUST boot into the new kernel version after compiling and installing successfully.**
+ A screenshot of the output of `sudo dmesg | tail` command showing that the `minimal_scheduler` had been enabled. (We shall look for the log message: `sched_ext: BPF scheduler "minimal_scheduler" enabled` )


### I.F. References
1. [Minimal Scheduler github-repo]( https://github.com/parttimenerd/minimal-scheduler )
2. [Sched-Ext/Scx github-repo](https://github.com/sched-ext/scx/wiki)
3. [FOSDEM25 - Sched-Ext github-repo](https://github.com/sched-ext/fosdem25)
4. [Sched-Ext : A BPF Extensible Scheduler Class Part-I](https://blogs.igalia.com/changwoo/sched-ext-a-bpf-extensible-scheduler-class-part-1)
5. [Sched-Ext: Scheduler Architecture and Interface Part-II](https://blogs.igalia.com/changwoo/sched-ext-scheduler-architecture-and-interfaces-part-2)
6. [SCX Simple Scheduler : User-space Implementation](https://eunomia.dev/en/tutorials/44-scx-simple/#user-space-implementation)





