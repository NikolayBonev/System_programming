#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <mqueue.h>

#define MQUEUE "/mqueue"

/*
	@author Nikolay Bonev
	The program shows how to create a message queue and send through it a integer.
*/

/*
	Send function
	Receives as a paramenter a integer that will be sended through the queue.
*/
void send_num(int num);

/*
	Receive function
	Reads from the queue the sended integer.
*/
void receive_num();

mqd_t mq;
struct mq_attr attr;

int main()
{
	int send = 666;

	attr.mq_flags = 0; // O_NONBLOCK
	attr.mq_maxmsg = 16;
	attr.mq_msgsize = sizeof(int)*2;
	attr.mq_curmsgs = 0;

	send_num(send);
	receive_num();
}

void send_num(int num)
{
	int status = -1;

	mq = mq_open(MQUEUE, O_RDWR | O_CREAT, 0777, NULL);

	if(0 > mq)
	{
		perror("open");
	}

	status = mq_send(mq, (char*)&num, sizeof(num), 1);

	if(0 > status)
	{
		perror("send");
	}

	printf("Expected %d\n", num);
}

void receive_num()
{
	int status = -1;
	int rcv;

	mq_getattr(mq, &attr);
	status = mq_receive(mq, (char*)&rcv, attr.mq_msgsize, NULL);
	
	if(0 > status)
	{
		perror("receive");
	}

	status = close(mq);

	if(0 > status) 
	{
		perror("close");
	}

	status = mq_unlink(MQUEUE);
	if(0 > status)
	{
		perror("unlink");
	}

	printf("Received %d\n", rcv);
}
