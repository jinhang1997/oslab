#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void panic(char *word)
{
  printf("\033[31mError:\033[0m %s\n", word);
  exit(1);
}

