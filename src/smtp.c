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
    char buffer_entrada[1000], buffer_salida[1000];
    // Esperamos a que el servidor nos de el estado del servicio
    // Si quisieramos hacer varios intentos podriamos meter este paso
    // en un ciclo
    hay_error = read(socket,buffer_entrada,1000);
    if (hay_error < 0) 
         error("Hubo un error al leer desde el socket verifica como fueron creados e intentalo de nuevo");
    printf("%s\n", buffer_entrada);
    // Generamos un arreglo que tenga cada valor de la linea del estado por separado
    // Funcion usada en la practica pasada
    char** estado = separa_request_line(buffer_entrada);
    // Si el servicio no esta disponible se sale
    if(procesa_estado(estado) != 0){
        hay_error = 1;
        close(socket);
        return hay_error;
    }
    // Limpiamos los buffers de comunicacion
    bzero(buffer_entrada, 1000);
    bzero(buffer_salida, 1000);
    // Mandamos el mensaje HELO de listo
    hay_error = write(socket, HELO , strlen(HELO));
    // Pedimos los datos al usuario
    Usuario* user = ingresa_usuario();
    // Generamos el mail from con los datos del usuario
    char* origen = crea_mail_from(user->direccion);
    // y lo mandamos
    hay_error = write(socket, origen , strlen(origen));
    // leemos la confirmacion
    hay_error = read(socket,buffer_entrada,1000);
    printf("Confirmacion:\n%s\n", buffer_entrada);
    bzero(buffer_entrada, 1000);
    // Creamos el mensaje del usuario
    Mensaje* msj = crea_mensaje();
    // Generamos el rcpt to con los datos del mensaje
    char* destino = crea_rcpt_to(msj->destinatario);
    // y lo mandamos
    hay_error = write(socket, destino , strlen(destino));
    // leemos la confirmacion
    hay_error = read(socket,buffer_entrada,1000);
    printf("Confirmacion:\n%s\n", buffer_entrada);
    bzero(buffer_entrada, 1000);
    // Y pedimos los datos para escribir el mensaje
    hay_error = write(socket, "DATA \x0D\x0A" , 7);
    // leemos los datos
    hay_error = read(socket,buffer_entrada,1000);
    printf("Confirmacion:\n%s\n", buffer_entrada);
    bzero(buffer_entrada, 1000);
    // Mandamos el mensaje para terminar la comunicacion
    hay_error = write(socket, QUIT , strlen(QUIT));
    printf("Funciono con exito :)\n");
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
    printf("Por favor ingresa tus datos\n");
    printf("Usuario: ");
    scanf("%s", user->usuario);
    printf("\nDireccion de correo (incluyendo host): ");
    scanf("%s", user->direccion);
    printf("\n");
    return user;
}

/* Creamos el mensaje */
Mensaje* crea_mensaje(){
    Mensaje* msj = malloc(sizeof(Mensaje));
    printf("Por favor ingresa los datos del mensaje\n");
    printf("Destinatario: ");
    scanf("%s", msj->destinatario);
    printf("Cuerpo del Mensaje: \n");
    scanf("%s", msj->mensaje);
    printf("\nListo! se esta procesando tu mensaje...");
    return msj;
}