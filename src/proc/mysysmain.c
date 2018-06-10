#include "proc.h"

int main(int argc, char *argv[])
{
  int ret;
  
  printf("------------------------------\n");
  ret = mysys("echo HELLO WORLD");
  printf("---------------\n");
  printf("Process exited with return value %d.\n", ret);
  printf("------------------------------\n");
  ret = mysys("ls /");
  printf("---------------\n");
  printf("Process exited with return value %d.\n", ret);
  printf("------------------------------\n");
  /*ret = mysys("~/ret1");
  printf("---------------\n");
  printf("Process exited with return value %d\n", ret);
  printf("------------------------------\n");*/
  
  return 0;
}
