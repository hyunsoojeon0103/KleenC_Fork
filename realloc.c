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

struct Heap
{
	int size;
	int written;
	void *addr;
	int type;
};

void safe_write(struct Heap *id, void *val, int type);

void __malloc(struct Heap *id)
{
	id->addr = malloc(BASESIZE);
}

void __contructor(struct Heap *id)
{
	__malloc(id);       
	id->written = 0;
	id->size = BASESIZE;
	id->type = VOID;

}
void __destructor(struct Heap *id)
{
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
}
void __realloc(struct Heap *id)
{
	int size = id->size;
	id->addr = realloc(id->addr, size*REALLOC);
	id->size = size*REALLOC;
}

void __realloc_exact(struct Heap *id, int size)
{
	id->addr = realloc(id->addr, size);
	id->size = size;
}

int __boundary(struct Heap *id)
{
	if (id->written > BOUNDARY * (id->size))
		return 1;
	else
		return 0;
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
	struct Heap h1;
	struct Heap h2;
	struct Heap h3;
	struct Heap h4;

	double test;

	printf("Allocating memory...");
	__contructor(&h1);
	__contructor(&h2);
	__contructor(&h3);
	__contructor(&h4);

	printf("Testing writing and overwriting char * in heap...\n");
	str_safe_write(&h1, "Clean C");
	printf("%s\n", (char *)h1.addr);
	str_safe_write(&h1, "Kleen C");
	printf("%s\n", (char *)h1.addr);
	printf("SUCCESS IN OVERWRITING CHAR *\n");

	printf("Testing storing ints, floats, doubles in heap...\n");
	int_safe_write(&h2, 10);
	double_safe_write(&h3, 10.1);
	double_safe_write(&h4, 10.2);
	test = *(double *)(h3.addr) + *(double *)(h4.addr);
	printf("SUCCESS IN ADDITION, TEST = %f\n", test);

	printf("Deallocating memory...");
	__destructor(&h1);    
	__destructor(&h2);
	__destructor(&h3);
	__destructor(&h4);

	return 0;
}

