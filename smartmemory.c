#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct Heap
{
	int size;
	int written;
	void *addr;
	int type;
	int *count;
} heap;

void __malloc(heap *id)
{
	id->addr = malloc(BASESIZE);
}

void __contructor(heap *id)
{
	__malloc(id);       
	id->written = 0;
	id->size = BASESIZE;
	id->type = VOID;
	id->count = malloc(sizeof(int));
	*(id->count) = 1;
}

void __assign(heap *id1, heap *id2)
{
	assert(id1->type == id2->type);

	id1->written = id2->written;
	id1->size = id2->size;

	memcpy(id1->addr, id2->addr, id2->size);
	id1->count = id2->count;
}

heap* assign(heap *id) {
	heap *t = malloc(sizeof(heap));
	__constructor(t);	
	t->type = id->type;
	__assign(t, id);
	return &t;
}
