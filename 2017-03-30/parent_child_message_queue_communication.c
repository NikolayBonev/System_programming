#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <mqueue.h>
#include <string.h>

#define MQUEUE "/myqueue"

void produceMessage(char* message, mqd_t mq);
void consumeMessage(char** message, mqd_t mq, struct mq_attr attr);
void error_mq_open(char* process_name, mqd_t mq);
void error_mq_close(char* process_name, int status);

int main(int argc, char** argv)
{
	pid_t childPid = 0;
	childPid = fork();
	
	char* send = "Hello father!";
	char* rcv;
	int status = -1;

	mqd_t mq;
	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = 16;
	attr.mq_msgsize = strlen(send);
	attr.mq_curmsgs = 0;
	
	if(childPid >= 0)
	{
		if(childPid == 0)
		{
			produceMessage(send, mq);
		}else{
			consumeMessage(&rcv, mq, attr);

			printf("%s\n", rcv);
		}
	}else{
		printf("Forking failed!\n");
		
		return 1;
	}
}

void produceMessage(char* message, mqd_t mq)
{
	int status = -1;

	mq = mq_open(MQUEUE, O_WRONLY | O_CREAT, 0777, NULL);

	error_mq_open("openC", mq);

	status = mq_send(mq, message, strlen(message) + 1, 1);

	if(status < 0)
	{
		perror("sendC");
	}

	status = mq_close(mq);

	error_mq_close("closeC", status);	
}

void consumeMessage(char** message, mqd_t mq, struct mq_attr attr)
{
	int status = -1;

	mq = mq_open(MQUEUE, O_RDONLY | O_CREAT, 0777, NULL);

	error_mq_open("openF", mq);
			
	mq_getattr(mq, &attr);

	*message = malloc(attr.mq_msgsize);

	status = mq_receive(mq, *message, attr.mq_msgsize, NULL);

	if(status < 0)
	{
		perror("receiveF");
	}

	status = mq_close(mq);

	error_mq_close("closeF", status);

	status = mq_unlink(MQUEUE);
			
	if(status < 0)
	{
		perror("unlink");
	}
}

void error_mq_open(char* process_name, mqd_t mq)
{
	if(mq < 0)
	{
		perror(process_name);
	}
}

void error_mq_close(char* process_name, int status)
{
	if(status < 0)
	{
		perror("closeF");
	}	
}
