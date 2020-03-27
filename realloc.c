#define BASESIZE 100
#define BOUNDARY .66
#define REALLOC 2
struct Heap
{
    int size;
    int written;
    void *addr;
}

__malloc(struct Heap *id)
{
    id->addr = malloc(BASESIZE);
}
__realloc(struct Heap *id)
{
    int size = id->size;

    id->addr = realloc(id->addr, size*REALLOC);
    id->size = size*REALLOC;
}
__boundary(struct Heap *id)
{
    if (id->written > BOUNDARY * (id->size))
        return 1;
    else
        return 0;
}

/* for each write
 * if they exceed 2/3 of mem
 * of struct, reallocate
 */
__write(struct Heap *id, void *data)
{
    if(__boundary(id))
        __realloc(id);

    memcpy(data, id->addr);

}
int main()
{

    
    struct Heap h;
    /* first write on stack then always copy to heap? */

    
    free(h);
}
