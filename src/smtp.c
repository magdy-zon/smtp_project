/* 
 * Servidor simple de SMTP
 * Este servidor se utiliza a traves de la clase principal (pendiente...)
 * El metodo principal de este archivo es sesion donde administras toda la conexion
 * con el servidor STMP para mas informacion ver las cabeceras de este archivo
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "smtp.h"

/* Esta funcion nos servira para administrar nuestra sesion que tengamos con el 
   servidor SMTP */
int sesion(int socket){
    // Funcion que nos representa si se ha encontrado un error
    int hay_error;
    // Tamaño del buffer debe ser suficiente para almacenar los mensajes recibidos
    char buffer_entrada[2000], buffer_salida[1000];
    // Esperamos a que el servidor nos de el estado del servicio
    // Si quisieramos hacer varios intentos podriamos meter este paso
    // en un ciclo
    hay_error = read(socket,buffer_entrada,2000);
    if (hay_error < 0) 
         error("Hubo un error al leer desde el socket verifica como fueron creados e intentalo de nuevo");
    printf("%s\n", buffer_entrada);
    // Generamos un arreglo que tenga cada valor de la linea del estado por separado
    // Funcion usada en la practica pasada
    /*char** estado = separa_request_line(buffer_entrada);
    // Si el servicio no esta disponible se sale
    if(procesa_estado(estado)){
        hay_error = 1;
        close(socket);
        return hay_error;
    }*/
    // Limpiamos los buffers de comunicacion
    bzero(buffer_entrada, 2000);
    bzero(buffer_salida, 1000);
    // Mandamos el mensaje HELO de listo
    hay_error = write(socket, HELO , strlen(HELO));
    hay_error = read(socket,buffer_entrada,2000);
    printf("Confirmacion1:\n%s\n", buffer_entrada);
    bzero(buffer_entrada, 2000);
    // Pedimos los datos al usuario
    Usuario* user = ingresa_usuario();
    // Generamos el mail from con los datos del usuario
    char* origen = crea_mail_from(user->direccion);
    printf("\nEste es el mensaje Mailfrom a mandar: %s\n", origen);
    // y lo mandamos
    hay_error = write(socket, origen , strlen(origen));
    // leemos la confirmacion
    hay_error = read(socket,buffer_entrada,2000);
    printf("Confirmacion2:\t%s\n", buffer_entrada);
    bzero(buffer_entrada, 2000);
    // Creamos el mensaje del usuario
    Mensaje* msj = crea_mensaje();
    // Enviamos el mensaje a los destinatarios que introdujo
    int i = 0;
    while(strlen(msj->destinatario[i])>0)
    {
        // Generamos el rcpt to con los datos del mensaje
        char* destino = crea_rcpt_to(msj->destinatario[i]);    
        // y lo mandamos
        printf("Este es el mensaje rcpt to: %s\n", destino);
        hay_error = write(socket, destino , strlen(destino));
        // leemos la confirmacion
        hay_error = read(socket,buffer_entrada,2000);
        i++;
    }
    printf("Confirmacion3:\n%s\n", buffer_entrada);
    bzero(buffer_entrada, 2000);
    // Y pedimos los datos para escribir el mensaje
    hay_error = write(socket, "DATA \x0D\x0A" , 7);
    // leemos los datos
    hay_error = read(socket,buffer_entrada,2000);
    printf("Confirmacion4:\n%s\n", buffer_entrada);
    bzero(buffer_entrada, 2000);
    // Y escribimos el mensaje
    // char* cuerpo_mensaje = crea_cuerpo(msj->mensaje);
    // Mandamos el mensaje por el buffer
    hay_error = write(socket, msj->mensaje , strlen(msj->mensaje));
    hay_error = write(socket, "\x0D\x0A.\x0D\x0A" , strlen(msj->mensaje));
    // leemos la confirmacion
    hay_error = read(socket,buffer_entrada,2000);
    printf("Confirmacion5:\n%s\n", buffer_entrada);
    // Mandamos el mensaje para terminar la comunicacion
    bzero(buffer_entrada, 2000);
    hay_error = write(socket, QUIT , strlen(QUIT));
    printf("Funciono con exito :)\n");
    // Cerramos la conexion
    close(socket);
    // Regresamos positivo de que todo salio bien :)
    return hay_error;
}

/* Procesamos el estado... */
int procesa_estado(char** estado){
    // Variable que nos indica si el servicio esta activo
    int activo, i;
    for(i=0; i<6; i++){
        printf("El elemento %d del request es: %s \n", i, estado[i]);
    }
    // Verificamos que el estado que nos respondio el server sea realmente
    // 202, si quisieramos verificar mas condiciones del servicio este seria
    // el metodo donde hacerlo
    printf("Este es el estado que recibimos del server: %s \n", estado[0]);
    if(strncmp(estado[0], "202", 3)){
        activo = 0;
        printf("El servidor SMTP esta listo para la comunicacion :) !\n");
    } else if(strncmp(estado[0], "354", 3)){
        activo = 0;
        printf("El servidor SMTP esta listo para mandar el mensaje :) !\n");
    } else {
        activo = 1;
        printf("El servidor SMTP no tiene el servicio disponible :( checa\n");
        printf("tus datos de conexion SMTP y vuelve a intentarlo mas tarde\n");
    }
    return activo;
}

/* Registramos los datos del usuario... */
Usuario* ingresa_usuario(){
    Usuario* user = malloc(sizeof(Usuario));
    user->usuario = malloc(sizeof(char) * 50);
    user->direccion = malloc(sizeof(char) * 150);
    printf("-POR FAVOR INGRESA TUS DATOS-\n");
    printf("\nUsuario: ");
    scanf("%s", user->usuario);
    printf("Direccion de correo (incluyendo host): ");
    scanf("%s", user->direccion);
    printf("\n");
    return user;
}

/* Creamos el mensaje */
Mensaje* crea_mensaje(){
    Mensaje* msj = malloc(sizeof(Mensaje));
    //Reservamos memoria para el mensaje
    char mensj[1000];
    //Reservamos memoria para el conjunto de destinatarios
    msj->destinatario = malloc(sizeof(char*)*10);
    //Reservamos memoria para cada destinatario
    int reserva;
    for(reserva = 0; reserva < 10 ; reserva++)
        msj->destinatario[reserva] = malloc(sizeof(char)*50);
    printf("Por favor ingresa los datos del mensaje\n");
    char respuesta[2];
    int i=0;
    int bandera = 1;
    while(bandera==1)
    {
        printf("Destinatario: ");
        scanf("%s", msj->destinatario[i]);
        printf("PARA:%s\n", msj->destinatario[i]);
        printf("%s\n", "¿Deseas agregar otro destinatario? ¿SI / NO?");
        scanf("%s", respuesta);        
        if(strncmp("SI", respuesta,1)==0){
            bandera = 1;
            i++;
        }
        else
        {
            bandera = 0;
        }
    }
    
    printf("Por favor escribe tu mensaje: (Para terminar teclea . y luego enter)\n");
    char* buf = (char*)malloc(sizeof(char)*1000);
    buf = crea_cuerpo_mensaje();
    msj->mensaje = buf;
    
    printf("\nListo! se esta procesando tu mensaje...\n");

    return msj;
}
/*
  Solicita al usuario que teclee el cuerpo del mensaje completo, lo procesa y
  guarda mensaje final en buffer. Para cuando el suario ingresa el caracter '.'
 */
char* crea_cuerpo_mensaje() {
  // Aqui va guardando lo que el usuario va teleando como parte del mensaje

  char* bufer2 = (char*)malloc(sizeof(char)*1000);
  char bufer1[256];
  int es_un_punto = 1;
  bzero(bufer2, 1000);
  bzero(bufer1, 256);
  // Agregado solo como un parche para que no agregue un salto de line al principio
  getchar();
  printf("ASUNTO >");
  fgets(bufer1, 250, stdin);
  strcat(bufer2, "Subject: ");
  do{
    strcat(bufer2, bufer1);
    bzero(bufer1, 256);
    printf("MENSAJE >");
    //  printf("cadena ingresada: %s", bufer1);
    fgets(bufer1, 250, stdin);
  }while (!es_punto(bufer1));
  strcat(bufer2, "\x0D\x0A.\x0D\x0A");
  printf("texto: %s", bufer2);
  return bufer2;
}
/*
  Verifica si una cadena es un punto.
  Para esto, se fija si la longiutd es 2, el punto más el salto de linea, 
  en ese caso y si es igual al cracter . entonces sí escribieron un punto. 
 */
int es_punto(char* cadena) {
  char elemento;
  elemento = cadena[0];
  if (strlen(cadena) == 2 && elemento == '.'){
    //printf("%c", elemento);
    return 1;
  }
  return 0;
}