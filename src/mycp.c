#include "common.h"

char buf[512];

int main(int argc, char *argv[])
{
  if (argc <= 1)
  {
    panic("cp: need a file to copy");
  }
  if (argc <= 2)
  {
    panic("cp: no target file given");
  }
  return 0;
}
