/*************************************************************************
    > File Name: filecopy.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: 2016年12月07日 星期三 15时43分41秒
 ************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>
#define BUF_SIZE 4096
int pipe_fd[2];
struct task
{
	int read_fd;
	int write_fd;
	unsigned long long len;
};
struct task rtask;
struct task wtask;
int stop = 0;

unsigned long long real_copy(int fd_src, int fd_dst, unsigned long long len);
void *copy_thread(void *arg)
{
	unsigned long long ret;
	struct task *tsk = (struct task *)arg;
	ret = real_copy(tsk->read_fd, tsk->write_fd, tsk->len);
	return (void *)(ret == tsk->len);
}
unsigned long long real_copy(int fd_src, int fd_dst, unsigned long long len)
{
	printf("real copy.\n");
	char buf[BUF_SIZE];
	int bytes_copyed = 0;
	unsigned long long tlen = len;
	int old_pst = 0;
	time_t start, end;
	start = time(NULL);
	while(!stop && len > 0){
		int read_bytes = read(fd_src, buf, BUF_SIZE);
		if(read_bytes == 0){
			break;
		}
		if(read_bytes < 0){
			printf("read file failed:%s\n", strerror(errno));
			break;
		}
		len -= read_bytes;

		while(read_bytes > 0){
			int write_bytes = write(fd_dst, buf, read_bytes);
			if(write_bytes <0){
				break;
			}
			if(write_bytes <0){
				printf("write file failed:%s\n", strerror(errno));
				stop = 1;
				break;
			}
			read_bytes -= write_bytes;
			bytes_copyed += write_bytes;
		}
		int new_pst = (int)((long long)100 * bytes_copyed / tlen);
		if(new_pst != old_pst){
			end = time(NULL);
			if(end - start == 0)
				printf("copyed %d%%100.\n", new_pst);
			else
				printf("copyed %d%%100, speed= %.2lfMB/s\n", new_pst, 1.0*bytes_copyed/(end-start)/1024/1024);
			old_pst = new_pst;
		}
	}
	return bytes_copyed;
}

int copy(const char *src, const char *dst)
{
	int fd_src = -1;
	int fd_dst = -1;
	
	if(access(src, R_OK)){
		printf("access %s:%s\n", src, strerror(errno));
		return -1;
	}
	struct stat st;
	if(stat(src, &st) != 0){
		printf("stat:%s\n", strerror(errno));
		return -1;
	}
	if(!S_ISREG(st.st_mode)){
		printf("file %s isn't a regular file, can not copy.\n", src);
		return -1;
	}

	struct stat stdst;
	if(!stat(dst, &stdst)){
		if(!S_ISREG(stdst.st_mode)){
			printf("file %s isn't a regular file, can not copy.\n", dst);
			return -1;
		}
	
		char ans[3];
		printf("file %s exists, do you want to overwrite(y/n)?\n", dst);
		do{
			scanf("%1s", ans);
			if(ans[0]=='y')
				break;
			else if(ans[0]=='n')
				return 0;
			printf("please type y or n:");
			while(getchar() != '\n');
		}while(1);
	}else{
		if((fd_dst = creat(dst, 0644)) == -1){
			printf("create file %s failed:%s\n", dst, strerror(errno));
			return -1;
		}
	}
	if((fd_src = open(src, O_RDONLY)) == -1){
		printf("open %s failed:%s\n", src, strerror(errno));
		return -1;
	}

	if(fd_dst == -1 && (fd_dst = open(dst, O_WRONLY|O_TRUNC)) == -1){
		printf("open %s failed:%s\n", dst, strerror(errno));
		close(fd_src);
		return -1;
	}
//	int ret = real_copy(fd_src, fd_dst, st.st_size);
	int ret = 0;
	pthread_t rthread, wthread;
	rtask.read_fd = fd_src;
	rtask.write_fd = pipe_fd[1];
	rtask.len = st.st_size;
	ret = pthread_create(&rthread, NULL,  copy_thread, &rtask);
	if(ret < 0){
		printf("create read thread failed.\n");
		goto failed1;
	}

	wtask.read_fd = pipe_fd[0];
	wtask.write_fd = fd_dst;
	wtask.len = st.st_size;
	ret = pthread_create(&wthread, NULL,  copy_thread, &wtask);
	if(ret < 0){
		printf("create read thread failed.\n");
		goto failed2;
	}
	void *rret, *wret;
	int ret1, ret2;
	ret1 = pthread_join(rthread, &rret);
	ret2 = pthread_join(wthread, &wret);
	if(ret1 !=0 || ret2 != 0){
		ret = -1;
		goto failed1;
	}

	if(!rret || !wret){
		printf("copy ans: copy failed.\n");
		goto failed1;
	}

	close(fd_src);
	close(fd_dst);
	return st.st_size;
failed2:
	stop = 1;
	pthread_join(rthread, &rret);
failed1:
	close(fd_src);
	close(fd_dst);
	return ret;
}

int main(int argc, char *argv[])
{
	int ret;
	if(argc != 3)
	{
		printf("Useage:filecopy src dst\n");
		return -1;
	}
	printf("copy %s to %s\n", argv[1], argv[2]);
	ret = pipe(pipe_fd);
	if(ret < 0){
		printf("create pipe failed.\n");
		return -1;
	}
	ret =  copy(argv[1], argv[2]);
	if(ret >= 0)
		printf("copyed %d bytes\n", ret);
	else
		printf("copy failed.\n");
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return ret;
}
