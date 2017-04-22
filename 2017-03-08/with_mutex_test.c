#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>

#define NUM_THREADS 1000

/*
	@author Nikolay Bonev
	The program shows concurrency between threads with mutexes.
*/

//Declaring mutex
pthread_mutex_t lock;

//Shared variable between threads
int i = 0;

//Increment function. Changes the value of the shared variable 'i'
void increment(void)
{
	i++;
}

/*
	Thread function. 
	Calls one thousand times the increment function. Before incrementing check if the lock is free. If so enters the critical section and 	locks it.
*/
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
