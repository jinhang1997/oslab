#include "common.h"

void panic(char *word)
{
  printf("\033[31mError:\033[0m %s\n", word);
  exit(1);
}

void warning(char *word)
{
  printf("\033[33mWarning:\033[0m %s\n", word);
}


