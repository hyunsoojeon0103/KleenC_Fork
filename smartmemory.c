#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define BASESIZE 100
#define BOUNDARY .66
#define REALLOC  2

#define VOID    0
#define STR     1
#define INT     2
#define DOUBLE  3       
#define FLOAT   4

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

heap *constructor()
{
	heap *id = (heap *) malloc(sizeof(heap));
	__malloc(id);       
	id->written = 0;
	id->size = BASESIZE;
	id->type = VOID;
	id->count = malloc(sizeof(int));
	*(id->count) = 1;

	return id;
}

void __assign(heap *id1, heap *id2)
{
	assert(id1->type == id2->type);

	id1->written = id2->written;
	id1->size = id2->size;

	//memcpy(id1->addr, id2->addr, id2->size);
	id1->addr = id2->addr;
	id1->count = id2->count;
	*(id1->count) += 1;
}

heap* assign(heap *id) {
	heap *t = constructor();	
	t->type = id->type;
	__assign(t, id);
	return t;
}

void __destructor(heap *id) {
	if (--*(id->count) == 0) {
		/*
		switch(id->type)
		{
			case STR: 
				free((char *)id->addr);
				break;

			case INT:
				free((int *)id->addr);
				break;

			case DOUBLE:
				free((double *)id->addr);
				break;

			case FLOAT:
				free((float *)id->addr);
				break;
		}
		*/
		free(id->addr);
		free(id->count);
	}
	free(id);
}

void __realloc_exact(struct Heap *id, int size)
{
	id->addr = realloc(id->addr, size);
	id->size = size;
}


void __safe_write(struct Heap *id, int wsize, void *val)
{
	__realloc_exact(id, wsize);
} 

void safe_write(struct Heap *id, void *val, int type)
{
	switch(type) {
		case STR:
			__safe_write(id, (strlen((char *)val) + 1), val);
			strcpy(id->addr, ((char *)val));
			id->type = STR;
			break;

		case INT:
			__safe_write(id, sizeof(int), val);
			*(int *)(id->addr) = *((int *)val);
			id->type = INT;
			break;

		case DOUBLE:
			__safe_write(id, sizeof(double), val);
			*(double *)(id->addr) = *((double *)val);
			id->type = DOUBLE;
			break;

		case FLOAT:
			__safe_write(id, sizeof(float), val);
			*(float *)(id->addr) = *((float *)val);
			id->type = FLOAT;
			break;
	}
}

void str_safe_write(struct Heap *id, char *val)
{
	safe_write(id, (void *)val, STR);
}

void int_safe_write(struct Heap *id, int val)
{
	safe_write(id, &val, INT);
}

void float_safe_write(struct Heap *id, float val)
{
	safe_write(id, &val, FLOAT);
}

void double_safe_write(struct Heap *id, double val)
{
	safe_write(id, &val, DOUBLE);
}

int main()
{
	heap* h1;

	printf("Allocating memory...\n");
	h1 = constructor();

	printf("Assigning\n");
	heap* h2 = assign(h1);
	printf("h1's count: %d\n", *h1->count);
	printf("h2's count: %d\n", *h2->count);

	printf("Testing writing and overwriting char * in heap...\n");
	str_safe_write(h1, "Clean C");
	printf("%s\n", (char *)h1->addr);
	str_safe_write(h1, "Kleen C");
	printf("%s\n", (char *)h1->addr);
	printf("SUCCESS IN OVERWRITING CHAR *\n");
	
	printf("Deallocating memory...\n");
	__destructor(h1);
    __destructor(h2);

	return 0;
}

