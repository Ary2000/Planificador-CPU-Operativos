#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define PAUSAR 1

void *myfunc (void *myvar);


int main(int argc, char *argv[])
{
	pthread_t thread1,thread2;
	char *msg1 = "FIRST THREAD";
	char *msg2 = "SECOND THREAD";
	int ret1, ret2;



	ret1 = pthread_create(&thread1, NULL, myfunc, (void *) msg1);
	ret2 = pthread_create(&thread2, NULL, myfunc, (void *) msg2);

	printf("Main function after pthread_created\n");


	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);

	printf("FIRST THREAD RET1 = %d\n", ret1);
	printf("SECOND THREAD RET2 = %d\n", ret2);

	return 0;
}

void *myfunc(void *myvar)
{
	char *msg;
	msg = (char *) myvar;
	int i = 0;
	while(1){
		printf("%s %d\n", msg, i);
		sleep(1);
		i = i + 1;
		if(i==10){
			break;
		}
	}
	return NULL;

}
