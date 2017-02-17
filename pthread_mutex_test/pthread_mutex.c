#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#define N 10
pthread_t tids[N];
volatile int counter = 0;
pthread_rwlock_t rwlock;
void reader(int arg)
{
	int k = 10;
	while(k){
		pthread_rwlock_rdlock(&rwlock);
		volatile int t = counter;
		usleep(rand()%100000);
		pthread_rwlock_unlock(&rwlock);
		printf("thread %p read:%d.\n", arg, t);
	}
}
void *add_counter(void *arg)
{
	if((int)arg & 1){
		reader((int)arg);
		return NULL;
	}
	int k = 10;
	while(k--){
		printf("before write.\n");
		pthread_rwlock_wrlock(&rwlock);
		int t = counter;
		t++;
//		usleep(rand()%100000);
		counter = t;
		pthread_rwlock_unlock(&rwlock);
		printf("add finish:%d\n", t);	
	}
	printf("thread %p exited.\n", arg);
	return NULL;
}	
int main()
{
	int i;
	pthread_rwlock_init(&rwlock, 0);
	for(i = 0; i < N; i++){
		if(pthread_create(&tids[i], NULL, add_counter, (void*)(long)i) != 0){
			printf("create thread %d failed.\n", i);
			exit(-1);
		}
	}
	for(i = 0; i < N; i++)
		pthread_join(tids[i], NULL);
	printf("counter=%d\n", counter);
	pthread_rwlock_destroy(&rwlock);
	return 0;
}
