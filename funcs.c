#include <stdio.h>
#include <stdlib.h>
int getPow(int x, int p)
{
    int i;
    int res = 1;
    for (i = 0; i < p; ++i)
	res *= x;
    return res;
}
/*
int main(int argc, char** args)
{
    int x = 10;
    fprintf(stdout,"%d\n",getPow(x,2));

    return 0;
}
*/
