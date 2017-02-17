#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
pthread_key_t num;
pthread_once_t init_done = PTHREAD_ONCE_INIT;
void deconstructor(void *data)
{
	if(data == NULL){
		printf("Deconstructor running, thread:%lx, data:NULL\n", 
				pthread_self());
		return;
	}
	printf("Deconstructor running, thread:%lx, data:%d\n", 
			pthread_self(), *((int*)data));

}
void init_key(void)
{
	printf("thread:%lx init key.\n", pthread_self());
	pthread_key_create(&num, deconstructor);
}
void *thread(void *arg)
{
	int k = 10;
	printf("thread:%d -->0x%lx\n", (int)arg, pthread_self());
	pthread_once(&init_done, init_key);
	while(k--)
	{
		int *p = (void*)pthread_getspecific(num);
		if(p == NULL){
			p = (int *)malloc(sizeof(int));
			if(p == NULL){
				printf("malloc num failed.\n");
				pthread_exit((void*)1);
			}
			pthread_setspecific(num, (void*)p);
		}
		*p += k+1;
	}
	return NULL;
}
#define N 10
pthread_t tids[N];
int main()
{
	int i;
	for(i = 0; i < N; i++){
		pthread_create(&tids[i], NULL, thread, (void*)(long)i);
	}

	for(i = 0; i < N; i++){
		pthread_join(tids[i], NULL);
	}
	return 0;
}
