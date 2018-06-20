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
## mysys.c
A Simplified edition of `system' function in library. Start and load a new /bin/sh to execute program in the new process while the main process wait until the children returns.

## mysysmain.c
A main program to call `mysys'.

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



