#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define BUF_SIZE 50000000
char buf[BUF_SIZE];
int set_nonblock(int fd)
{
	int flag;
	flag = fcntl(fd, F_GETFL, NULL);
	if(flag < 0)
		return flag;
	flag |= O_NONBLOCK;
	return fcntl(fd, F_SETFL, &flag);
}

int main(int argc, char *argv[])
{
	if(argc !=2 ){
		printf("Usage:reclock file\n");
		return 1;
	}
	int fd = open(argv[1], O_RDWR);
	if(fd == -1){
		printf("open %s failed:%s\n", argv[1], strerror(errno));
		return 2;
	}
	printf("open successful.\n");
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_start = 0;
	lock.l_whence = SEEK_SET;
	lock.l_len = 10;
	lock.l_pid = 0;
	if(fcntl(fd, F_SETLKW, &lock) == -1){
		printf("fcntl failed:%s\n",strerror(errno));
	}else{
		printf("get lock, sleep.\n");
		sleep(20);
	}
	close(fd);
}
