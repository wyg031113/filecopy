/*************************************************************************
    > File Name: posix_mq.c
    > Author: wyg
    > Mail: wyg_0802@126.com 
    > Created Time: 2017年02月17日 星期五 15时22分26秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <map>
#include <string>
#include <string.h>
#include <sys/resource.h>
#include <signal.h>
// protocal: file_name offset data
#define MAX_MSG_SIZE 4096
char msg[MAX_MSG_SIZE];
using namespace std;

int main(int argc, char *argv[])
{
	if(argc != 4){
		printf("Usage:msg_client msg_queue src_file dst_file\n");
		return 1;
	}
	
	int fd = open(argv[2], O_RDONLY);
	if(fd < 0){
		perror("open file");
		return 5;
	}
	mqd_t mqd = mq_open(argv[1], O_WRONLY);
	if(mqd == -1){
		printf("mq open failed:%s.\n", strerror(errno));
		exit(2);
	}
	if(strlen(argv[3])>= MAX_MSG_SIZE-6){
		printf("error:file name too long!\n");
		exit(3);
	}
	strcpy(msg, argv[3]);
	int len = strlen(msg) + 1;
	int offset = 0;
	while(1)
	{
		*(int *)(msg+len) = offset;
		int ret = pread(fd, msg + len + 4, MAX_MSG_SIZE - len - 4, offset);
		if(ret < 0){
			printf("read file failed:%s\n", strerror(errno));
			break;
		}else if(ret == 0){
			printf("read finished.\n");
			break;
		}else{
			
			int r = mq_send(mqd, msg, ret + len + 4, 0);
			if(r == -1){
				printf("send msg failed:%s\n", strerror(errno));
				break;
			}
			offset += ret;
		}
	}
	close(fd);
	mq_close(mqd);
	printf("client finished.\n");
	return 0;
}
