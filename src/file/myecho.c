#include "common.h"

int main(int argc, char *argv[])
{
  int idx = 1;
  for (idx = 1; idx < argc; idx++)
  {
    if (idx == argc - 1)
    {
      printf("%s", argv[idx]);
    }
    else
    {
      printf("%s ", argv[idx]);
    }
  }
  putchar('\n');
  return 0;
}
