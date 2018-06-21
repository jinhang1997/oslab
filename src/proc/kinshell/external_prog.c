#include "kinshell.h"

/*int init_pipe(int pipefd[])
{
#ifdef DEBUG
  log("initializing pipe for child process...");
#endif
  //memset((void *)&pipebuf, 0, sizeof(pipebuf));
  if (pipe(pipefd))
  {
    warning("failed to create a pipe.");
    return 1;
  }
#ifdef DEBUG
  else
  {
    log("new pipe created: pipefd[0] = %d, pipefd[1] = %d", pipefd[0], pipefd[1]);
  }
#endif
  return 0;
}*/

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
    FILE *log_file;
    //log("openning log file for child process...");
    if ((log_file = fopen("log.txt", "a")) == NULL)
    {
      return 1;
    }
    log2file(log_file, "child, pid: %d, shargs: %s", pid, argv[2]);
    log2file(log_file, "child is setting pipe...");
#endif
    dup2(fd[1], fileno(stdout));
    close(fd[0]);
    close(fd[1]);


#ifdef DEBUG
    log2file(log_file, "child is judging if redirection is needed...");
#endif
    // input redirection enabled
    if (cmd->flag.dup_stdin)
    {
      in_fd = open(cmd->in_file, O_RDONLY);
#ifdef DEBUG
      log2file(log_file, "input redirection, in_fd: %d, in_file: %s", in_fd, cmd->in_file);
#endif
      if (in_fd == -1)
      {
#ifdef DEBUG      
        log2file(log_file, "failed to open file [%s] for reading.", cmd->in_file);
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
        log2file(log_file, "invaild output redirection flag [%u].", cmd->flag.dup_stdout); 
#endif
        exit(EXIT_FAILURE);
      }
#ifdef DEBUG
      log2file(log_file, "output redirection, out_fd: %d, out_file: %s (redirection mode: %u)", out_fd, cmd->out_file, cmd->flag.dup_stdout);
#endif
      if (in_fd == -1)
      {
#ifdef DEBUG       
        log2file(log_file, "failed to open file [%s] for reading.", cmd->in_file);
#endif    
        exit(EXIT_FAILURE);
      }
      dup2(out_fd, 1);
      close(out_fd);
    }
#ifdef DEBUG   
    fclose(log_file);
#endif  
    // load the child process.
    external_prog(total, depth + 1);
#ifdef DEBUG
    //log2file(log_file, "command [%s] is about to exit.", buf);
#endif  
    //exit(EXIT_SUCCESS);
  }
  else  // parent process
  {
#ifdef DEBUG
    //log("parent start waiting for pid: %d", pid);
#endif
    waitpid(pid, &status, 0);
    dup2(fd[0], fileno(stdin));
    close(fd[0]);
    close(fd[1]);
    //log("command about to be executed: [%s]", argv[2]);
    execvp("/bin/sh", argv);
#ifdef DEBUG
    //log("child exited. child pid: %d, stat: %d", pid, status);
#endif
    return WEXITSTATUS(status);
  }
  return 0;
}


