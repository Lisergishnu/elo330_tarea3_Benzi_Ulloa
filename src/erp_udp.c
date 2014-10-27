#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define MSG_SIZE 1000

int delay_avg, delay_variation;
int loss_percent, local_port, remote_port;
char remote_host[30] = "127.0.0.1";

void* client(void*);
void* server(void*);

char internalBuffer[50*MSG_SIZE];

int main(int argc, char* argv[])
{
	/* Main funcion, aqui deberia ir la creacion de hebras, a una hebra se le asigna el cliente Interno
	* y la otra se le asigna el serverInterno.
	* el server Interno escucha en un puerto X, pasa los datos al clienteInterno, y este mete el lag
	*/
	
	/* Lectura de parametros */
	if(argc == 6){
		delay_avg = atoi(argv[1]);
		delay_variation = atoi(argv[2]);
		loss_percent = atoi(argv[3]);
		local_port = atoi(argv[4]);
		remote_port = atoi(argv[5]);
	}
	else if(argc == 7){
		delay_avg = atoi(argv[1]);
		delay_variation = atoi(argv[2]);
		loss_percent = atoi(argv[3]);
		local_port = atoi(argv[4]);
		strcpy(remote_host, argv[5]);
		remote_port = atoi(argv[6]);
	}
	else{
		printf("Usage:\nerp_udp Delay_avg Delay_variation Loss_percent Local_port [Remote_host] Remote_port\n");
		exit(1);
	}
	printf("Delay_avg: %d\nDelay_variation: %d\nLoss_percent: %d\nLocal_port: %d\nRemote_host: %s\nRemote_port: %d\n", delay_avg, delay_variation, loss_percent, local_port, remote_host, remote_port);
	
	/* Creacion de los Thread */
	pthread_t sender, listener;
	int t_s, t_l;
	if(pthread_create(&sender, NULL, client, NULL) != 0){
		printf("Error al crear Thread Sender\n");
		exit(1);
	}
	if(pthread_create(&listener, NULL, server, NULL) != 0){
		printf("Error al crear Thread Listener\n");
		exit(1);
	}
	pthread_join(sender, NULL);
	pthread_join(listener, NULL);
}

void* client(void* arg){
	/* Cliente UDP, establece una conexion a un servidor con la IP especificada ipAddr */
	printf("Thread Client Started on port: %d\n", remote_port);
	
	int sockfd, n;
	struct sockaddr_in servaddr, cliaddr;
	char sendline[MSG_SIZE];
	char recvline[MSG_SIZE];

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(remote_host);
	servaddr.sin_port = htons(remote_port);

	while(fgets(sendline, 10*MSG_SIZE, stdin) !=  NULL)
	{
		sendto(sockfd, sendline, strlen(sendline), 0,
				(struct sockaddr*) &servaddr, sizeof(servaddr));
		n = recvfrom(sockfd, recvline, 10*MSG_SIZE, 0, NULL, NULL);
		recvline[n] = 0;
		fputs(recvline, stdout);
	}
	pthread_exit((void*)0);
}

void* server(void* arg){
	printf("Thread Server Started on port: %d\n", local_port);
	int sockfd,n;
	struct sockaddr_in servaddr,cliaddr;
	socklen_t len;
	char mesg[MSG_SIZE];
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(local_port);
	bind(sockfd,(struct sockaddr*) &servaddr, sizeof(servaddr));

	for(;;)
	{
		len = sizeof(cliaddr);
		n = recvfrom(sockfd, mesg, MSG_SIZE, 0, (struct sockaddr*)&cliaddr, &len);
		sendto(sockfd, mesg, n, 0, (struct sockaddr*) &cliaddr, sizeof(cliaddr));
		printf("-------------------------------------------------------\n");
		mesg[n] = 0;
		printf("Received the following:\n");
		printf("%s",mesg);
		printf("-------------------------------------------------------\n");
	}
	pthread_exit((void*)0);
}
