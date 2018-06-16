#ifndef KINSHELL_H
#define KINSHELL_H

//#define DEBUG

typedef unsigned char bool;
#define true 1
#define false 0

#include "proc.h"
#include <string.h>

typedef struct {
  char command[32];
  char argument[256];
} cmd_t;

int ui_mainloop(void);

int read_command(cmd_t *commands, char *prompt);

int builtin_command(char *command, char *argument);


#endif
