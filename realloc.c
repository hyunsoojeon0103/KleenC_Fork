#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASESIZE 100
#define BOUNDARY .66
#define REALLOC 2

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
	id->type = 0;

}
void __destructor(struct Heap *id)
{

	switch(id->type)
	{
		case 1: free((char *)id->addr);
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

void safe_write(struct Heap *id, void* val, int type)
{

	switch(type) {
		case 1:
			__safe_write(id, (strlen((char *)val) + 1), val);
			strcpy(id->addr, ((char *)val));
			id->type = 1;
			/*        case 2:
				  __safe_write(id, sizeof(char), val);
			 *(char *)(id->addr) = ((char)val);
			 case 3:
			 __safe_write(id, sizeof(int), val);
			 *(int *)(id->addr) = ((int )val);
			 case 4:
			 __safe_write(id, sizeof(double), val);
			 *(double *)(id->addr) = ((double)val);
			 case 5:
			 __safe_write(id, sizeof(float), val);
			 *(float *)(id->addr) = ((float)val);
			 */   
	}

}

void str_safe_write(struct Heap *id, char *val)
{
	safe_write(id, (void *)val, 1);
}
/*
   void char_safe_write(struct Heap *id, void val)
   {
   safe_write(id, val, 2);
   }

   void int_safe_write(struct Heap *id, void val)
   {
   safe_write(id, val, 3);
   }

   void float_safe_write(struct Heap *id, void val)
   {
   safe_write(id, val, 5);
   }

   void double_safe_write(struct Heap *id, void val)
   {
   safe_write(id, val, 6);
   }
   */
int main()
{

	struct Heap h1;
	struct Heap h2;
	struct Heap h3;
	struct Heap h4;
	struct Heap h5;

	__contructor(&h1);

	str_safe_write(&h1, "hello!!!!");
	printf("%s", (char *)h1.addr);

	__destructor(&h1);
	return 0;
}

