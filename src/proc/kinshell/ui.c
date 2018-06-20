#include "kinshell.h"
#include <readline/readline.h>
#include <readline/history.h>

char buf[512], cwd[128];

cmd_t commands[16];

int num_of_commands = 0;

struct passwd *pw;

int ui_mainloop()
{
  while (true)
  {
    pw = getpwuid(getuid());
    memset(commands, 0, sizeof(commands));
    getcwd(cwd, 128);
    sprintf(buf, "%s@kinshell:%s$ ", pw->pw_name, cwd);
    num_of_commands = read_command(commands, buf);
    fflush(stdin);
    
    // match with each command
    if (builtin_command(commands[0].command, commands[0].argument))
    {
      continue;
    }
    else
    {
      /*sprintf(buf, "%s %s", commands[0].command, commands[0].argument);
      mysys(buf);*/
      external_prog(&commands[0]);
    }
  }
  
  return 0;
}
