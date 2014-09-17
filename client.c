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

r_connection(char buffer[]) {
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
    bzero(buffer, 256);
    int n;
    n = write(sockfd, "HELO dominio",strlen(buffer));
    printf("%s\n", "envié un helo");
    if (n < 0){ 
        error("Error al escribir en el socket");
    }
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
  printf("entro a mail\n%s\n", line);
  int n;
  n = write(sockfd, line, strlen(line));
  printf("Intentando mandar mensaje\n");
  if (n < 0){ 
    error("Error al escribir en el socket");
  }
  printf("mensaje enviado\n");
  
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
  if (strcmp(tmp, "MAIL FROM:") == 0)
    mail_from(sockfd, line);
  free(tmp);
}

/* void init_mail_transaction() { */
/*   char* from_mail; */
  
/* } */

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
    // se lee el primer mensaje
    n = read(sockfd, buffer, 255);
    //Verifica la respuesta que me mandó el servidor y respondemos adecuadamente
    int boo1 = r_connection(buffer);
    if (!(boo1==1)) {
        close(sockfd);
        return 0;
    } else {
      int n;
      // escribe mensaje2
      bzero(buffer, 256);
      n = write(sockfd, "HELO dominio", 256);
      printf("%s\n", "envié un helo");
      if (n < 0){
        error("Error al exscribir en el socket");
      }
      // helo(sockfd, buffer);
    }
    /* if (n < 0){ */
    /*     error("Error al leer desde el socket"); */
    /* } */
    bzero(buffer, 256);
    // printf("Estoy leyendo %s\n", buffer);
    // lee mensaje 3
    n = read(sockfd, buffer, 256);
    if (n < 0){
            error("Error al leer desde el socket");
    }
    n = read(sockfd, buffer, 256);
    // NO IMPRIME EL BUFFER CORRECTAMENTE...PORQUE???
    printf("Te has conectado a GMAIL %s \n", buffer);

    /* bzero(buffer,256); */
    /* n = read(sockfd, buffer, 255); */
    /* printf("buffer a comparar %s\n", buffer); */


    char* buffer1 = (char*)malloc(sizeof(char)*254);
    char* tmp = (char*)malloc(sizeof(char)*254);
    int len;
    while(sockfd) {
      //  bzero(buffer,256); 
      printf("Por favor ingresa el comando\n> ");
      // lee el comando
      bzero(buffer1, 254); 
      bzero(tmp, 254);
      fgets(buffer1, 254, stdin);
      len =  strlen(buffer1); // longitud cadena
      // quita el espacio al final
      strncpy(tmp, buffer1, len-1);
      get_command(sockfd, tmp);

      bzero(buffer, 256);
      n = read(sockfd, buffer, 256);
      if (n < 0){
	error("Error al leer desde el socket");
      }
      printf("%s\n", buffer);

      //n = write(sockfd, buffer1, 256);
        /* n = write(sockfd,buffer,strlen(buffer)); */
        /* if (n < 0){  */
        /*     error("Error al escribir en el socket"); */
        /* } */
        
       
        /* if (n < 0){ */
        /*     error("Error al leer desde el socket"); */
        /* } */
        /* printf("BUFFER A COMPARAR EN EL SIGUIENTE WHILE %s\n",buffer); */
    }
    close(sockfd);
    return 0;
}
