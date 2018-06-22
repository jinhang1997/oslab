#define DEBUG
#include "thread.h"

typedef struct {
  int value;
} ringarg_t;

#define MAX_THREAD 100
int n;
int count = 0;
pthread_t init_tid;

void *add1(void *arg)
{
  pthread_t tid;
  ringarg_t *rarg = (ringarg_t *)arg;
  int self_count;
  
  count++;
  self_count = count;
  printf("[t%d] thread %lu: %d is received.\n", self_count, pthread_self(), rarg->value);
  if (count < n)
  {
    rarg->value++;
    printf("[t%d] thread %lu: %d is sent.\n", self_count, pthread_self(), rarg->value);
    pthread_create(&tid, NULL, &add1, (void *)rarg);
    pthread_join(tid, NULL);
  }
  else
  {
    rarg->value++;
    printf("[t%d] thread %lu: %d is sent.\n", self_count, pthread_self(), rarg->value);
  }
  
  if (pthread_self() == init_tid)
  {
    printf("[t%d] thread %lu: %d is received.\n", self_count, pthread_self(), rarg->value);
  }
  
  return NULL;
}

int main(int argc, char *argv[])
{
  ringarg_t init_arg;
  
  init_arg.value = 0;
  n = 10;
  
  printf("main thread: send %d to t1\n", init_arg.value);
  pthread_create(&init_tid, NULL, &add1, (void *)&init_arg);
  pthread_join(init_tid, NULL);
  printf("all threads joined here.\n");
  
  return 0;
}
