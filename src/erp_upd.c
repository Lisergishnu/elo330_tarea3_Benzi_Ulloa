#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <strings.h>

#include <sys/param.h>
#include <sys/socket.h>
#include <sys/file.h>

#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>

float delay_avg, delay_variation;
float loss_percent;
int loca
int main (int argc, char* argv[]){
	//retardo_promedio variación_retardo porcentaje_pérdida puerto_local [host_remoto] puerto_remoto
	if(argc == 5){
		
	}
	else if (argc == 6){
	
	}
	else{
		printf("Invalid number of Inputs, Use:\nDelay_avg Delay_variation Loss_percent Local_port [Remote_host] Remote_port\n");
	}
}