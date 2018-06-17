#include "kinshell.h"

int pid, status;
int in_fd, out_fd;
char buf[512];

pid_t wait(int *status);

int external_prog(cmd_t *cmd)
{
  int pid;
  char *argv[4];
  
  argv[0] = "/bin/sh";
  argv[1] = "-c";
  sprintf(buf, "%s %s", cmd->command, cmd->argument);
  argv[2] = buf;
  argv[3] = NULL;
  
  pid = fork();
  if (pid == -1)
  {
#ifdef DEBUG
    log("fork failed, pid: %d", pid);
#endif
    return -1;
  }
  else if (pid == 0)  // child process
  {
#ifdef DEBUG
    log("child, pid: %d, shargs: %s", pid, argv[2]);
#endif
    if (cmd->dup_stdin)
    {
      in_fd = open(cmd->in_file, O_RDONLY);
#ifdef DEBUG
      log("child, redirect, in_fd: %d, in_file: %s", in_fd, cmd->in_file);
#endif
      if (in_fd == -1)
      {
        sprintf(buf, "failed to open file [%s] for reading.", cmd->in_file);
        warning(buf);
        exit(1);
      }
      dup2(in_fd, 0);
      close(in_fd);
    }
    execv("/bin/sh", argv);
    exit(EXIT_SUCCESS);
  }
  else  // parent process
  {
#ifdef DEBUG
    log("parent start waiting for pid: %d", pid);
#endif
    wait(&status);
#ifdef DEBUG
    log("child proc exited parent pid: %d, stat: %d", pid, status);
#endif
    return WEXITSTATUS(status);
  }
  return 0;
}


