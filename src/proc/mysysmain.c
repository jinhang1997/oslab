#define DEBUG
#include "common.h"

#define MAX_ARGS 32

pid_t wait(int *status);

int mysys(const char *command)
{
  //printf("In function mysys: %s\n", command);
  char buf[256];
  int pid, status;
  char *argv[32];
  //char *cmd;//, *arg;
  int i;
  
  /*argv[0] = "/bin/sh";
  argv[1] = "-c";
  argv[2] = (char *)command;
  argv[3] = NULL;*/
  strcpy(buf, command);
  argv[0] = strtok(buf, " ");
  //arg = cmd + strlen(cmd) + 1;
  for (i = 1; i < MAX_ARGS; i++)
  {
    argv[i] = strtok(NULL, " ");
    //log("[%s]", argv[i]);
    if (argv[i] == NULL)
    {
      break;
    }
  }
#ifdef DEBUG  
  for (i = 0; i < MAX_ARGS; i++)
  {
    log("[%s]", argv[i]);
    if (argv[i] == NULL)
    {
      break;
    }
  }
#endif
  //argv[2] = NULL;
  //log("[%s] [%s]", cmd, arg);
  
  if ((pid = fork()) == -1)
  {
    return -1;
  }
  else if (pid == 0)
  {
    //printf("pid = %d\n", pid);
    execvp(argv[0], argv);
    exit(EXIT_SUCCESS);
  }
  else
  {
    //printf("pid = %d\n", pid);
    wait(&status);
    //printf("status = %d\n", status);
    return WEXITSTATUS(status);
  }

  return 0;
}

int main(int argc, char *argv[])
{
  int ret;
  
  printf("------------------------------\n");
  ret = mysys("echo HELLO WORLD");
  printf("---------------\n");
  printf("Process exited with return value %d.\n", ret);
  printf("------------------------------\n");
  ret = mysys("ls / /home");
  printf("---------------\n");
  printf("Process exited with return value %d.\n", ret);
  printf("------------------------------\n");
  /*ret = mysys("~/ret1");
  printf("---------------\n");
  printf("Process exited with return value %d\n", ret);
  printf("------------------------------\n");*/
  
  return 0;
}
