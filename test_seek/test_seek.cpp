#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
int main()
{
	int fd1 = open("test.dat", O_WRONLY|O_CREAT, 0644);
	int cur1 = lseek(fd1, 4096*100, SEEK_CUR);
	write(fd1, "hello", 5);
	close(fd1);
	return 0;
}
