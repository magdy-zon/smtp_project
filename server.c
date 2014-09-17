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

void quit(int, char*);
void mail_from(int, char*);
void get_command(int, char*);
int is_valid_mail(char*);

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

     listen(sockfd, 5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);

     if (newsockfd < 0) {
          error("Error 421. Servicio no disponible");
     }
     printf("%s\n", "Conexión establecida");
     bzero(buffer,256);
     //Mandamos una respuesta positiva de que pudimos responderle al cliente, primer mensaje que manda
     n = write(newsockfd, "200 OK", 256);
     error_esc(n);
     bzero(buffer, 256);
     // lee mensaje 2
     n = read(newsockfd, buffer,256);
     printf("recibí un %s\n", buffer);
     error_lect(n);
     
     // escribe mensaje 3
     n = write(newsockfd, "Soy GMAIL y mi dominio es:\n", 256);
     error_esc(n);
     printf("despues de mandar gmail\n");

    while(strstr(buffer, "QUIT")==NULL) {
        bzero(buffer,256);
        n = read(newsockfd, buffer,256);
        error_lect(n);
        printf("Procesando el  mensaje  \" %s \" recibido\n", 
	       buffer);
	get_command(newsockfd, buffer);
	//n = write(newsockfd,buffer,256);
        error_esc(n);
    }
    close(newsockfd);
    close(sockfd);
    return 0; 
}

void quit(int sockfd, char* line) {
  printf("entro a quit\n%s\n",line);
  //  bzero(buffer, 256);
  int n;
  n = write(sockfd, line, strlen(line));
  printf("%s\n", "Intentando mandar mensaje");
  if (n < 0){ 
    error("Error al escribir en el socket");
  }
  printf("mensaje enviado\n");
}

void mail_from(int sockfd, char* line) {
  int n;
  if (is_valid_mail(line)) {
    printf("EL CORREO SÍ EXISTE, PROCEDEMOS!\n");
    // Manda respuesta positiva
    n = write(sockfd, "250 ok", 50);
    printf("Mandadno respuesta de éxito...\n");
    if (n < 0){
      error("Error al escribir en el socket");
    }
    printf("Mensaje enviado!\n");
    
  } else {
    printf("EL CORREO NO EXISTE!\n");
    // Manda respuesta negativa;
  } 

  //printf("El correo %s entro a mail\n", line);
  
}

void get_command(int sockfd, char* line) {
  char *tmp;
  
  // Para quit
  tmp = malloc(sizeof(char)*4);
  strncpy(tmp, line, 4);
  tmp[4] = '\0';

  if (strcmp(tmp, "QUIT") == 0) {
    quit(sockfd, line);
  }
  free(tmp);

  // Para mail from
  tmp = malloc(sizeof(char)*10);
  strncpy(tmp, line, 10);
  tmp[10] = '\0';
  if (strcmp(tmp, "MAIL FROM:") == 0){
    mail_from(sockfd, line);
  }
  free(tmp);

  printf("termina for");
  
}

int is_valid_mail(char* correo){
  return 1;
}
