# OSLabs
NUAA OS Labs 2018

Author: Jin Hang, CCST, NUAA

2018.6

# Problems & Source Files
## Problems of File I/O
### myecho.c
Like `echo` command in shell, receive arguments and print them on the screen.

### mycat.c
Like `cat` command in shell, receive arguments which indicates filenames whose  will be shown and print the whole file on the screen.

If no argument is given, `mycat` will read from `stdin` and print it out until `^D`.

### mycp.c
Like `cp` command in shell, receive 2 arguments which indicates the names of source file and the dest file. Then read the source file and write everything into dest file.

`mycp` uses `lstat` to read the mode of source file and give the same mode to the dest file.
