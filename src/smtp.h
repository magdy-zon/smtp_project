/* Archivo de cabeceras para el archivo stmp.c aqui estan definidas todas las 
 * variables de uso y tambien definimos los metodos que vamos a usar mas adelante
 * para la comunicacion con el servidor SMTP */
#include "parseador.h"
// Constante que define el mensaje de HELO que siempre enviaremos
#define HELO "HELO redes2015.ciencias.unam.mx \x0D\x0A"
// Constante que nos define el mensaje para cerrar la comunicacion
#define QUIT "QUIT \x0D\x0A"
#define R_HOST "@redes2015.ciencias.unam.mx"
#ifndef SMTP_H

typedef struct arreglo_datos{
    char *usuario;
    char *direccion;
} Usuario;


/* Esta funcion nos sirve para manejar toda la sesion que se va a llevar acabo entre el cliente
 * smtp y el servidor como parametros se recibe el identificador generado por el socket en la funcion 
 * principal */ 
int sesion(int socket);

/* Esta funcion nos servira para procesar un estado recibido por el servidor SMTP 
 * para saber si tenemos acceso al servicio o nos rechazo el acceso el codigo del
   estado se encuentra en la primera posicion del arreglo estado */
int procesa_estado(char** estado); 

/* Esta funcion nos va a servir para obtener los datos del usuario que quiere mandar
   el correo al servidor SMTP */
Usuario* ingresa_usuario();

/* Esta funcion nos va a servir para recopilar los datos del mensaje que se va a enviar
   a traves de la terminal en un principio */
Mensaje* crea_mensaje();
char* crea_cuerpo_mensaje();
int es_punto(char*);
#endif
