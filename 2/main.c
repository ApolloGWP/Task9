#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>

#define A -2.0
#define B 2.0

pthread_mutex_t mutex;
double result = 0;
double h;

double CalcFunction(double x){
	return pow(cos(x), 2) + pow(cos(x), 2);
}

void *CalcIntegral(void * arg){
	int i = *(int*) arg;	
	pthread_mutex_lock(&mutex);
	result += CalcFunction(A + h*(i + 0.5));
	pthread_mutex_unlock(&mutex);
}

int main(int argc, char **argv){
	int p = 10;
	pthread_t *pt;
	if (argc > 1){
		p = atoi(argv[1]);
	}
	
	pt = malloc(sizeof(pthread_t) * p);
	pthread_mutex_init(&mutex, NULL);

	h = (B-A)/p;

	int i;
	for (i = 0; i < p; i++){
		if(pthread_create(&pt[i], NULL, &CalcIntegral, &i) !=0){
			fprintf(stderr, "Error (thread)\n");
			return 1;
		}		
	}

	for (i = 0; i < p; i++){
		if (pthread_join(pt[i], NULL) != 0) {
			printf("Error.\n");
			return 1;		
		}
	}

	printf("Значение интеграла: %f\n", result * h);

	pthread_mutex_destroy(&mutex);
	free(pt);
	return 0;
}

