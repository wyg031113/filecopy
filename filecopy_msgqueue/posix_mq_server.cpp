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
//file_name offset data
#define MAX_MSG_SIZE 4096
char msg[MAX_MSG_SIZE];
using namespace std;
volatile bool stop = false;
void handle_sig(int sig)
{
	printf("begin exiting...\n");
	stop = true;
}
void handle_msg(char *msg, int len)
{
	char *fname = msg;
	int name_len = strlen(msg);
	if(name_len <= 0 || name_len > len || len <= 5)
		return;
	int offset = *(int *)(msg + name_len + 1);
	char *data = msg + name_len + 1 + 4;
	int data_len = len - (name_len + 1 + 4);
	if(offset < 0){
		printf("error:%s, offset < 0.\n", fname);
		return;
	}
	int fd = open(fname, O_CREAT | O_WRONLY, 0666);
	if(fd < 0){
		printf("open file %s failed:%s\n", fname, strerror(errno));
		return;
	}
	int wbytes = 0;
	while(wbytes < data_len){
		int ret = pwrite(fd, data + wbytes, data_len - wbytes, offset+wbytes);
		if(ret < 0){
			printf("write file %s failed:%s\n", fname, strerror(errno));
			return;
		}
		wbytes += ret;
	}
	close(fd);
}

int main(int argc, char *argv[])
{
	if(argc != 2){
		printf("Usage:msg_server msgqueue\n");
		return 1;
	}
	signal(SIGINT, handle_sig);
	struct rlimit rlim;
	setrlimit(RLIMIT_MSGQUEUE, &rlim);
	printf("cur msg queue limit:cur:%ld max:%ld\n", 
			rlim.rlim_cur, rlim.rlim_max);
	memset(&rlim, 0, sizeof(rlim));
	rlim.rlim_cur = RLIM_INFINITY;
	rlim.rlim_max = RLIM_INFINITY;

	setrlimit(RLIMIT_MSGQUEUE, &rlim);

	mq_unlink(argv[1]);
	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = 4096;
	attr.mq_msgsize = MAX_MSG_SIZE;
	attr.mq_curmsgs = 0;
	mqd_t mqd = mq_open(argv[1], O_CREAT|O_RDONLY, 0666, &attr);
	if(mqd == -1){
		printf("mq open failed:%s.\n", strerror(errno));
		exit(2);
	}
	while(!stop){
		struct timespec abs_time;
		clock_gettime(CLOCK_REALTIME, &abs_time);
		abs_time.tv_sec += 1;
		int msg_len = mq_timedreceive(mqd, msg, MAX_MSG_SIZE, NULL, &abs_time);	
		if(msg_len == -1){
			printf("msg recv failed:%s\n", strerror(errno));
			if(errno == EINTR){
				printf("recv EINTR.\n");
				continue;
			}else if(errno == ETIMEDOUT){
				printf("recv timed out.\n");
				continue;
			}
			else
				break;
		}
		if(msg_len < 1){
			printf("bad msg size:%d\n", msg_len);
			continue;
		}
		handle_msg(msg, msg_len);		
	}
	mq_close(mqd);
	mq_unlink(argv[1]);
	printf("server exited.\n");
	return 0;
}
