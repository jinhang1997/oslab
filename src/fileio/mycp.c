#include "common.h"

char buf[512];

void cp(int fdi, int fdo)
{
  int n;
  
  while ((n = read(fdi, (void *)buf, sizeof(buf))) > 0)
  {
    write(fdo, buf, n);
  }
  if (n < 0)
  {
    panic("cp: read error");
  }
}

int get_stat(int fd)
{
  struct stat s;
  if (fstat(fd, &s) < 0)
  {
    panic("cp: failed to read mode of the source file.");
  }
  return s.st_mode;
}

int main(int argc, char *argv[])
{
  int fdi, fdo;
  int mode;
  
  if (argc <= 1)
  {
    panic("cp: need a file to copy");
  }
  if (argc <= 2)
  {
    panic("cp: no target file given");
  }
  if ((fdi = open(argv[1], O_RDONLY)) < 0)
  {
    sprintf(buf, "cat: cannot open %s", argv[1]);
    panic(buf);
  }
  mode = get_stat(fdi);
  if ((fdo = creat(argv[2], mode)) < 0)
  {
    sprintf(buf, "cat: cannot create %s for writing", argv[2]);
    panic(buf);
  }
  cp(fdi, fdo);
  
  return 0;
}
