#include "thread.h"
#include <time.h>

typedef struct {
  unsigned int start_idx;
  unsigned int end_idx;
  char tname[10];
} calcpart_arg_t;

typedef struct {
  double result;
  double timespan;
} calcpart_ret_t;

#define NR_CPUS 100

void *calcpart(void *arg)
{
  clock_t start, end;
  unsigned int i, denominator, start_idx, end_idx;
  double item, result;
  calcpart_arg_t *carg = (calcpart_arg_t *)arg;
  calcpart_ret_t *ret = malloc(sizeof(calcpart_ret_t));
  
  printf("thread %s: start calculating idx %u to %u.\n",
    carg->tname, carg->start_idx, carg->end_idx);
  start = clock();
  result = 0.0;
  start_idx = carg->start_idx;
  end_idx = carg->end_idx;
  for (i = start_idx; i <= end_idx; i += 1u)
  {
    denominator = i * 2 - 1;
    item = 1.0 / denominator; 
    result += (i % 2) ? item : -item;
#ifdef DEBUG    
    //log("i = %d, denominator = %d, item = %lf, result now: %lf", i, denominator, item, result);
#endif
  }
  ret->result = result;
  
  end = clock();
  ret->timespan = (double)(end - start) / CLOCKS_PER_SEC;
  
  printf("thread %s: ended idx %u to %u in %lf seconds.\n",
    carg->tname, carg->start_idx, carg->end_idx, ret->timespan);
  printf("thread %s: PI/4 = %.12lf, PI = %.12lf\n",
    carg->tname, ret->result, ret->result * 4.0);
  
  return (void *)ret;
}

int main(int argc, char *argv[])
{
  pthread_t workers[NR_CPUS];  // 0 is not used
  int i ;  
  unsigned int n = 5u;  // n threads
  unsigned int max_idx = 10u;
  calcpart_arg_t part[NR_CPUS];
  double sum = 0.0;
  double time = 0.0;
  calcpart_ret_t *result = NULL;

  if (argc < 3
   || !sscanf(argv[1], "%u", &n) 
   || !sscanf(argv[2], "%u", &max_idx) 
   || max_idx >= 0x7fffffffu)
  {
    warning("Usage: pi2 num_of_threads num_of_items");
    warning("num_of_threads: should not larger chan %d", NR_CPUS);
    warning("num_of_items: should not larger chan %u", 0x7fffffffu);
    return 1;
  }
  printf("calculating index %u to %u, please wait...\n", 1, max_idx);
  part[0].start_idx = 1;
  part[0].end_idx = max_idx / n;
  strcpy(part[0].tname, "main");
  for (i = 1; i < n; i++)
  {
    part[i].start_idx = part[i - 1].end_idx + 1u;
    part[i].end_idx = part[i - 1].end_idx + max_idx / n;
    sprintf(part[i].tname, "ass%d", i);
    // creating assistant thread
    pthread_create(&workers[i], NULL, &calcpart, (void *)&part[i]);
  }
  
  // calculation in main thread
  result = (calcpart_ret_t *)calcpart((void *)&part[0]);
  sum = result->result;
  time = result->timespan;
  free(result);
  printf("calculation in main thread ended with %.12lf and start waiting...\n", result->result);
  
  for (i = 1; i < n; i++)
  {
    pthread_join(workers[i], (void **)&result);
    printf("workers[%d] joined here with %.12lf.\n", i, result->result);
    sum += result->result;
    time += result->timespan;
    free(result);
  }
  
  time /= n;
  printf("all threads joined successfully. Average time: %lf seconds\n", time);
  printf("PI/4 = %.12lf\nPI   = %.12lf\n", sum, sum * 4.0);
  
  return 0;
}
