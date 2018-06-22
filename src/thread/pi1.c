#include "thread.h"
#include <time.h>

typedef struct {
  unsigned int start_idx;
  unsigned int end_idx;
  double result;
  double timespan;
  char *tname;
} calcpart_arg_t;

void *calcpart(void *arg)
{
  clock_t start, end;
  unsigned int i, denominator, start_idx, end_idx;
  double item, result;
  calcpart_arg_t *carg = (calcpart_arg_t *)arg;
  
  start = clock();
  result = 0.0;
  start_idx = carg->start_idx;
  end_idx = carg->end_idx;
  for (i = start_idx; i <= end_idx; i++)
  {
    denominator = i * 2 - 1;
    item = 1.0 / denominator; 
    result += (i % 2) ? item : -item;
#ifdef DEBUG    
    //log("i = %d, denominator = %d, item = %lf, result now: %lf", i, denominator, item, result);
#endif
  }
  carg->result = result;
  
  end = clock();
  carg->timespan = (double)(end - start) / CLOCKS_PER_SEC;
  
  printf("thread %s: idx %u to %u ended in %lf seconds.\n",
    carg->tname, carg->start_idx, carg->end_idx, carg->timespan);
  printf("thread %s: PI/4 = %.12lf, PI = %.12lf\n",
    carg->tname, carg->result, carg->result * 4.0);
  
  return NULL;
}

int main(int argc, char *argv[])
{
  pthread_t worker_tid;
  unsigned int mid = 100000000;
  calcpart_arg_t main_part, ass_part;
  
  main_part.start_idx = 1;
  main_part.end_idx = mid;
  main_part.tname = "main";
  ass_part.start_idx = mid + 1;
  ass_part.end_idx = mid * 2;
  ass_part.tname = "ass1";
  
  printf("calculating index %u to %u, please wait...\n", 1, mid * 2);
  
  // creating assistant thread
  pthread_create(&worker_tid, NULL, &calcpart, (void *)&ass_part);
  
  // calculation in main thread
  calcpart((void *)&main_part);
  
  pthread_join(worker_tid, NULL);
  
  printf("all threads joined here successfully.\n");
  printf("PI/4 = %.12lf\nPI   = %.12lf\n",
    main_part.result + ass_part.result, (main_part.result + ass_part.result) * 4.0);
  
  return 0;
}
