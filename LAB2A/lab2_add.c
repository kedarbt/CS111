#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define _GNU_SOURCE

char syncf;
int num_iterations = 1;
long nano = 1000000000;
int opt_yield = 0;
pthread_mutex_t mutex;
int mutual_exc = 0;

void cmpadd(long long *pointer, long long value)
{
	long long store;
	long long sum;
	do
	{
		store = *pointer;
		sum = store + value;
		if(opt_yield) { sched_yield(); }
	} while(__sync_val_compare_and_swap(pointer, store, sum) != store);
}

void add(long long *pointer, long long value)
{
	long long sum = *pointer + value;
	if(opt_yield) { sched_yield(); }
	*pointer = sum;
}

void * add_thread(void* counter)
{
	// increment counter
	for(int i = 0; i < num_iterations; i++)
	{
		if(syncf == 'm')
		{
			pthread_mutex_lock(&mutex);
			add((long long*) counter, 1);
			pthread_mutex_unlock(&mutex);
		} 
		else if (syncf == 's')
		{
			while(__sync_lock_test_and_set(&mutual_exc, 1));
			add((long long *) counter, 1);
			__sync_lock_release(&mutual_exc);
		} 
		else if (syncf == 'c')
		{
			cmpadd((long long*) counter, 1);
		} 
		else
		{
			add((long long*) counter, 1);
		}
	}
	//decrement counter
	for(int j = 0; j < num_iterations; j++)
	{
		
		if(syncf == 'm')
		{
			pthread_mutex_lock(&mutex);
			add((long long*) counter, -1);
			pthread_mutex_unlock(&mutex);
		} 
		else if (syncf == 's')
		{
			while(__sync_lock_test_and_set(&mutual_exc, 1));
			add((long long *) counter, -1);
			__sync_lock_release(&mutual_exc);
		}
		else if (syncf == 'c')
		{
			cmpadd((long long*) counter, -1);
		}
		else
		{
			add((long long*) counter, -1);
		}
	}
	return NULL;
}


int main(int argc, char ** argv)
{
	int option = 0;
	int yieldf = 0, csynchf = 0, mutf = 0, spinf = 0;
	int num_threads = 1;
	char string[100];
	struct timespec start;
	struct timespec end;
	static struct option long_opts[] =
	{
		{"threads", required_argument, 0, 't'},
		{"iterations", required_argument, 0, 'i'},
		{"yield", no_argument, 0, 'y'},
		{"sync", required_argument, 0, 's'}
	};


	while((option = getopt_long(argc, argv, "t:i:", long_opts, NULL)) != -1)
	{
		switch(option)
		{
			case 't':
				num_threads = atoi(optarg);
				break;
			case 'i':
				num_iterations = atoi(optarg);
				break;
			case 'y':
				opt_yield = 1; yieldf = 1;
				break;
			case 's':
				syncf = optarg[0];
				if(optarg[0] == 'm'){ mutf = 1;}
				if(optarg[0] == 's'){ spinf = 1;}
				if(optarg[0] == 'c'){ csynchf = 1;}
				break;
			default:
				perror("Default number of threads and iterations is 1");
				exit(1);
				break;
		}

	}

	if(!yieldf && !csynchf && !mutf && !spinf){ strcpy(string,"add-none");}
	else if(mutf && !yieldf && !csynchf && !spinf){ strcpy(string, "add-m");}
	else if(!yieldf && !csynchf && !mutf && spinf){ strcpy(string, "add-s");}
	else if(!yieldf && csynchf && !mutf && !spinf){	strcpy(string,"add-c");}
	else if(yieldf && !csynchf && !mutf && !spinf){	strcpy(string,"add-yield-none");}
	else if(yieldf && !csynchf && mutf && !spinf){ strcpy(string,"add-yield-m");}
	else if(yieldf && !csynchf && !mutf && spinf){ strcpy(string,"add-yield-s");}
	else if(yieldf && csynchf && !mutf && !spinf){ strcpy(string, "add-yield-c"); }

	char yieldstr[6] = "none";
	char syncstr[6] = "none";
	if(yieldf)
	{
		strcpy(yieldstr, "yield");
	}
	if(csynchf)
	{
		strcpy(syncstr, "c");
	}
	if(spinf)
	{
		strcpy(syncstr, "s");
	}
	if(mutf)
	{
		strcpy(syncstr, "m");
	}

	if(mutf)
	{
		pthread_mutex_init(&mutex, NULL);
	}

	pthread_t * threads = malloc(num_threads * sizeof(pthread_t));
	long long counter = 0;
	if(clock_gettime(CLOCK_MONOTONIC, &start) == -1)
	{
		perror("Could not start timer");
		exit(1);
	}

	for(int i = 0; i < num_threads; i++)
	{
		int status = pthread_create(threads + i, NULL, add_thread, &counter);
		if(status != 0)
		{
			perror("Error creating threads"); exit(1);
		}
	}
	for(int i = 0; i < num_threads; i++)
	{
		int status = pthread_join(*(threads + i), NULL);
		if(status != 0)
		{
			perror("Error with thread"); exit(1);
		}
	}

	if(clock_gettime(CLOCK_MONOTONIC, &end) == -1)
	{
		perror("Error on ending timer"); exit(1);
	}

	long timer = nano*(end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec);
	long ops = num_iterations * num_threads * 2;
	long tpo = timer/ops;
	if(!yieldf && !mutf && !csynchf && !spinf)
	{
		fprintf(stdout,"add-none,%d,%d,%d,%d,%d,%d\n",num_threads, num_iterations, ops, timer, tpo, counter);
	}
	if(yieldf)
	{
		fprintf(stdout,"add-yield-%s,%d,%d,%d,%d,%d,%d\n", syncstr, num_threads, num_iterations, ops, timer, tpo, counter);
	}
	if(!yieldf && (csynchf || mutf || spinf))
	{
		fprintf(stdout,"add-%s,%d,%d,%d,%d,%d,%d\n", syncstr, num_threads, num_iterations, ops, timer, tpo, counter);
	}
	// fprintf(stdout, "%s, %d, %d, %d, %d, %d, %d\n", string, num_threads, num_iterations, ops, timer, tpo, counter);
	
	if(counter == 0){ exit(0);}
	else{ exit(2);}
}