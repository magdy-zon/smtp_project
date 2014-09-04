/* 
 * Servidor simple usando sockets con TCP
 * Modo de ejecución: ./server PORT
 * donde PORT es el puerto en el que va a estar escuchando
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"Error, no se ha dado un puerto\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("Error al abrir el socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     printf("Iniciand la ejecución en el puerto: %d\n",portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("Error al enlazar");

     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("Error al ejecutar la funcion accept");
     printf("Conexión aceptada.\n");
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) error("Error al leer desde el socket");
     printf("Aquí está el mensaje recibido: %s\n",buffer);
     n = write(newsockfd,"He recibido tu mensaje",255);
     if (n < 0) error("Error al escribir en el socket");
     close(newsockfd);
     close(sockfd);
     return 0; 
}