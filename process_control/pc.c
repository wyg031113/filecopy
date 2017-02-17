#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
	printf("pid:%d ppid:%d uid:%d euid:%d gid:%d egid:%d\n",
			getpid(), getppid(), getuid(), geteuid(), getgid(), getegid());
	return 0;
}
