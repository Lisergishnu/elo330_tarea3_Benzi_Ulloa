#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT 9000

int main(int argc, char* argv[])
{
	int sockfd,n;
	struct sockaddr_in servaddr,cliaddr;
	char sendline[1000];
	char recvline[1000];

	if (argc != 3)
		{
			printf("usage:  udpcli <IP address> <Port>\n");
			exit(1);
		}

	sockfd=socket(AF_INET,SOCK_DGRAM,0);

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr=inet_addr(argv[1]);
	servaddr.sin_port=htons(atoi(argv[2]));

	printf("Client started on port: %d\n", atoi(argv[2]));
	while (fgets(sendline, 10000,stdin) != NULL)
		{
			sendto(sockfd,sendline,strlen(sendline),0,
			       (struct sockaddr *)&servaddr,sizeof(servaddr));
			n=recvfrom(sockfd,recvline,10000,0,NULL,NULL);
			recvline[n]=0;
			fputs(recvline,stdout);
		}
}