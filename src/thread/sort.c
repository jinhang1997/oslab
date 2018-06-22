//#define DEBUG

#include "thread.h"
#include <time.h>

typedef struct {
  int start_idx;
  int end_idx;
  char tname[10];
} sortpart_arg_t;

typedef struct {
  double timespan;
} sortpart_ret_t;

#define MAX_NUMS 1000
#define MAX_NUM 1000

int nums[MAX_NUMS];
int temp[MAX_NUMS];

void print_nums(int start, int end)
{
  int i;
  for (i = start; i <= end; i++)
  {   
    if ((i + 1) % 16 == 0)
    {
      printf("%5d\n", nums[i]);
    }
    else
    {
      printf("%5d", nums[i]);
    }
  }
  printf("\n");
}

void *sortpart(void *args)
{
  sortpart_arg_t *arg = (sortpart_arg_t *)args;
  sortpart_ret_t *ret = malloc(sizeof(sortpart_ret_t));
  int i, j;
  int flag_min, min, temp;
  clock_t start, end;
  
  printf("thread %s: start sorting index %d to %d.\n", arg->tname, arg->start_idx, arg->end_idx);
  start = clock();
  for (i = arg->start_idx; i <= arg->end_idx; i++)
  {
    min = nums[i];
    flag_min = i;
    for (j = i; j <= arg->end_idx; j++)
    {
      if (nums[j] < min)
      {
        min = nums[j];
        flag_min = j;
      }
    }
    temp = nums[flag_min];
    nums[flag_min] = nums[i];
    nums[i] = temp;
  }
  end = clock();
  ret->timespan = (double)(end - start) / CLOCKS_PER_SEC;
  
  printf("thread %s: finished sorting index %d to %d in %lf seconds.\n",
    arg->tname, arg->start_idx, arg->end_idx, ret->timespan);
#ifdef DEBUG    
  log("thread %s: they are: ", arg->tname);
  print_nums(arg->start_idx, arg->end_idx);
#endif  
  
  return (void *)ret;
}

void init_nums(int n)
{
  int i;
  srand(time(NULL));
  printf("initial nums to be sorted:\n");
  for (i = 0; i < n; i++)
  {
    nums[i] = rand() % MAX_NUM;
  }
  print_nums(0, n - 1);
}

void mergearray(int start, int mid, int end)
{
  int i = start, j = mid + 1;
  int k = 0;
  
  while (i <= mid && j <= end)
  {
    if (nums[i] < nums[j])
    {
      temp[k++] = nums[i++];
    }
    else
    {
      temp[k++] = nums[j++];
    }
  }
  
  while (i <= mid - 1)
  {
    temp[k++] = nums[i++];
  }
  
  while (j <= end)
  {
    temp[k++] = nums[j++];
  }
  
  for (i = 0; i < k; i++)
  {
    nums[start + i] = temp[i];
  }
}

void mergesort(int start, int end)
{
  int mid;
  if (start < end)
  {
    mid = (start + end) / 2;
#ifdef DEBUG
    log("start = %d, mid = %d, end = %d", start, mid, end);
#endif
    mergesort(start, mid);
    mergesort(mid + 1, end);
    mergearray(start, mid, end);
  }
}

int main(int argc, char *argv[])
{
  int n;
  clock_t start, end;
  pthread_t worker_tid;
  sortpart_arg_t arg, argmain;
  sortpart_ret_t *result;
  
  if (argc < 2 || sscanf(argv[1], "%d", &n) < 1 || n > MAX_NUMS)
  {
    warning("usage: sort nums_to_be_sorted");
    warning("nums_to_be_sorted: not larger than %d.", MAX_NUMS);
    return 1;
  }
  
  init_nums(n);
  
  arg.start_idx = n / 2;
  arg.end_idx = n - 1;
  strcpy(arg.tname, "assistant");
  pthread_create(&worker_tid, NULL, &sortpart, (void *)&arg);
  
  argmain.start_idx = 0;
  argmain.end_idx = n / 2 - 1;
  strcpy(argmain.tname, "main");
  sortpart(&argmain);

  pthread_join(worker_tid, (void **)&result);
  printf("All threads joined here. They are: \n");
  print_nums(0, n - 1);
  
  printf("sorting by mergesort...\n");
  start = clock();
  mergesort(0, n - 1);
  end = clock();
  printf("mergesort finished in %lf seconds. result:\n", (double)(end - start) / CLOCKS_PER_SEC);
  print_nums(0, n - 1);

  return 0;
}
