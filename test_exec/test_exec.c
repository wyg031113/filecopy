#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

char *env[] = {"a=b", "c=d"};
void sig_fun(int sig)
{
	printf("sig fun.\n");
}
int main(int argc, char *argv[])
{
	pid_t pid;
	fcntl(1, F_SETFD, FD_CLOEXEC);
	printf("sig1:%d\n", sig_fun == signal(SIGALRM, sig_fun));
	signal(SIGALRM, sig_fun);
	setenv("wyg", "26", 1);
	if((pid = fork()) < 0){
		printf("fork failed.\n");
		return -1;
	}
	if(pid > 0){
		int status = 0;
		printf("father running....\n");
		printf("son pid=%d\n", pid);
		pid_t ret = wait(&status);
		//printf("ret=%d status=%d\n", ret, WEXITSTATUS(status));
		if(WIFEXITED(status)){
			printf("Child exit:%d\n", WEXITSTATUS(status));
		}else if(WIFSIGNALED(status)){
			printf("Child sig:%d\n", WTERMSIG(status));
		}else if(WIFSTOPPED(status)){
			printf("Child stop:%d\n", WSTOPSIG(status));

		}else if(WIFCONTINUED(status)){
			printf("chile continued" );
		}
		else{
			printf("other exit.\n");
		}

		return 2;
	}else{
		printf("child running....\n");
		printf("%s:%s\n", "wyg", getenv("wyg"));
	//printf("child sig:%d\n", sig_fun == signal(SIGALRM, sig_fun));
		alarm(5);
		sleep(6);
		int fd = open("hello", O_CREAT|O_WRONLY | O_APPEND, 0666);
		if(fd == -1){
			printf("open failed..\n");
			return -1;
		}
		dup2(fd, 1);
		close(fd);
		dup2(2,1);
		int ret = execle("/root/filecopy/test_atexit/atexit", "/root/filecopy/test_atexit/atexit", "-al", NULL, env);
		if(ret < 0){
			printf("exec failed:%s\n", strerror(errno));
		}

		return 3;
	}
	return 0;
}
