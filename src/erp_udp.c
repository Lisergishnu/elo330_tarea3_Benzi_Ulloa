#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
   /* Main funcion, aqui deberia ir la creacion de hebras, a una hebra se le asigna el clienteInterno
   * y la otra se le asigna el serverInterno. 
   * el server Interno escucha en un puerto X, se lo pasa al clienteInterno, y este hace mete el lag
   */
   int delay_avg, delay_variation;
   int loss_percent, local_port, remote_port;
   char remote_host[30] = "127.0.0.1";
   
	if(argc == 5){
		delay_avg = atoi(argv[1]);
		delay_variation = atoi(argv[2]);
		loss_percent = atoi(argv[3]);
		local_port = atoi(argv[4]);
		remote_port = atoi(argv[5]);
	}
	else if (argc == 6){
		delay_avg = atoi(argv[1]);
		delay_variation = atoi(argv[2]);
		loss_percent = atoi(argv[3]);
		local_port = atoi(argv[4]);
		remote_host = argv[5];
		remote_port = atoi(argv[6]);
	}
	else{
		printf("Usage:\nerp_udp Delay_avg Delay_variation Loss_percent Local_port [Remote_host] Remote_port\n");
	}

}

void client(char* ipAddr){
	/* Cliente UDP, establece una conexion a un servidor con la IP especificada ipAddr*/
   int sockfd,n;
   struct sockaddr_in servaddr,cliaddr;
   char sendline[1000];
   char recvline[1000];

   sockfd=socket(AF_INET,SOCK_DGRAM,0);

   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=inet_addr(ipAddr);
   servaddr.sin_port=htons(32000);

   while (fgets(sendline, 10000,stdin) != NULL)
   {
      sendto(sockfd,sendline,strlen(sendline),0,
             (struct sockaddr *)&servaddr,sizeof(servaddr));
      n=recvfrom(sockfd,recvline,10000,0,NULL,NULL);
      recvline[n]=0;
      fputs(recvline,stdout);
   }
}

void server(){
   int sockfd,n;
   struct sockaddr_in servaddr,cliaddr;
   socklen_t len;
   char mesg[1000];
   
   sockfd=socket(AF_INET,SOCK_DGRAM,0);

   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
   servaddr.sin_port=htons(32000);
   bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

   for (;;)
   {
      len = sizeof(cliaddr);
      n = recvfrom(sockfd,mesg,1000,0,(struct sockaddr *)&cliaddr,&len);
      sendto(sockfd,mesg,n,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
      printf("-------------------------------------------------------\n");
      mesg[n] = 0;
      printf("Received the following:\n");
      printf("%s",mesg);
      printf("-------------------------------------------------------\n");
   }
}