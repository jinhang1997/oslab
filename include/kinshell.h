#ifndef KINSHELL_H
#define KINSHELL_H

#define DEBUG

typedef unsigned char bool;
#define true 1
#define false 0

#include "common.h"

typedef union {
  struct {
    unsigned int dup_stdin :1;
    unsigned int dup_stdout :2;
    unsigned int :1;
    unsigned int piped: 2;
  };
  unsigned int val;
} cmd_info;

#define INFO_IN_RED 0x1
#define INFO_OUT_RED 0x1
#define INFO_OUT_RED_APPEND 0x2
#define INFO_PIPED_IN 0x1
#define INFO_PIPED_OUT 0x2

typedef struct {
  char command[32];
  char argument[32];
  //bool dup_stdin;
  char in_file[32];
  //bool dup_stdout;
  char out_file[32];
  cmd_info flag;
} cmd_t;

typedef struct {
  char buf[4096];
  int size;
} pipebuf_t;

pid_t waitpid(pid_t pid, int *status, int options);

int ui_mainloop(void);

int read_command(cmd_t *commands, char *prompt);

int builtin_command(char *command, char *argument);

int external_prog(int total, int depth);

extern struct passwd *pw;

extern cmd_t commands[16];

#endif
