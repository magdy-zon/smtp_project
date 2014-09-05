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
#include "parseador.h"

/* Esta funcion nos servira para administrar nuestra sesion que tengamos con el 
   servidor SMTP */
int sesion(int socket){
    // Funcion que nos representa si se ha encontrado un error
    int hay_error;
    // Tamaño del buffer debe ser suficiente para almacenar los mensajes recibidos
    char buffer[2000];
    // Esperamos a que el servidor nos de el estado del servicio
    // Si quisieramos hacer varios intentos podriamos meter este paso
    // en un ciclo
    hay_error = read(socket,buffer,2000);
    if (hay_error < 0) 
         error("Hubo un error al leer desde el socket verifica como fueron creados e intentalo de nuevo");
    // Generamos un arreglo que tenga cada valor de la linea del estado por separado
    // Funcion usada en la practica pasada
    char** estado = separa_request_line(buffer);
    procesa_estado(estado);
    printf("%s\n", buffer);
    close(socket);
    // Regresamos positivo de que todo salio bien :)
    return 0;
}

int procesa_estado(char** estado){
    // Variable que nos indica si el servicio esta activo
    int activo;
    // Verificamos que el estado que nos respondio el server sea realmente
    // 202, si quisieramos verificar mas condiciones del servicio este seria
    // el metodo donde hacerlo
    if(!(strncmp(estado[0], "202", 3))){
        activo = 0;
        printf("El servidor SMTP esta listo para la comunicacion :)!\n");
    } else {
        activo = 1;
        printf("El servidor SMTP no tiene el servicio disponible :( checa\n");
        printf("tus datos de conexion SMTP y vuelve a intentarlo mas tarde\n");
    }
    return activo;
}