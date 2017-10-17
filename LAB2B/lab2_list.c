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
long lock_time = 0;
long lock_ops;
int * hashvalues; // array to store hash values
int num_threads = 1;
char s;
int opt_yield = 0;
long convert = 1000000000;
char yieldout[5];
pthread_mutex_t * mutex;
int num_lists = 1;
SortedList_t * list; SortedListElement_t * elements;
int  * mutual_exc;
int numelements = 0;
int len = 0;
int ins = 0,del = 0,look = 0;

void keygen()
{
		
	srand(time(NULL)); // random number generator

	for(int t = 0; t < numelements; t++)
	{
		
		int length = rand()%10 + 5; 
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
unsigned long hash(const char * key) 
{
	unsigned long listid = 1;
	for(int i = 0; key[i] != '\0'; i++)
	{
		listid += key[i] - 'a';
	}
	return listid % num_lists;
}

void length_check()
{
	for(int i = 0; i < num_lists; i++)
	{
		len += SortedList_length(&list[i]);
	}
}

void locks()
{
	if(s == 's')
	{
		mutual_exc = malloc(num_lists * sizeof(int));
		for(int i = 0; i < num_lists; i++)
		{
			mutual_exc[i] = 0;
		}
	}
	if(s == 'm')
	{
		mutex = malloc(num_lists * sizeof(pthread_mutex_t));
		for(int i = 0; i < num_lists; i++)
		{
			pthread_mutex_init(&mutex[i], NULL);
		}
	}

}
void * threadedlist(void * thread)
{
	for(int i = *(int *) thread; i < num_iterations * num_threads; i += num_threads)
	{
		struct timespec s1, e1;
		if(s == 'm')
		{
			int success = clock_gettime(CLOCK_MONOTONIC, &s1);
			pthread_mutex_lock(&mutex[hashvalues[i]]);
			success = clock_gettime(CLOCK_MONOTONIC, &e1);
			lock_ops++;
			lock_time += convert*(e1.tv_sec - s1.tv_sec) + (e1.tv_nsec - s1.tv_nsec);
			SortedList_insert(&list[hashvalues[i]], &elements[i]);
			pthread_mutex_unlock(&mutex[hashvalues[i]]);
		}
		else if(s == 's')
		{
			int success = clock_gettime(CLOCK_MONOTONIC, &s1);
			while(__sync_lock_test_and_set(&mutual_exc[hashvalues[i]], 1));
			success = clock_gettime(CLOCK_MONOTONIC, &e1);
			lock_ops++;
			lock_time += convert*(e1.tv_sec - s1.tv_sec) + (e1.tv_nsec - s1.tv_nsec);
			SortedList_insert(&list[hashvalues[i]],&elements[i]);
			__sync_lock_release(&mutual_exc[hashvalues[i]]);
		}
		else
		{
			SortedList_insert(&list[hashvalues[i]], &elements[i]);
		}

	}

	for(int k = *(int *) thread; k < num_iterations * num_threads; k += num_threads)
	{
		struct timespec s1, e2;
		SortedListElement_t * obj;
		if(s == 'm')
		{
			pthread_mutex_lock(&mutex[hashvalues[k]]);
			obj = SortedList_lookup(&list[hashvalues[k]], elements[k].key);
			SortedList_delete(obj);
			pthread_mutex_unlock(&mutex[hashvalues[k]]);
		}
		else if(s == 's')
		{
			while(__sync_lock_test_and_set(&mutual_exc[hashvalues[k]], 1));
			obj = SortedList_lookup(&list[hashvalues[k]], elements[k].key);
			SortedList_delete(obj);
			__sync_lock_release(&mutual_exc[hashvalues[k]]);
		}
		else
		{
			obj = SortedList_lookup(&list[hashvalues[k]], elements[k].key);
			SortedList_delete(obj);
		}

	}

}
void get_yield_opts(char * opts)
{
	
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
		{"sync", required_argument, 0, 's'},
		{"lists", required_argument, 0, 'l'}
	};


	while((option = getopt_long(argc, argv, "t:i:y:s:l", long_opts, NULL)) != -1)
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
			case 'l':
				num_lists = atoi(optarg);
				break;
			default:
				perror("Default number of threads and iterations is 1");
				exit(1);
				break;
		}

	}

	list = malloc(num_lists * sizeof(SortedList_t)); // allocate list
	for(int i = 0; i < num_lists; i++)
	{
		list[i].next = &list[i];
		list[i].prev = &list[i];
		list[i].key = NULL;
	}

	elements = malloc(num_iterations * num_threads * sizeof(SortedListElement_t));
	numelements = num_iterations * num_threads;
	keygen();
	locks();
	hashvalues = malloc(numelements * sizeof(int));
	for(int i = 0; i < numelements; i++)
	{
		hashvalues[i] = hash(elements[i].key);
	} 	
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



	long timer = convert * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
	int ops = num_threads * num_iterations * 3;
	long tpo = timer/ops;
	if(mutf || spinf)
	{
		lock_time = lock_time/lock_ops;
	}
	fprintf(stdout, "list-%s-%s,%d,%d,%d,%d,%d,%d,%d\n",yieldout,syncout,num_threads,num_iterations,num_lists,ops,timer,tpo, lock_time);
	length_check();
	free(IDs);
	free(elements);
	free(threads);
	free(hashvalues);
	free(mutex);
	free(mutual_exc);
	free(list);
	if(len != 0)
	{
		 exit(2);
	}
	exit(0);

}