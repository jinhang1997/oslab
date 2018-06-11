#include "kinshell.h"

int main(int args, char *argv[])
{
  printf("\033[;32mWelcome to Kin Shell!\033[0m\n");
  
  ui_mainloop();
  
  return 0;
}


