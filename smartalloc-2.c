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

typedef struct Parent 
{
	int size;
	int written;
	void *addr;
	int type;
	int count;
} parent;

typedef struct Heap
{
	struct Parent *parent;
} heap;

void __destructor(heap *id)
{
	if ((--(id->parent->count)) == 0) {
		switch(id->parent->type) {
			case STR: 
				free((char *)id->parent->addr);
				break;

			case INT:
				free((int *)id->parent->addr);
				break;

			case DOUBLE:
				free((double *)id->parent->addr);
				break;

			case FLOAT:
				free((float *)id->parent->addr);
				break;
		}
		
		free(id->parent);
	}

	free(id);
	return;
}
void __free_parent(struct Heap *id) 
{
	free(id->parent->addr);
	free(id->parent);

}
void __realloc_exact(struct Heap *id, int size)
{
	if(id->parent->addr != NULL) {	
		free(id->parent->addr);
	}
	id->parent->addr = malloc(size);
	id->parent->size = size;
}

heap *constructor()
{
	struct Heap *id = (struct Heap *)malloc(sizeof(struct Heap));
	struct Parent *parent = (struct Parent *)malloc(sizeof(struct Parent));

	/* initialize all Parent data vals */
	parent->written = 0;
	parent->size = 0; 
	parent->type = VOID;
	parent->count = 1;
	parent->addr = NULL;

	/* assign id to parent */
	id->parent = parent;

	return id;
}

void __assign(heap *dest, heap *source)
{
	assert(source->parent->type == dest->parent->type);

	__free_parent(dest);

	dest->parent = source->parent;
	source->parent->count = source->parent->count + 1;
	return;

}

heap* assign(heap *id) {

	heap *t = constructor();	
	t->parent->type = id->parent->type;
	__assign(t, id);
	return t;
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
			strcpy(id->parent->addr, ((char *)val));
			id->parent->type = STR;
			break;

		case INT:
			__safe_write(id, sizeof(int), val);
			*(int *)(id->parent->addr) = *((int *)val);
			id->parent->type = INT;
			break;

		case DOUBLE:
			__safe_write(id, sizeof(double), val);
			*(double *)(id->parent->addr) = *((double *)val);
			id->parent->type = DOUBLE;
			break;

		case FLOAT:
			__safe_write(id, sizeof(float), val);
			*(float *)(id->parent->addr) = *((float *)val);
			id->parent->type = FLOAT;
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

	int *p = malloc(10000);
	free(p);
	p = malloc(10000000);
	free(p);
	printf("Allocating memory...\n");
	h1 = constructor();

	printf("Assigning\n");
	heap* h2 = assign(h1);
	printf("h1's count: %d\n", h1->parent->count);
	printf("h2's count: %d\n", h2->parent->count);

	printf("Testing writing and overwriting char * in heap...\n");
	str_safe_write(h1, "Clean Ccccccccccc");
	printf("%s\n", (char *)h1->parent->addr);
	//str_safe_write(h1, "Kleen Cccccccccccccccccccccc");
	printf("%s\n", (char *)h1->parent->addr);
	printf("SUCCESS IN OVERWRITING CHAR *\n");

	printf("Deallocating memory...\n");
	__destructor(h1);
	__destructor(h2);

	return 0;
}

