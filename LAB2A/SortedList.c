#include "SortedList.h"
#include <pthread.h>
#include <string.h>

#define _GNU_SOURCE

void SortedList_insert(SortedList_t *list, SortedListElement_t *element)
{
	if(list == NULL || element == NULL) { return; }
	SortedListElement_t *insert = list->next;
	while(insert != list)
	{
		if(strcmp(element->key, insert->key) <= 0)
			break;
		insert  = insert->next;
	}
	if(opt_yield & INSERT_YIELD)
		sched_yield();
	element->next = insert;
	element->prev = insert->prev;
	insert->prev->next = element;
	insert->prev = element;
}

int SortedList_delete(SortedListElement_t *element)
{
	if(element == NULL) { return 1; }
	if(element->next->prev == element->prev->next)
	{
		if(opt_yield & DELETE_YIELD)
			sched_yield();
		element->prev->next = element->next;
		element->next->prev = element->prev;
		return 0;
	}
	return 1;
}

SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key)
{
	if(list == NULL || key == NULL) { return NULL; }
	SortedListElement_t *look = list->next;
	while(look != list)
	{
		if(strcmp(look->key, key) == 0)
			return look;
		if(opt_yield & LOOKUP_YIELD)
			sched_yield();
		look = look->next;
	}
	return NULL;
}

int SortedList_length(SortedList_t *list)
{
	int count = 0;
	if(list == NULL) { return -1; }
	SortedListElement_t *look = list->next;
	while(look != list)
	{
		count++;
		if(opt_yield & LOOKUP_YIELD)
			sched_yield();
		look = look->next;
	}
	return count;
}