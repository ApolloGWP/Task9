#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#define TIME 5
#define WRITER 5
#define READER 10

int *global;
pthread_rwlock_t rwlock;

void *print(void *arg){
  int size = *(int*) arg;
  int i;
  time_t t = time(NULL) + TIME;
  while (t > time(NULL)) {
    for (i = 0; i < size; i++){
      printf("%d ", global[i]);
    }  
    printf("\n");
    sleep(1);
  }
  pthread_exit(NULL);
}

void *GlWrite(void *arg){
  int n = *(int*) arg;
  time_t t = time(NULL) + TIME;
  while (t > time(NULL)){
    pthread_rwlock_wrlock(&rwlock);
    srand(time(NULL));
    int r = rand() % n;
    int value = (rand() % 10) + 1;
    if (global[r] == 0){
      global[r] = value;
      printf("Write: %d\n", global[r]);  
    }
    pthread_rwlock_unlock(&rwlock);
    sleep(1);
  }
}

void *GlRead(void *arg){
  int n = *(int*) arg;
  time_t t = time(NULL) + TIME;
  while (t > time(NULL)){
    sleep(3);
    pthread_rwlock_rdlock(&rwlock);
    srand(time(NULL));
    int r = rand() % n;
    if(global[r] != 0){
      printf("Read: %d\n",global[r]);
      global[r] = 0;  
    }
    pthread_rwlock_unlock(&rwlock);
  } 
}

int main(int argc, char **argv){
  char *opts = "e:";
  int n = 10;
  int opt;
  while ((opt = getopt(argc, argv, opts)) != -1){
    switch(opt){
      case 'e':
        n = atoi(optarg);
        break;
      default: {
        printf("Неизвестная команда...\n");
        break;
      };     
    }
  }

  global = malloc(sizeof(int) * n);

  pthread_attr_t attr;
  pthread_t pt;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  pthread_create(&pt, &attr, &print, &n);
  pthread_attr_destroy(&attr);
  
  pthread_t *wr, *rd; 
  wr = malloc(sizeof(pthread_t) * WRITER);
  rd = malloc(sizeof(pthread_t) * READER);

  pthread_rwlock_init(&rwlock, NULL);

  int i;
  for (i = 0; i < WRITER; i++){
    if (pthread_create(&wr[i], NULL, &GlWrite, &n) != 0) {
        fprintf(stderr, "Error (thread)\n");
        return 1;
      }
  }

  for (i = 0; i < READER; i++){
    if (pthread_create(&rd[i], NULL, &GlRead, &n) != 0) {
        fprintf(stderr, "Error (thread)\n");
        return 1;
      }
  }

  for(i = 0; i < WRITER; ++i){
    if (pthread_join(wr[i], NULL) != 0) {
      printf("Error\n");   
    }
  }

  for(i = 0; i < READER; ++i){
    if (pthread_join(rd[i], NULL) != 0) {
      printf("Error\n");   
    }
  }

  for (i = 0; i < n; i++){
    printf("%d ", global[i]);
  }
  printf("\n");
  
  pthread_rwlock_destroy(&rwlock);
  free(global);
  free(wr);
  free(rd);
  return 0;
}
