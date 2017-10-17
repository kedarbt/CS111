#include <stdio.h>
#include <getopt.h>
#include <pthread.h>
#include <string.h>
#include "SortedList.h"
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#define _GNU_SOURCE


int num_iterations = 1;
int num_threads = 1;
char s;
int opt_yield = 0;
long convert = 1000000000;
char yieldout[5];
pthread_mutex_t mutex;
int num_lists = 1;
SortedList_t * list; SortedListElement_t * elements;
int mutual_exc = 0;
int numelements = 0;
int len = 0;
int ins = 0,del = 0,look = 0;

void keygen()
{
		
	srand(time(NULL)); // random number generator

	for(int t = 0; t < numelements; t++)
	{
		
		int length = 10; 
		int r = rand() % 26;
		char* key = malloc((length+1) * sizeof(char));
		for(int i = 0; i < length; i++)
		{
			key[i] = 'a' + r;
			r = rand() % 26;
		}
		key[length] = '\0';	
		elements[t].key = key;
	}

}

void * threadedlist(void * thread)
{
	for(int i = *(int *) thread; i < num_iterations; i += num_threads)
	{
		if(s == 'm')
		{
			pthread_mutex_lock(&mutex);
			SortedList_insert(list, &elements[i]);
			pthread_mutex_unlock(&mutex);
		}
		else if(s == 's')
		{
			while(__sync_lock_test_and_set(&mutual_exc, 1));
			SortedList_insert(list,&elements[i]);
			__sync_lock_release(&mutual_exc);
		}
		else
		{
			SortedList_insert(list, &elements[i]);
		}

	}

	for(int k = *(int *) thread; k < num_iterations; k += num_threads)
	{
		SortedListElement_t * obj;
		if(s == 'm')
		{
			pthread_mutex_lock(&mutex);
			obj = SortedList_lookup(list, elements[k].key);
			SortedList_delete(obj);
			pthread_mutex_unlock(&mutex);
		}
		else if(s == 's')
		{
			while(__sync_lock_test_and_set(&mutual_exc, 1));
			obj = SortedList_lookup(list, elements[k].key);
			SortedList_delete(obj);
			__sync_lock_release(&mutual_exc);
		}
		else
		{
			obj = SortedList_lookup(list, elements[k].key);
			SortedList_delete(obj);
		}

	}

}
void get_yield_opts(char * opts)
{
	int prevopt = opt_yield;
	
	for(int i = 0; *(opts + i) != '\0'; i++)
	{
		char c = *(opts + i);
		switch(c)
		{
			case 'i':
				opt_yield = opt_yield|INSERT_YIELD;
				ins = 1;
				break;
			case 'd':
				opt_yield = opt_yield|DELETE_YIELD;
				del = 1;
				break;
			case 'l':
				opt_yield = opt_yield|LOOKUP_YIELD;
				look = 1;
				break;
			default:
				perror("Wrong yield argument");
				exit(1);
		}

	}

	if(ins && !del && !look){ strcpy(yieldout, "i"); }
	if(!ins && del && !look){ strcpy(yieldout, "d"); }
	if(!ins && !del && look){ strcpy(yieldout, "l"); }
	if(ins && del && !look){ strcpy(yieldout, "id");}
	if(ins && !del && look){ strcpy(yieldout, "il");}
	if(!ins && del && look){ strcpy(yieldout, "dl"); }
	if(ins && del && look){ strcpy(yieldout, "idl"); }
}



int main(int argc, char ** argv)
{
	
	int yieldf = 0, csynchf = 0, mutf = 0, spinf = 0, option = 0;
	char syncout[5] = "none";
	strcpy(yieldout, "none");
	struct timespec start, end;

	static struct option long_opts[] =
	{
		{"threads", required_argument, 0, 't'},
		{"iterations", required_argument, 0, 'i'},
		{"yield", required_argument, 0, 'y'},
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
				get_yield_opts(optarg); yieldf = 1;
				break;
			case 's':
				if(optarg[0] == 'm'){ mutf = 1; strcpy(syncout,"m"); s = 'm';}
				if(optarg[0] == 's'){ spinf = 1; strcpy(syncout, "s"); s = 's';}
				break;
			default:
				perror("Default number of threads and iterations is 1");
				exit(1);
				break;
		}

	}

	if(mutf){ pthread_mutex_init(&mutex, NULL);} // initialize mutex

	list = malloc(sizeof(SortedList_t)); // allocate list
	list->key = NULL;
	list->prev = list; list->next = list;

	elements = malloc(num_iterations * num_threads * sizeof(SortedListElement_t));
	numelements = num_iterations * num_threads;
	keygen();
	pthread_t * threads = malloc(num_threads*sizeof(pthread_t));
	int * IDs = malloc(num_threads * sizeof(int));

	int success = clock_gettime(CLOCK_MONOTONIC, &start);
	if(success == -1)
	{
		perror("Could not start timer"); exit(1);
	}
	for(int i = 0; i < num_threads; i++)
	{
		IDs[i] = i;
		int status = pthread_create(threads + i, NULL, threadedlist, &IDs[i]);
		if(status) { perror("Error creating threads");}
	}
	for(int i = 0; i < num_threads; i++)
	{
		int status = pthread_join(threads[i], NULL);
		if(status) { perror("Could not create threads");}
	}
	success = clock_gettime(CLOCK_MONOTONIC, &end);
	if(success == -1){ perror("Timer error."); exit(1);}

	free(IDs);
	free(elements);
	free(threads);

	long timer = convert * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
	int ops = num_threads * num_iterations * 3;
	long tpo = timer/ops;
	fprintf(stdout, "list-%s-%s,%d,%d,1,%d,%d,%d\n",yieldout,syncout,num_threads,num_iterations,ops,timer,tpo);
	if(len != 0)
	{
		perror("The list was not totally empty at exit."); exit(2);
	}
	exit(0);
}