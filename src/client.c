/*
 * Cliente simple para el envío de mensajes al servidor.
 * Modo de uso: ./client IPSERVER PORTSERVER
 * donde IPSERVER es la ip del servidor y 
 * PORTSERVER es el puerto del servidor.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    // Basandonos en el codigo de la practica 2
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char *buffer;
    if (argc < 3) {
       fprintf(stderr,"usando el puerto %s\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("Error al abrir el socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"Error, no se encontro el servidor\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("Error al iniciar la conexión");
    printf("Iniciando la comunicacion con el servidor SMTP...\n");
    // En esta linea empezamos a manejar la sesion SMTP
    int correcta = sesion(sockfd);
    return correcta;
}
