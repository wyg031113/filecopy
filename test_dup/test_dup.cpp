#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
int main()
{
	int fd1 = open("test.dat", O_RDONLY);
	int fd2 = dup(fd1);
	//int fd2 = open("test.dat", O_RDONLY);
	if(fd1 < 0 || fd2 < 0){
		printf("open test.dat failed.\n");
		return -1;
	}
	char buf[10];
	read(fd1, buf, 10);
	close(fd2);
	read(fd1, buf, 10);
	int cur2 = lseek(fd2, 0, SEEK_CUR);
	int cur1 = lseek(fd1, 0, SEEK_CUR);
	printf("cur1:%d\tcur2:%d\n", cur1, cur2);
	close(fd1);
	close(fd2);
	return 0;
}
