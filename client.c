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

r_connection(char buffer[]){
    int boo1;
    if (strstr(buffer, "200 OK")==NULL) {
        boo1 = 0;
        printf("%s\n", "recibi un error");
    }
    else{
        boo1 = 1;
        printf("%s\n", "recibi un 200 OK");
    }
    return boo1;
}

void helo(int sockfd, char buffer[]){
    bzero(buffer,256);
    int n;
    n = write(sockfd, "HELO dominio",strlen(buffer));
    printf("%s\n", "envié un helo");
    if (n < 0){ 
        error("Error al escribir en el socket");
    }
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    /*Creamos un archivo GET.txt que tiene nuestra petición de una página*/
    FILE *archivo;
    archivo=fopen("GET.txt","r");

    char buffer[256];
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

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        error("Error al iniciar la conexión 421.");
    }
    bzero(buffer,256);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0){ 
        error("Error al escribir en el socket");
    }
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    //Verifica la respuesta que me mandó el servidor y respondemos adecuadamente
    int boo1 = r_connection(buffer);
    if (!(boo1==1)) {
        close(sockfd);
        return 0;
    }else {
        helo(sockfd, buffer);
    }
    if (n < 0){
        error("Error al leer desde el socket");
    }
    bzero(buffer,256);
    printf("Estoy leyendo %s\n", buffer);
    n = read(sockfd,buffer,255);
    if (n < 0){
            error("Error al leer desde el socket");
    }
    printf("%s\n", "Te has conectado a GMAIL");

    while(sockfd){
        bzero(buffer,256);
        fgets(buffer,255,stdin);
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0){ 
            error("Error al escribir en el socket");
        }
        
        bzero(buffer,256);
        n = read(sockfd,buffer,255);
        printf("buffer a comparar %s\n", buffer);
        
        if (n < 0){
            error("Error al leer desde el socket");
        }
        printf("BUFFER A COMPARAR EN EL SIGUIENTE WHILE %s\n",buffer);
    }
    close(sockfd);
    return 0;
}