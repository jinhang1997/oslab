#include "kinshell.h"

static char buf[512];

static int builtin_exit(char *args)
{
  exit(0);
  
  return 1;
}

static int builtin_pwd(char *args)
{
  if (!getcwd(buf, 512))
  {
    warning("pwd: cannot get the current working directory.");
    return 1;
  }
  printf("%s\n", buf);
  
  return 1;
}

static int builtin_cd(char *args)
{
  if (!args || strlen(args) == 0 || !strcmp(args, "~"))
  {
    args = pw->pw_dir;
  }
#ifdef DEBUG
  log("cd: trying to change dir to [%s].", args);
#endif
  if (chdir(args))
  {
    warning("cd: cannot change to the specified directory.");
  }
#ifdef DEBUG
  else
  {
    log("cd: operation succeded.");
  }
#endif

  return 1;
}

struct {
  char *name;
  int (*handler)(char *);
} builtin_list[] = {
  {"exit", builtin_exit},
  {"pwd", builtin_pwd},
  {"cd", builtin_cd},
  
};

#define NR_BUILTIN sizeof(builtin_list) / sizeof(builtin_list[0])

int builtin_command(char *command, char *argument)
{
  int i;
  for (i = 0; i < NR_BUILTIN; i++)
  {
    if (!strcmp(builtin_list[i].name, command))
    {
#ifdef DEBUG
      log("built-in command [%s](%s) found.", command, argument);
#endif
      return builtin_list[i].handler(argument);
    }
  }
#ifdef DEBUG
      log("command [%s] was not found in built-in commands.", command);
#endif
  return 0;
}
