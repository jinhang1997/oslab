#define DEBUG
#include "thread.h"

#define BUFFER_SIZE 4
#define ITEM_COUNT 8

char buffer1[BUFFER_SIZE];
int buf1in = 0, buf1out = 0;
pthread_mutex_t mutex_buf1;
pthread_cond_t wait_empty_buf1;
pthread_cond_t wait_full_buf1;

int buf1_is_empty()
{
  return buf1in == buf1out;
}

int buf1_is_full()
{
  return (buf1in + 1) % BUFFER_SIZE == buf1out;
}

char buf1_get_item()
{
  char item;
  item = buffer1[buf1out];
  buf1out = (buf1out + 1) % BUFFER_SIZE;
  return item;
}

void buf1_put_item(char item)
{
  buffer1[buf1in] = item;
  buf1in = (buf1in + 1) % BUFFER_SIZE;
}

char buffer2[BUFFER_SIZE];
int buf2in = 0, buf2out = 0;
pthread_mutex_t mutex_buf2;
pthread_cond_t wait_empty_buf2;
pthread_cond_t wait_full_buf2;

int buf2_is_empty()
{
  return buf2in == buf2out;
}

int buf2_is_full()
{
  return (buf2in + 1) % BUFFER_SIZE == buf2out;
}

char buf2_get_item()
{
  char item;
  item = buffer2[buf2out];
  buf2out = (buf2out + 1) % BUFFER_SIZE;
  return item;
}

void buf2_put_item(char item)
{
  buffer2[buf2in] = item;
  buf2in = (buf2in + 1) % BUFFER_SIZE;
}

void *produce(void *targ)
{
  int i;
  int item;
  
  for (i = 0; i < ITEM_COUNT; i++)
  {
    // lock the mutex to ensure that only one thread  
    // is accessing the buffer at the same time
    pthread_mutex_lock(&mutex_buf1);
    // keep waiting until the buffer is has empty space
    while (buf1_is_full())
    {
      pthread_cond_wait(&wait_empty_buf1, &mutex_buf1);
    }
    // put an item into the buffer
    item = 'a' + i;
    buf1_put_item(item);
    printf("/// \033[;31mproducer put an item: [%c]\033[0m\n", item);
    // wake up the producer who is waiting for data
    // in the buffer and unlock the mutex
    pthread_cond_signal(&wait_full_buf1);
    pthread_mutex_unlock(&mutex_buf1);
  }
  
  return NULL;
}

void *process(void *targ)
{
  int i;
  int item;
  
  for (i = 0; i < ITEM_COUNT; i++)
  {
    // lock the mutex to ensure that only one thread  
    // is accessing the buffer at the same time
    pthread_mutex_lock(&mutex_buf1);
    // keep waiting until the buffer is not empty
    while (buf1_is_empty())
    {
      pthread_cond_wait(&wait_full_buf1, &mutex_buf1);
    }
    // take out an item from the buffer
    item = buf1_get_item(); 
    printf("*** \033[;32mprocessor got an item: [%c]\033[0m\n", item);
    // wake up the producer who is waiting for an empty
    // space in the buffer and unlock the mutex
    pthread_cond_signal(&wait_empty_buf1);
    pthread_mutex_unlock(&mutex_buf1); 
    
    // now convert the item and put it into the buffer
    item -= 0x20;
    
    // lock the mutex to ensure that only one thread  
    // is accessing the buffer at the same time
    pthread_mutex_lock(&mutex_buf2);
    // keep waiting until the buffer is has empty space
    while (buf2_is_full())
    {
      pthread_cond_wait(&wait_empty_buf2, &mutex_buf2);
    }
    // put an item into the buffer
    buf2_put_item(item);
    printf("--- \033[;33mprocessor put an item: [%c]\033[0m\n", item);
    // wake up the producer who is waiting for data
    // in the buffer and unlock the mutex
    pthread_cond_signal(&wait_full_buf2);
    pthread_mutex_unlock(&mutex_buf2);
  }
  
  return NULL;
}

void *consum(void *targ)
{
  int i;
  int item;
  
  for (i = 0; i < ITEM_COUNT; i++)
  {
    // lock the mutex to ensure that only one thread  
    // is accessing the buffer at the same time
    pthread_mutex_lock(&mutex_buf2);
    // keep waiting until the buffer is not empty
    while (buf2_is_empty())
    {
      pthread_cond_wait(&wait_full_buf2, &mutex_buf2);
    }
    // take out an item from the buffer
    item = buf2_get_item();
    printf("\033[1;34m+++ consumer got an item: [%c]\033[0m\n", item);
    // wake up the producer who is waiting for an empty
    // space in the buffer and unlock the mutex
    pthread_cond_signal(&wait_empty_buf2);
    pthread_mutex_unlock(&mutex_buf2);   
  }
  
  return NULL;
}

int main()
{
  pthread_t processor_tid;
  pthread_t consumer_tid;
  
  pthread_mutex_init(&mutex_buf1, NULL);
  pthread_cond_init(&wait_empty_buf1, NULL);
  pthread_cond_init(&wait_full_buf1, NULL);
  pthread_mutex_init(&mutex_buf2, NULL);
  pthread_cond_init(&wait_empty_buf2, NULL);
  pthread_cond_init(&wait_full_buf2, NULL);
  
  pthread_create(&processor_tid, NULL, &process, NULL);
  pthread_create(&consumer_tid, NULL, &consum, NULL);
  produce(NULL);
  pthread_join(processor_tid, NULL);
  pthread_join(consumer_tid, NULL);
  
  return 0;
}

