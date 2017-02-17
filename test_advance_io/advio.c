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
	int len = 0;
	int wbyts = 0;
	len = read(STDIN_FILENO, buf, BUF_SIZE);
	if(len == -1){
		fprintf(stderr,"read failed:%s\n", strerror(errno));
		return -1;
	}else{
		fprintf(stderr,"read %d bytes.\n", len);
	}
	if(set_nonblock(STDOUT_FILENO) == -1){
		fprintf(stderr,"set nonblock failed:%s\n", strerror(errno));
		return -2;
	}
	fprintf(stderr,"begin write\n");
	while(wbyts < len){
		int ret = write(STDOUT_FILENO, buf + wbyts, len - wbyts);
		if(ret == -1){
			fprintf(stderr,"write error:%s\n", strerror(errno));
			if(errno != EAGAIN){
				break;
			}
		}else{
			wbyts += ret;
		}

	}
	fprintf(stderr, "write %d bytes.\n", wbyts);
	return 0;
}
