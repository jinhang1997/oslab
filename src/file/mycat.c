#include "common.h"

char buf[512];

void cat(int fd)
{
  int n;
  
  while ((n = read(fd, (void *)buf, sizeof(buf))) > 0)
  {
    write(1, buf, n);
  }
  if (n < 0)
  {
    panic("cat: read error");
  }
}

int main(int argc, char *argv[])
{
  int fd, i;
  
  if (argc <= 1)
  {
    cat(0);
    exit(0);
  }
  
  for (i = 1; i < argc; i++)
  {
    if ((fd = open(argv[i], O_RDONLY)) < 0)
    {
      panic("cat: cannot open %s", argv[i]);
    }
    cat(fd);
    close(fd);
  }
  
  return 0;
}
