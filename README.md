# OSLabs
NUAA OS Labs 2018

Author: Jin Hang, CCST, NUAA

2018.6

# Problems of File I/O
## myecho.c
Like `echo` command in shell, receive arguments and print them on the screen.

## mycat.c
Like `cat` command in shell, receive arguments which indicates filenames whose  will be shown and print the whole file on the screen.

If no argument is given, `mycat` will read from `stdin` and print it out until `^D`.

## mycp.c
Like `cp` command in shell, receive 2 arguments which indicates the names of source file and the dest file. Then read the source file and write everything into dest file.

`mycp` uses `lstat` to read the mode of source file and give the same mode to the dest file.

# Problems of Multiple Processes

## mysysmain.c
A Simplified edition of `system' function in library. Start and load a new /bin/sh to execute program in the new process while the main process wait until the children returns. Thers's also a testcase to call `mysys' in.

## KinShell
A simulated shell program made by kin with part functions from the orginal shell. Thus, each command should be input as following rules:
``` bash
command1 [arguments] [< inputfile] [> outputfile] [| command2 ...]
```

### supported built-in commands
* cd: change current working directory.
* pwd: print the current working directory.
* exit: exit the kinshell and return to the OS.

### external programs
* use `mysys` to execute external programs.
* added support for input redirection with `<` or `<<`.
* added support for output redirection with `>`(forced new file) or `>>`(append).

### support features
* allow at most 16 piped commands in a single commandline.
* redirection of `stdin` and/or `stdout` to specified file.
* use pipe to transfer output and/or input from command to the following one.

### organization of source and header files
* src/proc/kinshellmain.c: main program to be executed.
* src/proc/kinshell/ui.c: main loop used for user interface.
* src/proc/kinshell/read_command.c: use `readline` library to read command lines from console and it provides feature of command history by pressing `↑` and `↓` keys. Then, commands will be parsed and prepared to be execute in next steps.
* src/proc/kinshell/builtin_command.c: built-in commands of `cd` and `pwd` in which pipe and redirection is not allowed.
* src/proc/kinshell/external_prog.c: all commands except built-in ones, use regression to transfer data via pipe to child processes.
* include/common.h: references to some essential headers in libraries and some convenient macroes for debugging.
* include/kinshell.h: data structure definations and declarations of global variables and functions. **Note that `DEBUG` macro is defined here, you may comment it if not needed.**

# Problems of Multiple Threads

## pi1.c
Uses 2 threads to calculate PI according to Leibniz Series.

## pi2.c
Uses a specified number given by user of threads to calculate PI according to Leibniz Series.

## sort.c
Uses 2 threads to sort an array of numbers and merge them together.

## pc1.c
* the producer puts letters `a` to `h` into buffer1.
* the processor gets letters from buffer1.
* the processor convert letters into upper forms then put them into buffer2.
* the consumer gets letters from buffer2 and print them on the screen (emphasized).
* each step is printed on the screen but the emphasized blue ones are required only.
* use condition variables to collaborate threads.

## pc2.c
* things are same to the `pc1.c` above.
* note that `pc2.c` uses semaphore to collaborate threads.

## ring.c
* create specified numbers of threads named `t1` to `tn`.
* `t1` send an integer 1 to `t2`, and `t2` add 1 to the integer and send it to the next.
* the integer will be sent though all threads, and at last `tn` send `n` to the `t1`.
* each thread receives the integer and print it on the screen the do the next step.
* each `t(i+1)` thread is created in `ti`.

That's everything of introduction to my oslab.

Jin Hang, 2018. 6

