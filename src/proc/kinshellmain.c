#include "kinshell.h"

int main(int args, char *argv[])
{
  printf("\033[;32mWelcome to Kin Shell!\033[0m\n");
  printf("Rules of commands:\n");
  printf("  command1 [arguments] [< inputfile] [> outputfile] [| command2 ...]\n");
  
  ui_mainloop();
  
  return 0;
}


