#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
int x = 0;
void *thread(void *arg)
{
	printf("thread %lx running...\n", pthread_self());
	//pthread_detach(pthread_self());
	int n = 0;
	while(n < 3){
		pthread_mutex_lock(&mtx);
		while(x == 0){
			pthread_cond_wait(&cond, &mtx);
			printf("thread %ld awake\n", pthread_self());
		}
		n++;
		x = 0;
		pthread_mutex_unlock(&mtx);
		printf("n=%d tid=%lu\n", n, pthread_self());
	}
}

void sig(int sig){
	pthread_mutex_lock(&mtx);
	x = 1;
	pthread_mutex_unlock(&mtx);
	//pthread_cond_signal(&cond);
	pthread_cond_broadcast(&cond);
	sleep(1);
	printf("sig handler exited.\n");
}

#define N 2
int main(int argc, char *argv[])
{
	int i;
	pthread_t tids[N];
	signal(SIGINT, sig);

	for(i = 0; i < N; i++){
		pthread_create(&tids[i], NULL, thread, NULL);
	}
	sleep(20);
	printf("after sleep.\n");
	for(i = 0; i < N; i++){
		pthread_join(tids[i], NULL);
	}

	printf("Main thread exited.\n");
	return 0;
}
