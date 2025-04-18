# Introduction
In this programming assignment, PA7, we shall focus on introducing a relatively new OS-kernel level technology called `BPF` which has become ubiquitous across the Linux, Windows and Mac-OS systems. It's a technology that started off as a way to improve the speeds of processing network packets based on the `(extended)-Berkeley Packet Filter`. It then grew into other use cases as process tracing and performance measurements within the OS. This technology is still under active development and some componenets of it have become integrated into the Linux-mainline code repository. 

## EBPF: The technology

To understand the `BPF`-based technology, we present a small excerpt from its [official documentation](https://ebpf.io/what-is-ebpf/).

eBPF is a revolutionary technology with origins in the Linux kernel that can run sandboxed programs in a privileged context such as the operating system kernel. It is used to safely and efficiently extend the capabilities of the kernel without requiring to change kernel source code or load kernel modules.

Historically, the operating system has always been an ideal place to implement observability, security, and networking functionality due to the kernel’s privileged ability to oversee and control the entire system. At the same time, an operating system kernel is hard to evolve due to its central role and high requirement towards stability and security. The rate of innovation at the operating system level has thus traditionally been lower compared to functionality implemented outside of the operating system.

eBPF changes this formula fundamentally. It allows sandboxed programs to run within the operating system, which means that application developers can run eBPF programs to add additional capabilities to the operating system at runtime. The operating system then guarantees safety and execution efficiency as if natively compiled with the aid of a Just-In-Time (JIT) compiler and verification engine. This has led to a wave of eBPF-based projects covering a wide array of use cases, including next-generation networking, observability, and security functionality.

Today, eBPF is used extensively to drive a wide variety of use cases: Providing high-performance networking and load-balancing in modern data centers and cloud native environments, extracting fine-grained security observability data at low overhead, helping application developers trace applications, providing insights for performance troubleshooting, preventive application and container runtime security enforcement, and much more. The possibilities are endless, and the innovation that eBPF is unlocking has only just begun.


## SCHED-EXT

`sched_ext` is a scheduler class whose behavior can be defined by a set of BPF programs - the BPF scheduler.

+ sched_ext exports a full scheduling interface so that any scheduling algorithm can be implemented on top.
+ The BPF scheduler can group CPUs however it sees fit and schedule them together, as tasks aren’t tied to specific CPUs at the time of wakeup.
+ The BPF scheduler can be turned on and off dynamically anytime.
+ The system integrity is maintained no matter what the BPF scheduler does. 
+ When the BPF scheduler triggers an error, debug information is dumped to aid debugging. 

The above excerpt is taken from the [official documentation of `sched-ext`.](https://docs.kernel.org/scheduler/sched-ext.html)

## PA7 : Writing a Custom User-space based Process Scheduler using BPF

Please follow the instructions provided in the following pages, to complete the programming assignment. 
1. [Installation](https://github.com/cu-csci-3753-operating-systems/PAs/blob/main/ebpf_sched/installation.md)
2. [Understanding Process Schedulers and the Minimal Scheduler](https://github.com/cu-csci-3753-operating-systems/PAs/blob/main/ebpf_sched/understanding_sched_ext.md)


You may review the following videos before you proceed with your programming assignment: 
1. [Writing a minimal sched-ext scheduler with eBPF + C](https://www.youtube.com/watch?v=cDM_bNKbBII)
2. [Writing production-ready schedulers using SCHED-EXT](https://www.youtube.com/watch?v=KoBwlQ0AQzg)



## References
1. [eBPF: Unlocking the Kernel ( Official Documentary)](https://www.youtube.com/watch?v=Wb_vD3XZYOA)
2. [The History of eBPF](https://www.youtube.com/watch?v=sYxUOKi3Q00)
3. [Kernel tracking using eBPF](https://www.youtube.com/watch?v=JqnWp6X6otw)
4. [Getting Started with BPF Observability - Brenda Gregg](https://www.youtube.com/watch?v=bGAVrtb_tFs)


## Acknowledgement