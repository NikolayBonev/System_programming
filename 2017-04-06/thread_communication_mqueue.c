#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <mqueue.h>
#include <string.h>
#include <fcntl.h>

#define MQUEUE "/myqueue"
#define MESSAGE_SIZE 100

void *send_message_function(void* ptr);
void *receive_message_function(void* ptr);

int main()
{

	int status = -1;
	
	pthread_t th1, th2;

	status = pthread_create(&th1, NULL, send_message_function, NULL);
	
	if(status < 0)
	{
		perror("Error create send thread!");
	}
	
	status = pthread_create(&th2, NULL, receive_message_function, NULL);

	if(status < 0)
	{
		perror("Error create receive thread!");
	}

	pthread_join(th2, NULL);

	pthread_exit((void*) th1);
	pthread_exit((void*) th2);

	return 0;
}

void *send_message_function(void* ptr)
{
	mqd_t mq;
	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = 16;
	attr.mq_msgsize = MESSAGE_SIZE;
	attr.mq_curmsgs = 0;	

	char message[MESSAGE_SIZE];
	int status = 0;

	mq = mq_open(MQUEUE, O_CREAT, 0777, NULL);
	
	scanf("%s", message);

	mq = mq_open(MQUEUE, O_WRONLY, 0777, NULL);

	if(mq < 0)
	{
		printf("Send thread open mq!");
	}

	printf("Send: %s", message);

	mq_send(mq, message, MESSAGE_SIZE, 1);

	mq_close(mq);
}

void *receive_message_function(void* ptr)
{
	mqd_t mq;
	char* message = malloc(MESSAGE_SIZE);
	int status = 0;	
	struct mq_attr attr;

	mq_getattr(mq, &attr);

	mq = mq_open(MQUEUE, O_RDONLY, 0777, NULL);

	mq_receive(mq, message, attr.mq_msgsize, NULL);
		
	printf("Received: %s", message);
}
