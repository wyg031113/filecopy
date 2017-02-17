#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if(argc != 2){
		printf("Usage:getrlimit num\n");
		return -2;
	}
	struct rlimit rl;
	getrlimit(atoi(argv[1]), &rl);
	printf("%d %d\n", rl.rlim_cur, rl.rlim_max);
	
}
