#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define PACKET_SIZE 1500

/* Variables globales */
int send_port, rec_port;
char remote_address[30] = "127.0.0.1";

/* Hilos de ejecucion */
void* senderT(void*);
void* receiverT(void*);

/* Pthread Mutex */
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char** argv)
{
	if (argc != 4)
		{
			printf("Usage:  peer <Dest address> <Dest Port> <RecPort>\n");
			exit(1);
		}
	strcpy(remote_address, argv[1]);
	send_port = atoi(argv[2]);
	rec_port = atoi(argv[3]);
	
	int sockfd;
	pthread_t sender, receiver;
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	if(pthread_create(&receiver, NULL, receiverT, (void*) &sockfd) != 0)
	{
		printf("Error al crear Thread Receiver\n");
		exit(1);
	}
		if(pthread_create(&sender, NULL, senderT, (void*) &sockfd) != 0)
	{
		printf("Error al crear Thread Sender\n");
		exit(1);
	}
	
	pthread_join(sender, NULL);
	pthread_join(receiver, NULL);
}

void* receiverT(void* arg){
	int n, sockfd;
	struct sockaddr_in recaddr;
	char recline[PACKET_SIZE];
	
	sockfd = *((int*) arg);
	
	recaddr.sin_family = AF_INET;htonl(INADDR_ANY);
	recaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	recaddr.sin_port = htons(rec_port);
	bind(sockfd, (struct sockaddr*) &recaddr, sizeof(recaddr));
	
	printf("Receiver started on port: %d\n", rec_port);
	for(;;){
		n = recvfrom(sockfd, recline, PACKET_SIZE, 0, NULL, NULL);
		recline[n] = 0;
		if(n > 0)
			printf("<-- %s", recline);
	}	
	printf("Receiver thread ended\n");
}

void* senderT(void* arg){
	int sockfd;
	struct sockaddr_in servaddr;
	char sendline[PACKET_SIZE];

	sockfd = *((int*) arg);

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr=inet_addr(remote_address);
	servaddr.sin_port=htons(send_port);

	printf("Sender started on port: %d\n", send_port);
	while (fgets(sendline, 10*PACKET_SIZE,stdin) != NULL)
		{
			pthread_mutex_lock(&lock);
			sendto(sockfd,sendline,strlen(sendline),0,(struct sockaddr *)&servaddr,sizeof(servaddr));
			pthread_mutex_unlock(&lock);
			printf("--> %s", sendline);
		}
	printf("Sender thread ended\n");
}