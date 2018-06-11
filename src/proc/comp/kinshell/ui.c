#include "kinshell.h"
#include <readline/readline.h>
#include <readline/history.h>

bool is_exit = false;

char buf[512];

char *line_read = NULL;

char *rl_read(char *prompt)
{
  if (line_read)
  {
    free(line_read);
    line_read = NULL;
  }
  
  line_read = readline(prompt);
  
  if (line_read && *line_read)
  {
    add_history(line_read);
  }
  
  return line_read;
}

int ui_mainloop()
{
  char *args = NULL;
  
  while (!is_exit)
  {
    char *str = rl_read("kinshell:$ ");
    // record the total length of original input
    int len = strlen(str);
#ifdef DEBUG
    //printf("Input: [%s]\n", str);
#endif 
    char *cmd = strtok(str, " ");
    int cmdlen = strlen(cmd);
    
    // no argument is given
    if (len == cmdlen)
    {
      args = "";
    }
    else
    {
      args = cmd + cmdlen + 1;
    }
      
    // match cmd with each command
    if (!strcmp(cmd, "exit"))
    {
      break;
    }
    else
    {
      sprintf(buf, "%s %s", cmd, args);
#ifdef DEBUG
      //printf("Command: [%s]\n", buf);
#endif 
      mysys(buf);
    }
  }
  
  return 0;
}
