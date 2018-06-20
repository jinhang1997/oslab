#include "proc.h"

pid_t wait(int *status);

int mysys(const char *command)
{
  //printf("In function mysys: %s\n", command);
  int pid, status;
  char *argv[4];
  
  argv[0] = "/bin/sh";
  argv[1] = "-c";
  argv[2] = (char *)command;
  argv[3] = NULL;
  
  if ((pid = fork()) == -1)
  {
    return -1;
  }
  else if (pid == 0)
  {
    //printf("pid = %d\n", pid);
    execv("/bin/sh", argv);
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
  ret = mysys("ls /");
  printf("---------------\n");
  printf("Process exited with return value %d.\n", ret);
  printf("------------------------------\n");
  /*ret = mysys("~/ret1");
  printf("---------------\n");
  printf("Process exited with return value %d\n", ret);
  printf("------------------------------\n");*/
  
  return 0;
}
