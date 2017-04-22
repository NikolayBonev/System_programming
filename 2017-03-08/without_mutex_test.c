#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 1000

/*
	@author Nikolay Bonev
	The program shows concurrency between threads without mutexes.
*/

//Shared variable between threads
int i;

//Increment function. Changes the value of the shared variable 'i'
void increment(void)
{
	i++;
}

/*
	Thread function. 
	Calls one thousand times the increment function. All threads enters the critical section at ones and can't guaranteed that the result  value will be saved properly.
*/
void *inc_var()
{
	int j;

	for(j = 0; j < NUM_THREADS; j++)
	{
		increment();
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


