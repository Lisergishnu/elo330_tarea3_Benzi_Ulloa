#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MSG_SIZE 1500
#define LIST_SIZE 10000

/* Estructura que describe un paquete recibido / enviado */
typedef struct pkg
{
  char msg[MSG_SIZE];
  time_t timestamp;
} datagram;

int delay_avg, delay_variation;
int loss_percent, local_port, remote_port;
char remote_host[30] = "127.0.0.1";

void* senderThread(void*);
void* recieverThread(void*);
void initList(datagram**, int);
void lagger(double, double);
int randsafe(double *);

char mesg[MSG_SIZE] = "";

pthread_mutex_t listMutex = PTHREAD_MUTEX_INITIALIZER;
datagram* mesgList[LIST_SIZE];
int mesgIndex = 0, sendIndex = 0;

/* Declaraciones implicitas para suprimir warnings */
int main(int argc, char* argv[])
{
    /* Main funcion, aqui deberia ir la creacion de hebras, a una hebra se le asigna el cliente Interno
    * y la otra se le asigna el serverInterno.
    * el server Interno escucha en un puerto X, pasa los datos al clienteInterno, y este mete el lag
    */

    /* Lectura de parametros */
    if(argc == 6)
        {
            delay_avg = atoi(argv[1]);
            delay_variation = atoi(argv[2]);
            loss_percent = atoi(argv[3]);
            local_port = atoi(argv[4]);
            remote_port = atoi(argv[5]);
        }
    else if(argc == 7)
        {
            delay_avg = atoi(argv[1]);
            delay_variation = atoi(argv[2]);
            loss_percent = atoi(argv[3]);
            local_port = atoi(argv[4]);
            strcpy(remote_host, argv[5]);
            remote_port = atoi(argv[6]);
        }
    else
        {
            printf("Usage:\nerp_udp Delay_avg Delay_variation Loss_percent Local_port [Remote_host] Remote_port\n");
            exit(1);
        }
    printf("Delay_avg: %d\nDelay_variation: %d\nLoss_percent: %d\nLocal_port: %d\nRemote_host: %s\nRemote_port: %d\n", delay_avg, delay_variation, loss_percent, local_port, remote_host, remote_port);
    initList(mesgList, LIST_SIZE);

    /* Creacion de los Thread */
    pthread_t sender, listener;
    if(pthread_create(&sender, NULL, senderThread, NULL) != 0)
        {
            printf("Error al crear Thread Sender\n");
            exit(1);
        }
    if(pthread_create(&listener, NULL, recieverThread, NULL) != 0)
        {
            printf("Error al crear Thread Listener\n");
            exit(1);
        }
    pthread_join(sender, NULL);
    pthread_join(listener, NULL);
    return 0;
}

void* senderThread(void* arg)
{
    /* Cliente UDP, establece una conexion a un servidor con la IP especificada ipAddr */
    printf("Thread Client Started on port: %d\n", remote_port);

    // Descriptor del socket
    int sockfd;
    // Direccion del socket
    struct sockaddr_in servaddr;

    // Se abre el socket para INET, tipo UDP y devuelve el descriptor del socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1)
        {
            printf("ERROR al crear descriptor en internalClient\n");
            pthread_exit((void*)1);
        }

    /* Rellena con \0 sizeof(serveraddr) bytes a partir de serveraddr
    * para evitar errores en la informacion */
    bzero(&servaddr, sizeof(servaddr));

    // Requisitos minimos de serveaddr para uso posterior
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(remote_host);
    servaddr.sin_port = htons(remote_port);

    for(;;)
        {
            if(NULL != mesgList[sendIndex])
                {
                    int error;
                    if ((error = pthread_mutex_lock( &listMutex )) != 0)
                        fprintf(stderr,"ERROR: %d\n", error);
                    else
                        {
                            strcpy(mesg, mesgList[sendIndex]->msg);
                            lagger(delay_avg, delay_variation);

                            if(strcmp(mesg, "") != 0)
                                sendto(sockfd, mesg, strlen(mesg), 0,(struct sockaddr*) &servaddr, sizeof(servaddr));
                            printf("Mesg: %s\nmesgList[%d]: %s\nOutbound timestamp: %s\n", mesg, sendIndex,
                                mesgList[sendIndex]->msg, ctime(&mesgList[sendIndex]->timestamp));
                            free(mesgList[sendIndex]);
                            mesgList[sendIndex] = NULL;
                            sendIndex = (sendIndex+1) % LIST_SIZE;
                        }
                    pthread_mutex_unlock( &listMutex );
                }
        }
    printf("Thread Client ENDED\n");
    pthread_exit((void*)0);
}

void* recieverThread(void* arg)
{
    printf("Thread Server Started on port: %d\n", local_port);
    int sockfd, n;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    // Requisitos minimos de serveaddr para uso posterior
    // INADDR_ANY: atender a cualquier cliente
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(local_port);
    // Bindea al descriptor de archivo con el socketaddr del servidor
    bind(sockfd,(struct sockaddr*) &servaddr, sizeof(servaddr));

    for(;;)
        {
            len = sizeof(cliaddr);
            n = recvfrom(sockfd,mesg,MSG_SIZE,0,(struct sockaddr *)&cliaddr,&len);
            sendto(sockfd, mesg, n, 0, (struct sockaddr*) &cliaddr, sizeof(cliaddr));

            int error;
            if ((error = pthread_mutex_lock( &listMutex )) != 0)
                fprintf(stderr,"ERROR: %d\n", error);
            else
                {
                  if (NULL == mesgList[mesgIndex])
                  {
                    datagram *n = malloc(sizeof(datagram));
                    if (NULL == n)
                    {
                      fprintf(stderr,"ERROR: Couldn't allocate memory for datagram\n");
                      abort();
                    }
                    strcpy(n->msg,mesg);
                    n->timestamp = time(NULL);
                    mesgList[mesgIndex] = n;

                    printf("-------------------------------------------------------\n");
                    printf("Received the following:\n");
                    printf("%s\n",mesgList[mesgIndex]->msg);
                    printf("Inbound timestamp: %s\n", ctime(&mesgList[mesgIndex]->timestamp));
                    printf("-------------------------------------------------------\n");

                  }
                  else
                  {
                    printf("Buffer pos %d overwritten\n",mesgIndex);
                    sendIndex = (sendIndex+1) % LIST_SIZE;
                  }
                  mesgIndex=(mesgIndex+1)%LIST_SIZE;

                }
            pthread_mutex_unlock( &listMutex );
        }
    printf("Thread Server ENDED\n");
    pthread_exit((void*)0);
}

void initList(datagram ** list, int size)
{
  int i;
  for(i=0; i<size; i++)
  {
    list[i] = NULL;
  }
}

void lagger(double avg, double variation)
{
  double random;
  double delay;

  if(randsafe(&random) != 0)
    return;
  printf("Random created = %f\n", random);

  /* Le doy un 2.5% mas de rango a la variacion total hacia arriba y hacia abajo */
  double fixed_variation = variation*(1 + (double)loss_percent/100);
  delay = 2*random*fixed_variation + avg - fixed_variation;

  if(delay > avg + variation || delay < avg - variation)
  {
    strcpy(mesg, "");
    printf("**********************Packet Loss*********************\n");
  }
  printf("Delay: %f\n", delay);

  if(delay > 1000)
  {
    /* Segun documentacion, no llamar a usleep con mas de 1M us*/
    printf("Too much lag\n");
    return;
  }

  usleep((int) delay *1000);
}

int randsafe(double *ranp)
{
  static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
  int error= 0;

  if ((error = pthread_mutex_lock(&lock)))
    return error;
  *ranp = (double)rand()/RAND_MAX;
  return pthread_mutex_unlock(&lock);
}
