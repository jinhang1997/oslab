#include "common.h"

void panic(char *word)
{
  printf("\033[31mError: %s\033[0m\n", word);
  exit(1);
}

void warning(char *word)
{
  printf("\033[33mWarning: %s\033[0m\n", word);
}


