#include <unistd.h>
#include <cstdlib>
#include <fcntl.h>
#include <cstdio>
#include <sys/wait.h>
int main()
{
	int fd1 = open("test.dat", O_RDONLY);
	int pid = fork();
	int fd2 = dup(fd1);
	if(pid < 0){
		printf("fork failed.\n");
		return -1;
	}else if(pid > 0){
		//sleep(1);
		int status = 0;
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
		printf("Pos son:%ld fd2:%ld\n", lseek(fd1, 0, SEEK_CUR), 
				lseek(fd2, 0, SEEK_CUR));
		close(fd1);
		close(fd2);
	}else{
		char buf[10];
		read(fd2, buf, 10);
		printf("Pos son:%ld fd2:%ld\n", lseek(fd1, 0, SEEK_CUR), 
				lseek(fd2, 0, SEEK_CUR));
		close(fd1);
		close(fd2);
		sleep(20);
		exit(3);
	}
	return 0;
}
