#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>

jmp_buf jb;
int x = 0;
void fun2()
{
	printf("enter fun2.\n");
	longjmp(jb, 23);
	printf("exit fun2.\n");;
}
void fun1()
{
	printf("enter fun1.\n");
	x = 345;
	fun2();
	printf("exit fun1.\n");
}
int main()
{
	int ret;
	volatile int y = 0;
	if((ret=setjmp(jb)) !=0 ){
		printf("x=%d ret=%d y = %d\n", x, ret, y);
		exit(0);
	}
	y = 234;
	fun1();
	return 0;
}
