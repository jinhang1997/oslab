#define DEBUG
#include "thread.h"

#define BUFFER_SIZE 4
#define ITEM_COUNT 8

typedef struct {
  int value;  // value of semaphore
  pthread_mutex_t mutex;  // mutex 
  pthread_cond_t cond;  // condition
} sema_t;

void sema_init(sema_t *sema, int value)
{
  // initialize sema value with value given
  sema->value = value;
  // initialize the mutex in sema
  pthread_mutex_init(&sema->mutex, NULL);
  // initialize the condition in sema
  pthread_cond_init(&sema->cond, NULL);
}

void sema_wait(sema_t *sema)
{
  // lock the mutex
  pthread_mutex_lock(&sema->mutex);
  // waiting for the semaphore
  while (sema->value <= 0)
  {
    pthread_cond_wait(&sema->cond, &sema->mutex);
  }
  // take out 1 semaphore after waiting
  sema->value--;
  // unlock the mutex
  pthread_mutex_unlock(&sema->mutex); 
}

void sema_signal(sema_t *sema)
{
  // lock the mutex
  pthread_mutex_lock(&sema->mutex);
  // add 1 semaphore after waiting
  sema->value++;
  // give signal to the condition 
  pthread_cond_signal(&sema->cond);
  // unlock the mutex
  pthread_mutex_unlock(&sema->mutex); 
}

char buffer1[BUFFER_SIZE];
int buf1in = 0, buf1out = 0;
sema_t buf1_mutex_sema;
sema_t buf1_empty_sema;
sema_t buf1_full_sema;

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
sema_t buf2_mutex_sema;
sema_t buf2_empty_sema;
sema_t buf2_full_sema;

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
    // wait for the empty semaphore first
    sema_wait(&buf1_empty_sema);
    // then wait for the mutex semaphore
    sema_wait(&buf1_mutex_sema);
    // put an item into the buffer
    item = 'a' + i;
    buf1_put_item(item);
    printf("/// \033[;31mproducer put an item: [%c]\033[0m\n", item);
    // give signal to mutex semaphore
    sema_signal(&buf1_mutex_sema);
    // give signal to full semaphore
    sema_signal(&buf1_full_sema);
  }
  
  return NULL;
}

void *process(void *targ)
{
  int i;
  int item;
  
  for (i = 0; i < ITEM_COUNT; i++)
  {
    // wait for the full semaphore first
    sema_wait(&buf1_full_sema);
    // then wait for the mutex semaphore
    sema_wait(&buf1_mutex_sema);
    // take out an item from the buffer
    item = buf1_get_item(); 
    printf("*** \033[;32mprocessor got an item: [%c]\033[0m\n", item);
    // give signal to mutex semaphore
    sema_signal(&buf1_mutex_sema);
    // give signal to empty semaphore
    sema_signal(&buf1_empty_sema);
    
    // now convert the item and put it into the buffer
    item -= 0x20;
    
    // wait for the empty semaphore first
    sema_wait(&buf2_empty_sema);
    // then wait for the mutex semaphore
    sema_wait(&buf2_mutex_sema);
    // put an item into the buffer
    buf2_put_item(item);
    printf("--- \033[;33mprocessor put an item: [%c]\033[0m\n", item);
    // give signal to mutex semaphore
    sema_signal(&buf2_mutex_sema);
    // give signal to full semaphore
    sema_signal(&buf2_full_sema);
  }
  
  return NULL;
}

void *consum(void *targ)
{
  int i;
  int item;
  
  for (i = 0; i < ITEM_COUNT; i++)
  {
    // wait for the full semaphore first
    sema_wait(&buf2_full_sema);
    // then wait for the mutex semaphore
    sema_wait(&buf2_mutex_sema);
    // take out an item from the buffer
    item = buf2_get_item();
    printf("\033[1;34m+++ consumer got an item: [%c]\033[0m\n", item);
    // give signal to mutex semaphore
    sema_signal(&buf2_mutex_sema);
    // give signal to empty semaphore
    sema_signal(&buf2_empty_sema); 
  }
  
  return NULL;
}

int main()
{
  pthread_t processor_tid;
  pthread_t consumer_tid;
  
  sema_init(&buf1_mutex_sema, 1);
  sema_init(&buf1_empty_sema, BUFFER_SIZE - 1);
  sema_init(&buf1_full_sema, 0);
  sema_init(&buf2_mutex_sema, 1);
  sema_init(&buf2_empty_sema, BUFFER_SIZE - 1);
  sema_init(&buf2_full_sema, 0);
  
  pthread_create(&processor_tid, NULL, &process, NULL);
  pthread_create(&consumer_tid, NULL, &consum, NULL);
  produce(NULL);
  pthread_join(processor_tid, NULL);
  pthread_join(consumer_tid, NULL);
  
  return 0;
}

