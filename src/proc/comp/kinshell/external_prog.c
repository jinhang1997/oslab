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
  
  // fork a new process for child process
  pid = fork();
  if (pid == -1)  // failed to fork new process
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
    // input redirection enabled
    if (cmd->flag.dup_stdin)
    {
      in_fd = open(cmd->in_file, O_RDONLY);
#ifdef DEBUG
      log("input redirection, in_fd: %d, in_file: %s", in_fd, cmd->in_file);
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
    // output redirection enabled
    if (cmd->flag.dup_stdout & 0x1)
    {
      if (cmd->flag.dup_stdout == 0x1)  // create new file
      {
        out_fd = open(cmd->out_file, O_CREAT | O_WRONLY | O_TRUNC, 0666);
      }
      else if (cmd->flag.dup_stdout == 0x3) // append to the file if existing
      {
        out_fd = open(cmd->out_file, O_CREAT | O_WRONLY | O_APPEND, 0666);
      }
      else
      {
        sprintf(buf, "invaild output redirection flag [%u].", cmd->flag.dup_stdout);
        warning(buf); 
        exit(1);
      }
#ifdef DEBUG
      log("output redirection, out_fd: %d, out_file: %s (redirection mode: %u)", out_fd, cmd->out_file, cmd->flag.dup_stdout);
#endif
      if (in_fd == -1)
      {
        sprintf(buf, "failed to open file [%s] for reading.", cmd->in_file);
        warning(buf);
        exit(1);
      }
      dup2(out_fd, 1);
      close(out_fd);
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
    log("child exited. child pid: %d, stat: %d", pid, status);
#endif
    return WEXITSTATUS(status);
  }
  return 0;
}


