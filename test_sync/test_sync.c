#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
	if(argc != 2){
		printf("test_sync size/write.");
		return -1;
	}
	int n = atoi(argv[1]);
	if(n <=0 ){
		printf("size/write must > 0 & < 4096");
		return -2;
	}
	int fd = open("test.dat", O_WRONLY | O_CREAT | O_EXCL, 0666);
	if(fd < 0){
		printf("open test.dat failed.\n");
		return -3;
	}
	int flag = fcntl(fd, F_GETFL, 0);
	flag |= O_SYNC;
	if(fcntl(fd, F_SETFL, flag) < 0){
		printf("fcntl set flag failed.\n");
		close(fd);
		return -4;
	}
	char buf[4096];
	int len = 4096 * 1024 * 100;
	int wbytes = 0;
	
	while(wbytes < len){
		int ret = write(fd, buf, 4096);
		if(ret <= 0)
			break;
		wbytes += ret;
	}
	close(fd);
	return 0;
}
