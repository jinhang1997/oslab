#ifndef KINSHELL_H
#define KINSHELL_H

#define DEBUG

typedef unsigned char bool;
#define true 1
#define false 0

#include "proc.h"
#include <string.h>
#include <sys/types.h>
#include <pwd.h>

typedef struct {
  char command[32];
  char argument[32];
  bool dup_stdin;
  char in_file[32];
  bool dup_stdout;
  char out_file[32];
} cmd_t;

int ui_mainloop(void);

int read_command(cmd_t *commands, char *prompt);

int builtin_command(char *command, char *argument);
int external_prog(cmd_t *cmd);

extern struct passwd *pw;

#define log(format, ...) \
  do { \
    fprintf(stdout, "\033[1;34m[%s,%d,%s] " format "\033[0m\n", \
      __FILE__, __LINE__, __func__, ## __VA_ARGS__); \
  } while (0); \
  fflush(stdout);

#endif
