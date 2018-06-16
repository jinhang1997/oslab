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
  printf("Line read: [%s]\n", line_read);
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
  //int cmdlen = strlen(cmd);
  strcpy(commands[0].command, cmd);
  strcat(commands[0].argument, "");
  arg = strtok(NULL, " ");
  while (arg)
  {
    strcat(commands[0].argument, arg);
    arg = strtok(NULL, " ");
    if (!arg)
    {
      break;
    }
    strcat(commands[0].argument, " ");
  }
  
#ifdef DEBUG
  printf("Arguments: [%s]\n", commands[0].argument);
  printf("Command: [%s] [%s]\n", commands[0].command, commands[0].argument);
#endif 
  
  return count_command;
}


