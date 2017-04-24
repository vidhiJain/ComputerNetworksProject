#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

unsigned long hashx(unsigned char *str)
{
    unsigned long hash = 5381;
    int i=0;
    int size = (int)strlen(str);
    for(i=0;i<size;i++)
    {
    	hash=hash*33+(str[i]);
    }
    return hash;
}

struct hashmap* hashmap_create(int size)
{
	struct hashmap* h = (struct hashmap*)malloc(sizeof(struct hashmap));
	h->table=(NODEPTR*)malloc(sizeof(NODEPTR)*size);
	int i;
	for(i=0;i<size;i++)
		h->table[i]=NULL;
	h->size=size;
	return h;
}

void hashmap_insert(struct hashmap *h,char * key, int val)
{
	unsigned long hashval = hashx(key)%(h->size);
	NODEPTR temp = (NODEPTR) malloc(sizeof(struct hash_node));
	temp->key = (char*)malloc(sizeof(char)*100);
	strcpy(temp->key,key);
	temp->data = val;
	temp->next = h->table[hashval];
	h->table[hashval] = temp;
}

int hashmap_search(struct hashmap *h, char * key){
	unsigned long hashval = hashx(key)%(h->size);
	NODEPTR temp = h->table[hashval];
	while (temp!=NULL){
		//printf("comparing %s %s",key, temp->key);
		if (strcmp(key, temp->key)==0)
			return temp->data;
		temp = temp->next;
	}
	return -1;
}

void hashmap_print(struct hashmap *h){
	int sz = h->size;
	int i;
	NODEPTR temp;
	for (i = 0; i < sz; i++){
		temp = h->table[i];
		if(temp!=NULL)
		{
		while (temp != NULL){
			printf("%s-->",temp->key);
			printf("%d\t",temp->data);
			temp = temp->next;
		}
		printf("\n------------------------\n");
	    }
	}
}

void hashmap_delete(struct hashmap *h, int val){
	int i;
	for(i=0;i<h->size;i++)
	{
		NODEPTR temp = h->table[i];
		while (temp!=NULL){
			//printf("comparing %s %s",key, temp->key);
			if (temp->data==val)
				{
					strcpy(temp->key,"empty_nul");
				}
			temp = temp->next;
		}
	}
}
// void hashmap_non_null_print(struct hashmap *h, void (*print_data)(void * )){
// 	int sz = h->size;
// 	int i;
// 	NODEPTR temp;
// 	for (i = 0; i < sz; i++){
// 		temp = h->table[i];
// 		if(temp!=NULL)
// 		{
// 		while (temp != NULL){
			
// 				{
// 				//printf("%s-->",temp->key);
// 				print_data(temp->data);
// 				printf("\n------------------------\n");
// 				temp = temp->next;
// 			}
// 		}
// 	}
// 	}
// }
