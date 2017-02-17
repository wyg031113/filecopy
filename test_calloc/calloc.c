#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
	int i;
	long **p = calloc(10, sizeof(long*));
	for(i = 0; i < 10; i++)
		printf("%ld ", p[i]);
	printf("\n");
}
