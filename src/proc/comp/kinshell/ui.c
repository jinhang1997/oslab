#include "kinshell.h"
#include <readline/readline.h>
#include <readline/history.h>

char buf[512], cwd[512];

cmd_t commands[4];

int num_of_commands = 0;

int ui_mainloop()
{
  while (true)
  {
    memset(commands, 0, sizeof(commands));
    getcwd(cwd, 512);
    sprintf(buf, "kinshell:%s$ ", cwd);
    num_of_commands = read_command(commands, buf);
    fflush(stdin);
    
    // match with each command
    if (builtin_command(commands[0].command, commands[0].argument))
    {
      continue;
    }
    else
    {
      sprintf(buf, "%s %s", commands[0].command, commands[0].argument);
      mysys(buf);
    }
  }
  
  return 0;
}
