#include "kinshell.h"

//extern pipebuf_t pipebuf;
//extern int pipefd[2];

#ifdef DEBUG

void reset_logfile()
{
  int fd = open("log.txt", O_CREAT | O_WRONLY | O_TRUNC, 0666);
  close(fd);
}

#endif

int main(int args, char *argv[])
{
#ifdef DEBUG
  reset_logfile();
#endif
  
  printf("\033[;32mWelcome to Kin Shell!\033[0m\n");
  printf("Rules of commands are:\n");
  printf("\033[;32mcommand1[ arguments][ < inputfile][ > outputfile][ | command2 ...]\033[0m\n");
  printf("Note that the buf size of pipe is \033[;32m4096 bytes\033[0m.\n");
  //printf("For more help, type `help' in commandlines.\n");
  
  ui_mainloop();
  
  return 0;
}


