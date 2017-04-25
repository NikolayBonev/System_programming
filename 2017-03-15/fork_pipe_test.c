#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include <sys/types.h>
#include <sys/stat.h>

#define READ 0
#define WRITE 1

#define SIZE_BUFFER 50

#define NAMED_PIPE "/mypipe"

/*
	@author Nikolay Bonev
	The program shows how to create child processes and how to send from information from child to parent process using pipes. 
*/


/*
	Send function
	Sends string with max lenght 50 chars to the pipe.
*/
void send(int* mypipe, char* message);

/*
	Receive function
	Receives a string with max lenght 50 chars from the pipe.
*/
int receive(int* mypipe, char* rcvbuf);

int main()
{
	int mypipe[2];

	pipe(mypipe);
	mkfifo(NAMED_PIPE, 0666);

	char* msg = "Hello father!";

	if(0 == fork())
	{
		//Child
		send(mypipe, msg);		
	}
	else
	{
		//Parent
		char* rcvbuf = malloc(sizeof(char) * SIZE_BUFFER);

		int bytes_received = receive(mypipe, rcvbuf);

		printf("received %d bytes. Message %s", bytes_received, rcvbuf);

		free(rcvbuf);
	}
}

void send(int* mypipe, char* message)
{
	close(mypipe[READ]);

	write(mypipe[WRITE], message, strlen(message)+1);

	close(mypipe[WRITE]);
}

int receive(int* mypipe, char* rcvbuf)
{
	int bytes_received = 0;

	close(mypipe[WRITE]);

	bytes_received = read(mypipe[READ], rcvbuf, SIZE_BUFFER);

	close(mypipe[READ]);

	return bytes_received;
}
