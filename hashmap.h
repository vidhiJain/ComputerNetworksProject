#ifndef HASHMAP_H
#define HASHMAP_H

typedef struct hash_node
{
    char * key;
    int data;
    struct hash_node *next;
}hash_node, *NODEPTR;


typedef struct hashmap
{
	int size;   // size of hashmap
	NODEPTR* table; //
}hashmap, *Hashmap;

unsigned long hashx(unsigned char *str);

struct hashmap* hashmap_create(int size);

void hashmap_delete(struct hashmap *h, int val);

void hashmap_insert(struct hashmap *h,char * key, int val);

int hashmap_search(struct hashmap *h, char * key);

void hashmap_print(struct hashmap *h);

#endif