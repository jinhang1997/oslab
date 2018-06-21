#include "kinshell.h"

int external_prog(int total, int depth)
{
  if (depth == total)
  {
    exit(EXIT_SUCCESS);
  }
  
  int fd[2];
  int pid;
  int status;
  int in_fd, out_fd;
  char buf[256];
  char *argv[4];
#ifdef DEBUG
  FILE *log_child = NULL;
#endif 
  
  cmd_t *cmd = &commands[total - depth - 1];
  argv[0] = "/bin/sh";
  argv[1] = "-c";
  sprintf(buf, "%s %s", cmd->command, cmd->argument);
  argv[2] = buf;
  argv[3] = NULL;
  
  // pipe, fd[0] is reader and fd[1] is writer
  /*if (cmd->flag.piped & INFO_PIPED_OUT)
  {
    int pipefd[2]; 
    if (!init_pipe(pipefd))
    {
      return -1;
    }
  }*/
  if (pipe(fd) < 0)
  {
    exit(EXIT_FAILURE);
  }
  //log("pipe: fd[0] = %d, fd[1] = %d", fd[0], fd[1]);
  
  // fork a new process for child process
  pid = fork();
  
  if (pid < 0)  // failed to fork new process
  {
#ifdef DEBUG  
    warning("failed to fork a new process, pid: %d", pid);
#endif
    exit(EXIT_FAILURE);
  }
  else if (pid == 0)  // child process
  {
#ifdef DEBUG
    if (log_child != NULL)
    {
      fclose(log_child);
    }
    if ((log_child = fopen("log1.txt", "a")) == NULL)
    {
      return 1;
    }
    log2file(log_child, "/////logfile opened for child process at depth %d/%d", depth, total);
    log2file(log_child, "pipe: fd[0] = %d, fd[1] = %d", fd[0], fd[1]);
    log2file(log_child, "child pid %d: [%s].", pid, argv[2]);
    log2file(log_child, "child is setting pipe of stdout...");
#endif
    if (depth != total - 1)
    {
      dup2(fd[1], fileno(stdout));
      close(fd[0]);
      close(fd[1]);
#ifdef DEBUG
      log2file(log_child, "pipe of stdout is set");
#endif
    }
#ifdef DEBUG
    log2file(log_child, "logfile is about to be closed");
    fclose(log_child);
#endif
    // load the child process.
    external_prog(total, depth + 1);
  }
  else  // parent process
  {   
#ifdef DEBUG
    FILE *log_parent = NULL;
    if (log_parent != NULL)
    {
      fclose(log_parent);
    }
    log_parent = fopen("log2.txt", "a");
    if (!log_parent)
    {
      warning("cannot open logfile for parent process");
      exit(EXIT_FAILURE);
      return 1;
    }
    //log("123123");
    //log("%d", fileno(log_parent));
    log2file(log_parent, "/////logfile opened for traceback at depth %d", depth);
    log2file(log_parent, "child process %d exited with status %d (%d)", pid, status, WEXITSTATUS(status));
    log2file(log_parent, "parent process is setting pipe of stdin");
#endif
    waitpid(pid, &status, 0);
    if (depth != total - 1)
    {
      dup2(fd[0], fileno(stdin));
      close(fd[0]);
      close(fd[1]);
    }
#ifdef DEBUG
    log2file(log_parent, "child is judging if redirection is needed...");
#endif
    // input redirection enabled
    if (cmd->flag.dup_stdin)
    {
      in_fd = open(cmd->in_file, O_RDONLY);
#ifdef DEBUG
      log2file(log_parent, "input redirection, in_fd: %d, in_file: %s", in_fd, cmd->in_file);
#endif
      if (in_fd == -1)
      {
#ifdef DEBUG      
        log2file(log_parent, "failed to open file [%s] for reading.", cmd->in_file);
#endif
        exit(EXIT_FAILURE);
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
#ifdef DEBUG       
        log2file(log_parent, "invaild output redirection flag [%u].", cmd->flag.dup_stdout); 
#endif
        exit(EXIT_FAILURE);
      }
#ifdef DEBUG
      log2file(log_parent, "output redirection, out_fd: %d, out_file: %s (redirection mode: %u)", out_fd, cmd->out_file, cmd->flag.dup_stdout);
#endif
      if (out_fd == -1)
      {
#ifdef DEBUG       
        log2file(log_parent, "failed to open file [%s] for writing.", cmd->out_file);
#endif    
        exit(EXIT_FAILURE);
      }
      dup2(out_fd, 1);
      close(out_fd);
    }
    //log("command about to be executed: [%s]", argv[2]);
    execvp("/bin/sh", argv);
#ifdef DEBUG
    //log("child exited. child pid: %d, stat: %d", pid, status);
#endif
    return WEXITSTATUS(status);
  }
  return 0;
}


