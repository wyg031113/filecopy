#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
void print_id()
{
	pid_t pid = getpid();
	pthread_t tid = pthread_self();
	printf("pid:%d tid:%lu tid:%lx\n", pid, tid, tid);
}
void *th_fn(void *arg)
{
	print_id();
	int *x = (int *) malloc(sizeof(int));
	*x = 7788;
	return x;
}
int main()
{
	pthread_t tid;
	if(pthread_create(&tid, NULL, th_fn, NULL) != 0){
		printf("Create Thread Failed.");
		return -1;
	}
	print_id();
	int *ret = NULL;
	pthread_join(tid, (void **) &ret);
	if(ret != NULL){

		printf("Thread ret:%d\n", *ret);
		free(ret);
	}
}
