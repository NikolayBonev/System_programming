#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>

#define NUM_THREADS 1000

pthread_mutex_t lock;

int i = 0;

void increment(void)
{
	i++;
}

void *inc_var()
{
	int j;

	for(j = 0; j < NUM_THREADS ; j++)
	{
		pthread_mutex_lock(&lock);
		increment();
		pthread_mutex_unlock(&lock);
	}
}

int main()
{
	pthread_t threads[NUM_THREADS];

	int j;
	for(j = 0 ; j < NUM_THREADS ; j++)
	{
		pthread_create(&threads[j], NULL, inc_var, NULL);
	}

	for(j = 0; j < NUM_THREADS; j++)
	{	
		pthread_join(threads[j], NULL);
	}
	
	printf("The result value of variable i is = %d\n", i);
}
