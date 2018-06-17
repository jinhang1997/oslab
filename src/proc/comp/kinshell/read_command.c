#include "kinshell.h"
#include <readline/readline.h>
#include <readline/history.h>

//static char buf[512];

static char *line_read = NULL;

static char *rl_read(char *prompt)
{
  if (line_read)
  {
    free(line_read);
    line_read = NULL;
  }
  
  line_read = readline(prompt);
  if (!line_read)
  {
    printf("\n");
    exit(0);
  }
  
#ifdef DEBUG
  log("Line read: [%s]", line_read);
#endif 
  
  if (line_read && *line_read)
  {
    add_history(line_read);
  }
  
  return line_read;
}

int read_command(cmd_t *commands, char *prompt)
{
  int i;
  int count_command = 1;
  
  char *str = rl_read(prompt);
  if (!str)
  {
    return -1;
  }
  
  // test if the hole input is empty
  int len = strlen(str);
  for (i = 0; i < len; i++)
  {
    if (str[i] != ' ')
    {
      break;
    }
    if (i == len - 1)
    {
      return 0;
    }
  }
  
  char *cmd = strtok(str, " ");
  char *arg;
  i = 0;
  strcpy(commands[0].command, cmd);
  while (true)
  {
    arg = strtok(NULL, " ");
    if (!arg)
    {
      break;
    }
    else if (!strcmp(arg, "<") || !strcmp(arg, "<<"))
    {
      commands[i].flag.dup_stdin = 1;
      arg = strtok(NULL, " ");
      strcpy(commands[0].in_file, arg);
    }
    else if (!strcmp(arg, ">"))
    {
      commands[i].flag.dup_stdout = 1;
      arg = strtok(NULL, " ");
      strcpy(commands[0].out_file, arg);
    }
    else if (!strcmp(arg, ">>"))
    {
      commands[i].flag.dup_stdout = 2;
      arg = strtok(NULL, " ");
      strcpy(commands[0].out_file, arg);
    }
    else
    {
      strcat(commands[i].argument, arg);
      strcat(commands[i].argument, " ");
    }
  }
  
  // erase the last space in each argument
  for (i = 0; i < count_command; i++)
  {
    len = strlen(commands[i].argument);
    if (len > 0 && commands[i].argument[len - 1] == ' ')
    {
      commands[i].argument[len - 1] = '\0';
    }
  }
  
#ifdef DEBUG
  log("Command & Arguments: [%s] [%s]", commands[0].command, commands[0].argument);
#endif 
  
  return count_command;
}


