#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 1000

int i;

void inc_i()
{
	i++;
}

void *inc_var()
{
	int j;

	for(j = 0; j < NUM_THREADS; j++)
	{
		inc_i();
	}
}

int main()
{
	pthread_t threads[NUM_THREADS];

	i = 0;

	int j;

	for(j = 0; j < NUM_THREADS; j++)
	{
		pthread_create(&threads[j], NULL, inc_var, NULL);
	}

	for(j = 0; j < NUM_THREADS; j++)
	{	
		pthread_join(threads[j], NULL);
	}
	
	printf("The result value of variable i is = %d\n", i);
}


