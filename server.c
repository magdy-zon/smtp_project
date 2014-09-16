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
void error_esc(int n)
{
    if (n < 0) 
     {
        error("Error al escribir en el socket");
     }
}
void error_lect(int n)
{
    if (n < 0) 
     {
        error("Error al leer desde el socket");
     }
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
     printf("Iniciando la ejecución en el puerto: %d\n",portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("Error al enlazar");

     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);

     if (newsockfd < 0) {
          error("Error 421. Servicio no disponible");
     }
     printf("%s\n", "Conexión establecida");
     bzero(buffer,256);
     //Mandamos una respuesta positiva de que pudimos responderle al cliente
     n = write(newsockfd,"200 OK", 256);
     error_esc(n);
     bzero(buffer,256);
     printf("%s\n", "recibí un HELO");
     n = read(newsockfd, buffer,256);
     error_lect(n);
     
     n = write(newsockfd,"Soy GMAIL y mi dominio es:",256);
     error_esc(n);


    while(strstr(buffer,"QUIT")==NULL){
        bzero(buffer,256);
        n = read(newsockfd, buffer,256);
        error_lect(n);
        printf("Aquí está el mensaje recibido: %s\n",buffer);
        n = write(newsockfd,buffer,256);
        error_esc(n);
    }
    close(newsockfd);
    close(sockfd);
    return 0; 
}
