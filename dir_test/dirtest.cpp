#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <fcntl.h>
#include <errno.h>
#include <cstdlib>
#include <sys/stat.h>
#include <cstring>

int main(int argc, char *argv[])
{
	if(argc != 2){
		printf("Usage:dirtest file\n");
		return -1;
	}
	struct stat st;
	if(::stat(argv[1], &st) == -1){
		printf("stat failed:%s\n", strerror(errno));
		return -2;
	}
	if(st.st_mode & S_IRUSR){
		printf("can read.\n");
	}
	if(S_ISREG(st.st_mode)){
		printf("normal file.\n");
	}
	int fd;
	if((fd = open(argv[1], O_RDWR)) == -1){
		printf("open %s failed:%s\n", argv[1], strerror(errno));
		return -3;
	}
	unlink(argv[1]);
	int ret = write(fd, "hello world", 11);
	if(ret == -1){
		printf("stat failed:%s\n", strerror(errno));
		return -4;

	}
	printf("after unlink\n");
	sleep(10);
	close(fd);
	printf("after close\n");
	sleep(10);
}
