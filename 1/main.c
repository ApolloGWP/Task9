#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int value = 0;

void * incrementI(void * arg) { 
	 int val = *(int*) arg;
     int i;
     for (i = 0; i < val; i++)
     	value++;	
     //printf("%d\n", value);
     srand(time(NULL));
     int r = rand() % 5 + 1;
     sleep(r);
     pthread_exit(NULL);
}

void * decrementI(void * arg) { 
	 int val = *(int*) arg;
     int i;
     for (i = 0; i < val; i++)
     	value--;	
     //printf("%d\n", value);
     srand(time(NULL));
     int r = rand() % 5 + 1;
     sleep(r);
     pthread_exit(NULL);
}

int main(int argc, char **argv){
	pthread_t *pt1, *pt2;
	int th = 10;
	int val = 3;
	char *opts = "t:v:"; 
	int opt; 
	while((opt = getopt(argc, argv, opts)) != -1) {
		switch(opt) {
			case 't': 
				th = atoi(optarg);
				break;
			case 'v': 
				val = atoi(optarg);
				break;
			case '?': default: {
				printf("Неизвестная команда...\n");
				break;
			};		
		}
	}
	pt1 = malloc (sizeof(pthread_t) * th/2);
	pt2 = malloc (sizeof(pthread_t) * th/2);
	int i;
	for (i = 0; i < th/2; ++i){
		if (pthread_create(&pt1[i], NULL, &incrementI, &val) != 0) {
			fprintf(stderr, "Error (thread)\n");
			return 1;
		}
		if (pthread_create(&pt2[i], NULL, &decrementI, &val) != 0) {
			fprintf(stderr, "Error (thread)\n");
			return 1;
		}		
	}
		
	for(i = 0; i < th/2; ++i){
		if (pthread_join(pt1[i], NULL) != 0) {
			printf("Error.\n");		
		}
		if (pthread_join(pt2[i], NULL) != 0) {
			printf("Error.\n");		
		}
	}
	printf("Value: %d\n", value);
	free(pt1);
	free(pt2);
	return 0;
}
