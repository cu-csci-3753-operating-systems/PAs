# DNS Resolver

## Introduction
In this assignment, you will develop an application that resolves domain names (eg. google.com) to IP addresses. This is similar to a part of the process that takes place when you access a website in your web browser.

## Implementation
We provide a number of [input files](../common/hostnames/) that contain one hostname per line. Your program should process filenames passed via the command line, as well as a path to a log file:
```
./dns_lookup <log_file> [ <names1> <names2> ... ]
```

Read each line of each file supplied, lookup the IP address for the corresponding hostname, and then write the hostname and IP address to the log file using the following format:
```
google.com, 142.250.72.14
```

If an IP address cannot be found for a given hostname, `NOT_RESOLVED` should be written instead of an IP address:
```
glsdkjf.com, NOT_RESOLVED
```
Note, your program only needs to resolve one IPv4 address.

After processing each hostname of each file, prior to exiting, your program should print the total time taken:
```
./dns_lookup: total time is 2.810863 seconds
```

## Error handling
Ensure your program accounts for:
- Missing arguments - terminate with a usage synopsis to `stdout`
- If a log file exists and is writeable - its contents should be overwritten
- If a log file does not exist - it should be created by your program
- Missing or unreadable files - print "Invalid file <filaneme>" to `stderr` and continue to next file

All function calls should be checked for errors, and if fatal, print an error message and exit with the appropriate status.

## Submission requirements
- Your `lookup.c` source code
- A 1 page report of things you learned, difficulties you encountered and how you overcame them, comments about the assignment, etc.
