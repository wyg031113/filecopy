#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
extern char **environ;
void fun1(void)
{
	printf("fun1.\n");
}
void fun2(void)
{
	printf("fun2.\n");
}

int main()
{
	atexit(fun1);
	atexit(fun2);
	int i;
	printf("atexit:\n");
	for(i = 0; environ[i]; i++)
		printf("%s\n", environ[i]);
	alarm(3);
	sleep(15);
	exit(1);
	return 0;
}
