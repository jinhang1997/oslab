#include "kinshell.h"

char buf[512], cwd[128];
cmd_t commands[16];
int num_of_commands = 0;
struct passwd *pw;

int ui_mainloop()
{
  int i;
  while (true)
  {
    pw = getpwuid(getuid());
    memset(commands, 0, sizeof(commands));
    getcwd(cwd, 128);
    sprintf(buf, "%s@kinshell:%s$ ", pw->pw_name, cwd);
    num_of_commands = read_command(commands, buf);
#ifdef DEBUG    
    log("%d command(s) has been read successfully.", num_of_commands);
    log("They are:");
    for (i = 0; i < num_of_commands; i++)
    {
      log("[%s](%s), flag: 0x%x",
        commands[i].command, commands[i].argument, commands[i].flag.val);
    }
#endif    
    fflush(stdin);
    
    // match with each command and
    // judge if command is builted in
    if (builtin_command(commands[0].command, commands[0].argument))
    {
      continue;
    }
    else  // command is not builted in but a external program
    {
      int status;
      int pid = fork();
      if (pid < 0)
      {
        warning("failed to execute extern commands.");
      }
      else if (pid == 0)
      {
        external_prog(num_of_commands, 0);
      }
      else
      {
        waitpid(pid, &status, 0);
      }
    }
  }
  
  return 0;
}
